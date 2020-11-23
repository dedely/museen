#ifndef _SERVER_H_
#define _SERVER_H_
#include <pthread.h>
#include <netinet/in.h>

/**
 * @brief Server data structure to keep track of the active drivers
 * and shutdown requests.
 *
 */
typedef struct {
    int drivers_cnt;
    int port;
    int shutdown_requested;
    in_addr_t ip;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} Server;

void run(char *ip, int port);
void run_debug();

void init_server(in_addr_t ip, int port, Server *s);
void increment_drivers(Server *s);
void decrement_drivers(Server *s);
void *run_server(void *data);


#endif /* _SERVER_H_ */