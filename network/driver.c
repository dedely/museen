#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include "driver.h"
#include "server.h"
#include "query.h"
#include "util.h"

void read_event(int *s_dial, char **data, EventType *event, char *cli_info, Server *server);

ClientStateType login_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server);
ClientStateType query_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server);
ClientStateType data_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server);
ClientStateType timeout_handler(int *s_dial);

/**
 * @brief  The thread that manages the client retrieves from the socket
 * what the client tells.
 *
 * @param param
 * @return void*
 */
void *client_handler(void *param) {
    Driver *driver = param;
    char *cli_info = driver->cli_info;
    Server *server = driver->server;
    PGconn *conn = server->database_connection;
    int *s_dial = &driver->s_dial;
    char *data = NULL;
    ClientStateType next_state = CLIENT_INIT;
    EventType event = EVENT_CONNECTED;
    int stop = 0;

    while (!stop) {
        read_event(s_dial, &data, &event, cli_info, server);

        /*We use a state machine to handle the client.
        * Code related to specific events is delegated to the corresponding function.
        */
        switch (next_state) {
        case CLIENT_INIT:
            if (EVENT_DATA == event) {
                next_state = login_handler(data, s_dial, cli_info, conn, server);
            }
            break;
        case CLIENT_IDLE:
            if (EVENT_DATA == event) {
                next_state = query_handler(data, s_dial, cli_info, conn, server);
            }
            break;
        case CLIENT_MADE_QUERY:
            if (EVENT_DATA == event) {
                next_state = data_handler(data, s_dial, cli_info, conn, server);
            }
            break;
        case CLIENT_TIMED_OUT:
            if (EVENT_TIMEOUT_END == event) {
                next_state = timeout_handler(s_dial);
            }
            break;
        default:
            break;
        }
        if (EVENT_BUF_OVERFLOW == event) {
            next_state = CLIENT_INIT;
        }
        //EVENT_QUIT does not depend of the state
        if (EVENT_QUIT == event) {
            stop = 1;
        }
    }

    if (close(*s_dial) == -1) {
        perror("Couldn't close socket");
    }
    free(driver);
    decrement_drivers(server);
    return (NULL);
}

/**
 * @brief Checks for string buffer overflow by calling strnlen(). If no overflow, removes the last character if it's a '\n', or '\r\n'
 *
 * @param buf
 * @return int size of the (new) string of -1 in case of string buffer overflow
 */
int filter(char *buf, int max_length) {
    int length = strnlen(buf, max_length);
    int valid = (length != max_length);
    if (valid && (buf[length - 1] == '\n')) {
        buf[length - 1] = '\0';
        length--;
    }
    if (valid && (buf[length - 1] == '\r')) { /*Some terminals add '\r\n'*/
        buf[length - 1] = '\0';
        length--;
    }
    return valid ? length : -1;
}

/**
 * @brief
 *
 * @param s_dial
 * @param data
 * @param event
 * @param cli_info
 * @param server
 */
void read_event(int *s_dial, char **data, EventType *event, char *cli_info, Server *server) {
    char buf[BUFFER_SIZE];
    bzero(buf, BUFFER_SIZE);
    int n = read(*s_dial, buf, BUFFER_SIZE);
    if (n > 0) {
        //Check for string buffer overflow
        int length = filter(buf, BUFFER_SIZE);
        if (length == -1) {
            *event = EVENT_BUF_OVERFLOW;
        }
        else {
            //Prepare data string
            free(*data);
            *data = malloc_str(length);
            strncpy(*data, buf, length + 1);
            *event = EVENT_DATA;

            //Logs
            char tmp[150];
            sprintf(tmp, "Recieved:[%s]\n", *data);
            char *log = format_log(tmp, cli_info, SEVERITY_INFO);
            write_log(log, strlen(log), server);
            printf("%s", log);

            if (DEBUG) {
                printf("Sending back...\n");
                if (write(*s_dial, *data, n) == -1) {
                    perror("Couldn't write on file descriptor");
                }
            }
        }
    }
    else {
        *event = EVENT_QUIT;
    }
}

/**
 * @brief This function queries the database server to check if the login key provided by the client is valid.
 *
 * @param data
 * @param s_dial
 * @param ip
 * @param conn
 * @return ClientStateType
 */
ClientStateType login_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server) {
    ClientStateType next_state = CLIENT_INIT;
    int n;
    int status = CONN_FAILED_NOT_PREM;
    int length = ID_SIZE + 5;
    char id[ID_SIZE];
    char reply[length];
    bzero(id, ID_SIZE);
    bzero(reply, length);
    strncat(id, query_login(conn, data), ID_SIZE);
    if (strlen(id) > 0) {
        next_state = CLIENT_IDLE;
        status = CONN_AUTH_OK;
        sprintf(reply, "%d", status);
        strncat(reply, SEPARATOR, 2);
        strncat(reply, id, ID_SIZE);

        //Logs
        char *log = format_log("is logged in\n", cli_info, SEVERITY_INFO);
        write_log(log, strlen(log), server);
        printf("%s", log);
    }
    else {
        status = CONN_FAILED_UKN;
        sprintf(reply, "%d", status);

        char *log = format_log("was not logged in\n", cli_info, SEVERITY_INFO);
        write_log(log, strlen(log), server);
        printf("%s", log);
    }
    //Java client uses readline() so we add '\n' at the end of our message.
    strcat(reply, "\n");

    n = strlen(reply) + 1; //+1 for the '\0' character
    if (write(*s_dial, reply, n) == -1) {
        perror("Couldn't write on file descriptor");
    }
    return next_state;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType query_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server) {
    return CLIENT_MADE_QUERY;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType data_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server) {
    return CLIENT_IDLE;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType timeout_handler(int *s_dial) {
    return CLIENT_IDLE;
}