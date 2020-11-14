#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

/**
 * @brief Create a tcp server object, i.e. a listening socket on the given port with
 *  a maximum number clients_max of queued clients.
 *
 * @param port
 * @param clients_max
 * @return int
 */
int create_tcp_server(int port, int clients_max) {
    int s_listen;
    struct sockaddr_in serv_addr;

    /* define which address/port we're using */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* accept any incoming message */
    serv_addr.sin_port = htons(port);
    memset(&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    /* create a listening  socket and pass it the (address/port)*/
    s_listen = socket(PF_INET, SOCK_STREAM, 0);
    bind(s_listen, (struct sockaddr*)&serv_addr, sizeof serv_addr);
    listen(s_listen, clients_max); /* define s_listen as a listening socket*/

    return s_listen;
}

/**
 * @brief Connects to the database server in a synchronous(blocking) fashion.
 *
 * @return PGconn*
 */
PGconn *connect_db() {
    PGconn *conn = PQconnectdb(DB_INFO);
    if (PQstatus(conn) == CONNECTION_BAD) {
        printf("Connexion to database server failed : %s", PQerrorMessage(conn));
        exit(0);
    }
    else {
        printf("Connected to database server\n");
    }
}