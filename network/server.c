#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include "util.h"
#include "driver.h"

void run() {
    int s_listen, r;
    int stop = 0;

    s_listen = create_tcp_server(SERVER_PORT, 5);

    while (stop != 1) {
        pthread_attr_t *thread_attributes;
        pthread_t tid;
        int *s_dial;

        s_dial = malloc(sizeof * s_dial);

        /* We listen on the socket created above (create_tpc_server).
        * accept is a blocking function that is only unblocked when a client connects
        * to the listening socket. The return value is then the descriptor of
        * the connection socket.
        */
        *s_dial = accept(s_listen, NULL, 0);

        thread_attributes = malloc(sizeof * thread_attributes);
        r = pthread_attr_init(thread_attributes);
        r = pthread_attr_setdetachstate(thread_attributes, PTHREAD_CREATE_DETACHED);

        r = pthread_create(&tid, thread_attributes, handle_client, s_dial);

        r = pthread_attr_destroy(thread_attributes);
    }

    close(s_listen);
    exit(EXIT_SUCCESS);
}

void run_debug() {
    PGconn *conn = connect_db();

    //Sample insert query test: 
    printf("Running INSERT INTO query test...\n");
    char query[300] = "INSERT INTO \"public\".\"connection\" (name, time) VALUES(\'dedely\', \'";
    char* timestamp = get_timestamp();
    printf("%s\n", timestamp);
    strcat(query, timestamp);
    strcat(query, "\');");
    PGresult *result;
    result = PQexec(conn, query);
    ExecStatusType resultStatus;
    resultStatus = PQresultStatus(result);

    printf("%s\n", PQresStatus(resultStatus));
    printf("%s\n", PQresultErrorMessage(result));

    PQfinish(conn);
}