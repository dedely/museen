#ifndef _DRIVER_H_
#define _DRIVER_H_
#include "types.h"

/**
 * @brief  The thread that manages the client retrieves from the socket
 * what the client tells.
 *
 * @param dial
 * @return void*
 */
void *handle_client(void *dial);

EventType read_event(int s_dial, char **data);

#endif /* _DRIVER_H_ */