#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h> //for non blocking  options
#include "util.h"
#include "types.h"

/**
 * @brief Create a tcp server object, i.e. a listening socket on the given port with
 *  a maximum number clients_max of queued clients.
 *
 * @param port
 * @param clients_max
 * @return int
 */
int create_tcp_server(in_addr_t ip, int port, int clients_max) {
    int s_listen;
    int so_reuseaddr = 1;
    struct sockaddr_in serv_addr;

    /* define which address/port we're using */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = ip; /*ip is now passed as paramater*/
    serv_addr.sin_port = htons(port);
    memset(&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    /* create a listening  socket and pass it the (address/port)*/
    s_listen = socket(PF_INET, SOCK_STREAM, 0);
    if (s_listen < 0) {
        perror("Couldn't create socket");
        exit(EXIT_FAILURE);
    }

    int r = setsockopt(s_listen, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr,
        sizeof so_reuseaddr);
    if (r < 0) {
        perror("Couldn't set server socket options");
        exit(EXIT_FAILURE);
    }

    r = bind(s_listen, (struct sockaddr *)&serv_addr, sizeof serv_addr);
    if (r < 0) {
        perror("Couldn't bind server socket");
        exit(EXIT_FAILURE);
    }

    /*Make the server socket nonblocking*/
    r = fcntl(s_listen, F_SETFL, O_NONBLOCK);
    if (r < 0) {
        perror("Couldn't make server socket nonblocking");
        exit(EXIT_FAILURE);
    }

    r = listen(s_listen, clients_max); /* define s_listen as a listening socket*/
    if (r < 0) {
        perror("Couldn't listen on server socket");
        exit(EXIT_FAILURE);
    }
    return s_listen;
}


char *get_timestamp() {
    time_t rawtime;
    struct tm *info;
    char *timestamp = (char *)malloc(30 * sizeof(char));
    if (timestamp == NULL) {
        perror("Memory allocation error in function get_timestamp()\n");
        exit(0);
    }
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(timestamp, 30, "%Y-%m-%d %X", info);

    return timestamp;
}

in_addr_t set_ip(char *ip){
    in_addr_t s_ip;
    if(strcmp(ip, IP_DEFAULT) == 0){
        s_ip = htonl(INADDR_ANY); /* accept any incoming message */
    }else{
        s_ip = inet_addr (ip) ;
    }
    return s_ip;
}