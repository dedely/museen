#ifndef _DRIVER_H_
#define _DRIVER_H_
#include "types.h"
#include "server.h"
#define BUFFER_SIZE 80
#define DEBUG 0

/**
 * @brief Data used by the client_handler
 *
 */
typedef struct {
  Server *server;
  int s_dial;
  char *cli_info;
} Driver;

/**
 * @brief  The thread that manages the client retrieves from the socket
 * what the client tells.
 *
 * @param dial
 * @return void*
 */
void *client_handler(void *data);

#endif /* _DRIVER_H_ */