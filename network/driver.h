#ifndef _DRIVER_H_
#define _DRIVER_H_
#include "types.h"
#include "server.h"

 /**
  * @brief Data used by the client_handler
  * 
  */
typedef struct{
    Server *server;
    int s_dial;
} Driver;

/**
 * @brief  The thread that manages the client retrieves from the socket
 * what the client tells.
 *
 * @param dial
 * @return void*
 */
void *client_handler(void *data);

void read_event(int s_dial, char **data, EventType *event);

#endif /* _DRIVER_H_ */