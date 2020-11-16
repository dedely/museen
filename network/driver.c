#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "driver.h"

#define BUFFER_SIZE 80

ClientStateType login_handler(char *data);
ClientStateType query_handler(char *data);
ClientStateType data_handler(char *data);
ClientStateType timeout_handler();

/**
 * @brief  The thread that manages the client retrieves from the socket
 * what the client tells.
 *
 * @param data Descriptor of the connection socket allowing to dialog with a specific client.
 *             It's a generic type because we're using threads (see pthread_create documentation)
 * @return void*
 */
void *handle_client(void *dial) {
    int stop = 0;
    int s_dial = *(int *)dial;
    char *data = NULL;
    ClientStateType next_state = CLIENT_INIT;
    EventType event;

    while (!stop) {
        //Read event
        event = read_event(s_dial, &data);

        /*We use a state machine to handle the client.
        * Code related to specific events is delegated to the corresponding function.
        */
        switch (next_state) {
        case CLIENT_INIT:
            if (EVENT_AUTH == event) {
                next_state = login_handler(data);
            }
            break;
        case CLIENT_IDLE:
            if (EVENT_QUERY == event) {
                next_state = query_handler(data);
            }
            break;
        case CLIENT_MADE_QUERY:
            if (EVENT_DATA == event) {
                next_state = data_handler(data);
            }
            break;
        case CLIENT_TIMED_OUT:
            if (EVENT_TIMEOUT_END == event) {
                next_state = timeout_handler(data);
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
    }

    free(dial);
    close(s_dial);
    return (NULL);
}

/**
 * @brief  This filter is used to remove the last character in the buffer if it's a newline character.
 *  P.S. : not my cleanest function...
 * @param buf
 * @param size
 */
char *filter(char *buf) {
    int len = strlen(buf);
    if ((len > 0) && (buf[len - 1] == '\n')) {
        buf[len - 1] = '\0';
    }
    return buf;
}

/**
 * @brief
 *
 * @param s_dial
 * @param buf
 * @param size
 * @return EventType
 */
EventType read_event(int s_dial, char **data) {
    char buf[BUFFER_SIZE];
    bzero(buf, BUFFER_SIZE); /*This initializes the buffer*/

    int n = read(s_dial, buf, BUFFER_SIZE);
    EventType event;

    if (n != 0) {
        event = EVENT_CONNECTED;

        strcpy(buf, filter(buf));
        *data = (char *)realloc(*data, (strlen(buf) + 1) * sizeof(char));
        strcpy(*data, buf);
        printf("Recieved:[%s]\n", *data);

        printf("Sending back...\n");
        write(s_dial, *data, n);
        //Code pour la lecture à compléter.
    }
    else {
        event = EVENT_QUIT;
    }
    bzero(buf, BUFFER_SIZE);
    return event;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType login_handler(char *data) {
    return CLIENT_IDLE;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType query_handler(char *data) {
    return CLIENT_MADE_QUERY;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType data_handler(char *data) {
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