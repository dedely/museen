#ifndef _SERVER_H_
#define _SERVER_H_
#include <pthread.h>
#include <netinet/in.h>
#include </usr/include/postgresql/libpq-fe.h>


/**
 * @brief Server data structure to keep track of the active drivers
 * and shutdown requests.
 *
 */
typedef struct {
    int drivers_cnt;
    int port;
    int shutdown_requested;
    int log_fd;
    in_addr_t ip;
    PGconn *database_connection;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} Server;

void run(char *ip, int port, char *log_file, char *db_url);
void run_debug(char *db_url);

void init_server(in_addr_t ip, int port, char *log_file, char *db_url, Server *s);
void increment_drivers(Server *s);
void decrement_drivers(Server *s);
void *run_server(void *data);

void write_log(char *message, int  length, Server *s);

#endif /* _SERVER_H_ */