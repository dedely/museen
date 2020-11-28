#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include "driver.h"
#include "query.h"
#include "util.h"

ClientStateType login_handler(char *data, int *s_dial, char *ip, PGconn *conn);
ClientStateType query_handler(char *data, int *s_dial, char *ip, PGconn *conn);
ClientStateType data_handler(char *data, int *s_dial, char *ip, PGconn *conn);
ClientStateType timeout_handler(int *s_dial);

/**
 * @brief  The thread that manages the client retrieves from the socket
 * what the client tells.
 *
 * @param param
 * @return void*
 */
void *client_handler(void *param) {
    Driver *driver = param;
    char *ip = driver->ip;
    Server *server = driver->server;
    int *s_dial = &driver->s_dial;
    char *data = NULL;
    ClientStateType next_state = CLIENT_INIT;
    EventType event = EVENT_CONNECTED;
    PGconn *conn = connect_db();
    int stop = 0;

    while (!stop) {
        read_event(s_dial, &data, &event, ip);

        /*We use a state machine to handle the client.
        * Code related to specific events is delegated to the corresponding function.
        */
        switch (next_state) {
        case CLIENT_INIT:
            if (EVENT_DATA == event) {
                next_state = login_handler(data, s_dial, ip, conn);
            }
            break;
        case CLIENT_IDLE:
            if (EVENT_DATA == event) {
                next_state = query_handler(data, s_dial, ip, conn);
            }
            break;
        case CLIENT_MADE_QUERY:
            if (EVENT_DATA == event) {
                next_state = data_handler(data, s_dial, ip, conn);
            }
            break;
        case CLIENT_TIMED_OUT:
            if (EVENT_TIMEOUT_END == event) {
                next_state = timeout_handler(s_dial);
            }
            break;
        default:
            break;
        }
        if (EVENT_BUF_OVERFLOW == event) {
            next_state = CLIENT_INIT;
        }
        //EVENT_QUIT does not depend of the state
        if (EVENT_QUIT == event) {
            stop = 1;
        }
    }

    close(*s_dial);
    free(driver);
    decrement_drivers(server);
    return (NULL);
}

/**
 * @brief Checks for string buffer overflow by calling strnlen(). If no overflow, removes the last character if it's a '\n'.
 *
 * @param buf
 * @return int size of the (new) string of -1 in case of string buffer overflow
 */
int filter(char *buf, int max_length) {
    int length = strnlen(buf, max_length);
    int valid = (length != max_length);
    if (valid && (buf[length - 1] == '\n')) {
        buf[length - 1] = '\0';
        length--;
    }
    return valid ? length : -1;
}

/**
 * @brief
 *
 * @param s_dial
 * @param buf
 * @param size
 * @return EventType
 */
void read_event(int *s_dial, char **data, EventType *event, char *ip) {
    char buf[BUFFER_SIZE];
    bzero(buf, BUFFER_SIZE);

    int n = read(*s_dial, buf, BUFFER_SIZE);
    if (n > 0) {
        //Check for string buffer overflow
        int length = filter(buf, BUFFER_SIZE);
        if (length != -1) {
            //Prepare data string
            free(*data);
            *data = (char *)malloc(length * sizeof(char));
            if (*data == NULL) {
                perror("Couldn't allocate memory in function read_event.");
                exit(EXIT_FAILURE);
            }
            strncpy(*data, buf, length);
            *event = EVENT_DATA;
            //Add it to logs later
            printf("Recieved:[%s] from %s (client %d)\n", *data, ip, *s_dial);

            if (DEBUG) {
                printf("Sending back...\n");
                write(*s_dial, *data, n);
            }
        }
        else {
            *event = EVENT_BUF_OVERFLOW;
        }
    }
    else {
        *event = EVENT_QUIT;
    }
}

int query_login(char *data) {
    int result = 0;
    if (strcmp(data, DEBUG_KEY) == 0) {
        result = 1;
    }
    return result;
}

/**
 * @brief This function queries the database server to check if the login key provided by the client is valid.
 *
 * @return ClientStateType
 */
ClientStateType login_handler(char *data, int *s_dial, char *ip, PGconn *conn) {
    ClientStateType next_state = CLIENT_INIT;
    int n;
    int status = CONN_FAILED_NOT_PREM;
    char reply[5];
    bzero(reply, 5);

    if (query_login(data) == 1) {
        next_state = CLIENT_IDLE;
        status = CONN_AUTH_OK;
        printf("%s (client %d) is logged in\n", ip, *s_dial);
    }
    else {
        status = CONN_FAILED_UKN;
        printf("%s (client %d) was not logged in\n", ip, *s_dial);
    }

    sprintf(reply, "%d", status);
    n = strnlen(reply, 5) + 1; //+1 for the '\0' character
    write(*s_dial, reply, n);
    return next_state;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType query_handler(char *data, int *s_dial, char *ip, PGconn *conn) {
    return CLIENT_MADE_QUERY;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType data_handler(char *data, int *s_dial, char *ip, PGconn *conn) {
    return CLIENT_IDLE;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType timeout_handler(int *s_dial) {
    return CLIENT_IDLE;
}