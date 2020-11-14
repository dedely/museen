#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "driver.h"

#define BUFFER_SIZE 80

ClientStateType login_handler();
ClientStateType query_handler();
ClientStateType data_handler();
ClientStateType timeout_handler();

/**
 * @brief  The thread that manages the client retrieves from the socket
 * what the client tells.
 *
 * @param data Descriptor of the connection socket allowing to dialog with a specific client.
 *             It's a generic type because we're using threads (see pthread_create documentation)
 * @return void*
 */
void *handle_client(void *data) {
    int stop = 0;
    int s_dial = *(int *) data; /*cast*/
    char buf[BUFFER_SIZE];
    ClientStateType next_state = CLIENT_INIT;
    EventType event;
    bzero(buf, BUFFER_SIZE); /*This initializes the buffer*/

    while (!stop) {
        //Read event
        event = read_event(s_dial, buf, BUFFER_SIZE);

        /*We use a state machine to handle the client.
        * Code related to specific events is delegated to the corresponding function.
        */
        switch (next_state) {
        case CLIENT_INIT:
            if (EVENT_AUTH == event) {
                next_state = login_handler();
            }
            break;
        case CLIENT_IDLE:
            if (EVENT_QUERY == event) {
                next_state = query_handler();
            }
            break;
        case CLIENT_MADE_QUERY:
            if (EVENT_DATA == event) {
                next_state = data_handler();
            }
            break;
        case CLIENT_TIMED_OUT:
            if (EVENT_TIMEOUT_END == event) {
                next_state = timeout_handler();
            }
            break;
        default:
            /*Add to logs later*/
            break;
        }

        //EVENT_QUIT does not depend of the state
        if (EVENT_QUIT == event) {
            stop = 1;
        }
        bzero(buf, 80); /*Don't forget to reset the buffer*/
    }

    free(data);
    close(s_dial);
    return (NULL);
}

/**
 * @brief
 *
 * @param s_dial
 * @param buf
 * @param size
 * @return EventType
 */
EventType read_event(int s_dial, char *buf, int size) {
    int n = read(s_dial, buf, size);
    EventType event = EVENT_QUIT;

    printf("Recieved[%s]\n", buf);
    //Code pour la lecture à compléter.

    return event;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType login_handler() {
    return CLIENT_IDLE;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType query_handler() {
    return CLIENT_MADE_QUERY;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType data_handler() {
    return CLIENT_IDLE;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType timeout_handler() {
    return CLIENT_IDLE;
}