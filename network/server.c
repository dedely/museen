#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h> //for logs
#include "util.h"
#include "server.h"
#include "driver.h"
#include "query.h"

#define MAX_CLIENTS 5
#define TIMEOUT 1

/**
 * @brief Spawns a server thread. We use a thread in order to keep
 *  the server running until a shutdown is requested.
 *
 */
void run(char *ip, int port, char *log_file, char *db_url) {
    in_addr_t s_ip = set_ip(ip);
    Server *server = malloc(sizeof(Server));
    if (server == NULL) {
        perror("Couldn't allocate memory in function run");
        exit(EXIT_FAILURE);
    }
    pthread_t s_tid;

    init_server(s_ip, port, log_file, db_url, server);

    /*We use a thread to run the server in order to keep it running
    * until a shutdown is requested
    */

    //Start server thread
    pthread_create(&s_tid, NULL, &run_server, server);

    //use a mutex to prevent concurrent access to the Server data structure
    pthread_mutex_lock(&server->lock);

    while ((!server->shutdown_requested) || (server->drivers_cnt > 0)) {
        pthread_cond_wait(&server->cond, &server->lock);
    }
    //unlock
    pthread_mutex_unlock(&server->lock);

    //finally, join server thread
    pthread_join(s_tid, NULL);

    //don't forget to free the Server structure
    free(server);

    // exit(EXIT_SUCCESS);
}

/**
 * @brief Initializes a Server element. (Does not allocate memory)
 *
 * @param ip
 * @param port
 * @param s
 */
void init_server(in_addr_t ip, int port, char *log_file, char *db_url, Server *s) {
    s->port = port;
    s->ip = ip;
    s->drivers_cnt = 0;
    s->shutdown_requested = 0;
    s->database_connection = connect_db(db_url);
    s->log_fd = open(log_file, O_CREAT | O_WRONLY | O_APPEND,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (s->log_fd == -1) {
        perror("Couldn't open log file as output");
    }
    pthread_mutex_init(&s->lock, NULL);
    pthread_cond_init(&s->cond, NULL);
}

void increment_drivers(Server *s) {
    pthread_mutex_lock(&s->lock);
    s->drivers_cnt++;
    pthread_mutex_unlock(&s->lock);
}

void decrement_drivers(Server *s) {
    pthread_mutex_lock(&s->lock);
    s->drivers_cnt--;
    pthread_cond_broadcast(&s->cond);
    pthread_mutex_unlock(&s->lock);
}

void write_log(char *message, int  length, Server *s) {
    pthread_mutex_lock(&s->lock);
    int fd = s->log_fd;
    if (write(fd, message, length) == -1) {
        perror("Couldn't write in log file");
    }
    pthread_cond_broadcast(&s->cond);
    pthread_mutex_unlock(&s->lock);
}

void *run_server(void *data) {
    Server *server = data;
    struct sockaddr_in cli_addr; /*Client address*/
    int cli_len; /*cli_addr lenght*/
    int s_listen, s_dial; /*socket descriptor*/
    int r, sel; /*Integers values used to process functions return values*/

    int stop = 0;

    fd_set fds, readfds;
    struct timeval timeout;

    pthread_attr_t *thread_attributes;
    pthread_t d_tid;

    in_addr_t s_ip = server->ip;
    int port = server->port;
    PGconn *conn = server->database_connection;
    s_listen = create_tcp_server(s_ip, port, MAX_CLIENTS);

    /*Create a fd_set with the server socket s_listen
    * so we can do a timed wait for incoming connections
    * using select
    */
    FD_ZERO(&fds);
    FD_SET(s_listen, &fds);

    while (!stop) {
        // Set the timeout
        timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;

        //We only wait for the server socket
        readfds = fds;
        //printf("%d connected clients \n", server->drivers_cnt);
        // Preemptively increment the drivers count
        increment_drivers(server);

        //Wait until a connection is received or until the timeout duration expires
        sel = select(s_listen + 1, &readfds, NULL, NULL, &timeout);

        /*when sel > 0, select has been unlocked because the
        * descriptor we were monitoring received something
        */
        if (sel > 0) {
            if ((FD_ISSET(s_listen, &readfds))) {
                cli_len = sizeof cli_addr;
                s_dial =
                    accept(s_listen,
                        (struct sockaddr *)&cli_addr,
                        (socklen_t *)&cli_len);
                if (s_dial < 0) {
                    perror("Couldn't accept connection");
                    exit(EXIT_FAILURE);
                }

                //Format client_info as string (ip:port)
                char cli_info[BUFFER_SIZE];
                char *cli_ip;
                char cli_port[10];
                bzero(cli_info, BUFFER_SIZE);
                cli_ip = inet_ntoa(cli_addr.sin_addr);
                bzero(cli_port, 10);
                sprintf(cli_port, ":%d", ntohs(cli_addr.sin_port));
                append_str(cli_info, cli_ip, BUFFER_SIZE);
                append_str(cli_info, cli_port, BUFFER_SIZE);

                //write to log file
                char *tmp = format_log("is connected\n", cli_info, SEVERITY_INFO);
                write_log(tmp, strlen(tmp), server);
                printf("%s", tmp);

                /*Create a Driver element : it gives access to the server thread and
                 *to the correct socket descriptor to the thread that will handle the client)
                 */
                Driver *driver = malloc(sizeof(Driver));
                driver->server = server;
                driver->s_dial = s_dial;
                driver->cli_info = cli_info;

                /*Start a detached thread
                *(detached because no other thread will wait for it to complete)
                */
                thread_attributes = malloc(sizeof * thread_attributes);
                r = pthread_attr_init(thread_attributes);
                r = pthread_attr_setdetachstate(thread_attributes, PTHREAD_CREATE_DETACHED);

                r = pthread_create(&d_tid, thread_attributes, client_handler, driver);

                r = pthread_attr_destroy(thread_attributes);//Not sure...
            }
            //might add other cases later(allow server to read std input), which is why I didn't write an && condition
        }
        else { /*If select was unlocked due to timeout*/
            //Decrement drivers count because no driver thread was started
            decrement_drivers(server);

            //Stop waiting for connections if a shutdown was requested
            pthread_mutex_lock(&server->lock);
            if (server->shutdown_requested) {
                stop = 1;
            }
            pthread_mutex_unlock(&server->lock);
        }
    }

    if (close(s_listen) == -1) {
        perror("Couldn't close socket");
    }

    pthread_mutex_lock(&server->lock);
    if (close(server->log_fd) == -1) {
        perror("Couldn't close log file");
    }
    pthread_mutex_unlock(&server->lock);

    PQfinish(conn);
    return (NULL);
}

void run_debug(char *db_url) {
    PGconn *conn = connect_db(db_url);

    insert_test(conn);
    auth_test(conn);

    PQfinish(conn);
}