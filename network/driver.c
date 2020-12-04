#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include "driver.h"
#include "server.h"
#include "query.h"
#include "util.h"

void read_event(int *s_dial, char **data, EventType *event, char *cli_info, Server *server);

ClientStateType login_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server);
ClientStateType info_query_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server);
ClientStateType sugg_query_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server);
ClientStateType data_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server);
ClientStateType timeout_handler(int *s_dial);
EventType check_event(char *code);

/**
 * @brief  The thread that manages the client retrieves from the socket
 * what the client tells.
 *
 * @param param Driver*
 * @return void*
 */
void *client_handler(void *param) {
    Driver *driver = param;
    char *cli_info = driver->cli_info;
    Server *server = driver->server;
    PGconn *conn = server->database_connection;
    int *s_dial = &driver->s_dial;
    char *data = NULL;
    ClientStateType next_state = CLIENT_INIT;
    EventType event = EVENT_CONNECTED;
    int stop = 0;
    int handled_event;
    char reply[CODE_SIZE];
    int res;

    while (!stop) {
        handled_event = 0;
        bzero(reply, 5);
        res = 0;
        read_event(s_dial, &data, &event, cli_info, server);

        //Events that don't depend of the state.
        if ((EVENT_BUF_OVERFLOW == event) || (EVENT_UKN == event)) {
            next_state = CLIENT_INIT;
            res = REPLY_CLIENT_NOT_AUTH;
            if (snprintf(reply, 5, "%d\n", res) > 0) {
                if (write(*s_dial, reply, strlen(reply) + 1) == -1) {
                    perror("Couldn't write on file descriptor");
                }
            }
            handled_event = 1;
        }
        else if ((EVENT_EXIT == event) || (EVENT_DISCONNECT == event)) {
            stop = 1;
            handled_event = 1;
        }

        /*We use a state machine to handle the client.
        * Code related to specific events is delegated to the corresponding function.
        */
        if (!handled_event) {
            switch (next_state) {
            case CLIENT_INIT:
                if (EVENT_AUTH == event) {
                    next_state = login_handler(data, s_dial, cli_info, conn, server);
                }
                else {
                    res = REPLY_CLIENT_NOT_AUTH;
                    if (snprintf(reply, 5, "%d\n", res) > 0) {
                        if (write(*s_dial, reply, strlen(reply) + 1) == -1) {
                            perror("Couldn't write on file descriptor");
                        }
                    }
                }
                break;
            case CLIENT_IDLE:
                if (EVENT_INFO == event) {
                    next_state = info_query_handler(data, s_dial, cli_info, conn, server);
                }
                else if (EVENT_DATA == event) {
                    next_state = data_handler(data, s_dial, cli_info, conn, server);
                }
                else if (EVENT_SUGG == event) {
                    next_state = sugg_query_handler(data, s_dial, cli_info, conn, server);
                }
                else {
                    res = REPLY_CLIENT_ALREADY_AUTH;
                    if (snprintf(reply, 5, "%d\n", res) > 0) {
                        if (write(*s_dial, reply, strlen(reply) + 1) == -1) {
                            perror("Couldn't write on file descriptor");
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
    }

    if (close(*s_dial) == -1) {
        perror("Couldn't close socket");
    }
    free(driver);
    decrement_drivers(server);
    return (NULL);
}

/**
 * @brief Checks for string buffer overflow by calling strnlen(). If no overflow, removes the last character if it's a '\n', or '\r\n'
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
    if (valid && (buf[length - 1] == '\r')) { /*Some terminals add '\r\n'*/
        buf[length - 1] = '\0';
        length--;
    }
    return valid ? length : -1;
}

EventType check_event(char *code) {
    EventType event = EVENT_UKN;
    if (strncmp(code, query_code_types[CODE_EXIT], QUERY_CODE_LENGTH) == 0) {
        event = EVENT_EXIT;
    }
    else if (strncmp(code, query_code_types[CODE_LOGN], QUERY_CODE_LENGTH) == 0) {
        event = EVENT_AUTH;
    }
    else if (strncmp(code, query_code_types[CODE_LOCA], QUERY_CODE_LENGTH) == 0) {
        event = EVENT_DATA;
    }
    else if (strncmp(code, query_code_types[CODE_SUGG], QUERY_CODE_LENGTH) == 0) {
        event = EVENT_SUGG;
    }
    else if (strncmp(code, query_code_types[CODE_INFO], QUERY_CODE_LENGTH) == 0) {
        event = EVENT_INFO;
    }
    return event;
}

/**
 * @brief Read socket content and detect event
 *
 * @param s_dial
 * @param data
 * @param event
 * @param cli_info used for logs
 * @param server for synchronizing log write operations
 */
void read_event(int *s_dial, char **data, EventType *event, char *cli_info, Server *server) {
    char buf[BUFFER_SIZE];
    bzero(buf, BUFFER_SIZE);
    char *log;
    int n = read(*s_dial, buf, BUFFER_SIZE);
    printf("[%s]\n", buf);
    if (n == -1) {
        *event = EVENT_DISCONNECT;
        int errnum = errno;
        log = format_log(strerror(errnum), cli_info, SEVERITY_CRITICAL);
        write_log(log, strlen(log), server);
        printf("%s", log);
    }
    else if (n == 0) {
        *event = EVENT_DISCONNECT;
        log = format_log("forced disconnect\n", cli_info, SEVERITY_WARNING);
        write_log(log, strlen(log), server);
        printf("%s", log);
    }
    else if (n == BUFFER_SIZE) {
        *event = EVENT_BUF_OVERFLOW;
        log = format_log("message was ignored : buffer overflow\n", cli_info, SEVERITY_ERROR);
        write_log(log, strlen(log), server);
        printf("%s", log);
    }
    else {
        int length = filter(buf, BUFFER_SIZE);
        printf("newbuf = [%s]\n", buf);
        printf("length = %d\n", length);
        printf("strlen(buf) = %ld\n", strlen(buf));
        ServerReplyType reply = REPLY_FORMAT_ERROR;
        //Prepare data string
        *data = malloc_str(strlen(buf));
        strcpy(*data, buf);
        printf("data = %s\n", *data);
        *event = EVENT_UKN;
        if (length >= QUERY_CODE_LENGTH) {
            char *code = malloc_str(QUERY_CODE_LENGTH);
            strncpy(code, *data, QUERY_CODE_LENGTH);
            *event = check_event(code);
        }

        if (*event == EVENT_EXIT) {
            char reply[CODE_SIZE];
            if (snprintf(reply, 5, "%d\n", REPLY_BYE) > 0) {
                if (write(*s_dial, reply, strlen(reply) + 1) == -1) {
                    perror("Couldn't write on file descriptor");
                }
            }
        }

        /*---------Logs-----------*/
        int severity = (*event == EVENT_UKN) ? SEVERITY_WARNING : SEVERITY_INFO;
        char tmp[150];

        //Event
        bzero(tmp, 150);
        sprintf(tmp, "event = [%s]\n", event_types[*event]);
        log = format_log(tmp, cli_info, severity);
        write_log(log, strlen(log), server);
        printf("%s", log);

        //Message
        char msg[120];
        bzero(msg, 120);
        sprintf(msg, "Recieved:[%s]\n", *data);
        log = format_log(msg, cli_info, severity);
        write_log(log, strlen(log), server);
        printf("%s", log);

        if (DEBUG) {
            printf("Sending back...\n");
            if (write(*s_dial, *data, n) == -1) {
                perror("Couldn't write on file descriptor");
            }
        }
    }
}

/**
 * @brief This function queries the database server to check if the login key provided by the client is valid.
 *
 * @param data
 * @param s_dial
 * @param ip
 * @param conn
 * @return ClientStateType
 */
ClientStateType login_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server) {
    ClientStateType next_state = CLIENT_INIT;
    int n, recievedkey = 0;
    int status = CONN_FAILED_UKN;
    int length = ID_SIZE + 5;
    char id[ID_SIZE];
    char reply[length];
    char key[BUFFER_SIZE];
    char *log;
    bzero(id, ID_SIZE);
    bzero(reply, length);
    bzero(key, BUFFER_SIZE);
    char *field = strtok(data, SEPARATOR);
    if (field != NULL) {
        field = strtok(NULL, SEPARATOR);
        if (field != NULL) {
            recievedkey = 1;
            strcpy(key, field);
        }
    }
    if (!recievedkey) {
        if (snprintf(reply, 5, "%d\n", REPLY_FORMAT_ERROR) > 0) {
            if (write(*s_dial, reply, strlen(reply) + 1) == -1) {
                perror("Couldn't write on file descriptor");
            }
        }
        log = format_log("no key\n", cli_info, SEVERITY_ERROR);
        write_log(log, strlen(log), server);
        printf("%s", log);
    }
    else if (is_auth_key(key) != 0) {
        strncat(id, query_login(conn, key), ID_SIZE);
        if (strlen(id) > 0) {
            next_state = CLIENT_IDLE;
            status = CONN_AUTH_OK;
            sprintf(reply, "%d", status);
            strncat(reply, SEPARATOR, 2);
            strncat(reply, id, ID_SIZE);

            //Logs
            log = format_log("is logged in\n", cli_info, SEVERITY_INFO);
            write_log(log, strlen(log), server);
            printf("%s", log);
        }
        else {
            status = CONN_FAILED_UKN;
            sprintf(reply, "%d", status);

            char *log = format_log("was not logged in\n", cli_info, SEVERITY_INFO);
            write_log(log, strlen(log), server);
            printf("%s", log);
        }
        //Java client uses readline() so we add '\n' at the end of our message.
        strcat(reply, "\n");

        n = strlen(reply) + 1; //+1 for the '\0' character
        if (write(*s_dial, reply, n) == -1) {
            perror("Couldn't write on file descriptor");
        }
    }
    else {
        log = format_log("not an auth key\n", cli_info, SEVERITY_WARNING);
        write_log(log, strlen(log), server);
        printf("%s", log);
    }
    return next_state;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType info_query_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server) {
    int res = REPLY_FORMAT_ERROR;
    char *reply, *result, *log;
    char **fields;
    //Parse data
    if ((fields = parse_data(data, 2, SEPARATOR)) != NULL) {
        char *loc = fields[1];
        if (is_location(loc) == 1) {
            result = query_info(conn, loc);
            printf("result = %s\n", result);
            if (result != NULL) {
                res = RES_COMMAND_OK;
            }
        }
    }
    if (res == RES_COMMAND_OK) {
        printf("result = %ld\n", strlen(result));
        int length = strlen(result) + CODE_SIZE;
        printf("length= %d\n", length);
        reply = malloc_str(length);
        //Send back result
        if (snprintf(reply, 5, "%d;", res) > 0) {
            strcat(reply, result);
            strcat(reply, "\n\0");
            if (write(*s_dial, reply, strlen(reply) + 1) == -1) {
                perror("Couldn't write on file descriptor");
            }
            else {
                log = format_log("sent artwork informations to client\n", cli_info, SEVERITY_INFO);
                write_log(log, strlen(log), server);
                printf("%s", log);
            }
        }
    }
    else {
        reply = malloc_str(CODE_SIZE);
        if (snprintf(reply, 5, "%d\n", res) > 0) {
            if (write(*s_dial, reply, strlen(reply) + 1) == -1) {
                perror("Couldn't write on file descriptor");
            }
            else {
                log = format_log("incorrect data\n", cli_info, SEVERITY_WARNING);
                write_log(log, strlen(log), server);
                printf("%s", log);
            }
        }
    }
    return CLIENT_IDLE;
}

ClientStateType sugg_query_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server) {
    int res = REPLY_FORMAT_OK;
    char reply[CODE_SIZE];

    //Send back result
    if (snprintf(reply, 5, "%d\n", res) > 0) {
        if (write(*s_dial, reply, strlen(reply) + 1) == -1) {
            perror("Couldn't write on file descriptor");
        }
    }

    return CLIENT_IDLE;
}

/**
 * @brief Tries to store positionning data in the database.
 *
 * @param data
 * @param s_dial
 * @param cli_info
 * @param conn
 * @param server
 * @return ClientStateType
 */
ClientStateType data_handler(char *data, int *s_dial, char *cli_info, PGconn *conn, Server *server) {
    ClientStateType next_state = CLIENT_IDLE;
    int recieved_data = 0, cpt = 0, stored = 0, res;
    char *log;
    char reply[CODE_SIZE];
    //Parse data
    char **fields = (char **)malloc(5 * sizeof(char *));
    if (fields == NULL) {
        perror("Couldn't allocate memory");
        exit(EXIT_FAILURE);
    }
    char *field = strtok(data, SEPARATOR);
    while (field != NULL && cpt < 5) {
        fields[cpt] = field;
        cpt++;
        field = strtok(NULL, SEPARATOR);
    }
    //Check data
    if ((field == NULL) && (cpt == 5)) {
        int is_id = is_visitor_id(fields[1]);
        int is_loc = is_location(fields[2]);
        int is_time_in = is_timestamp(fields[3]);
        int is_time_out = is_timestamp(fields[4]);
        //Query
        if (is_id && is_loc && is_time_in && is_time_out) {
            recieved_data = 1;
            stored = store_position(conn, fields[1], fields[2], fields[3], fields[4]);
        }
    }
    //Results and logs
    if (stored == 1) {
        res = RES_COMMAND_OK;
        log = format_log("stored position\n", cli_info, SEVERITY_INFO);
        write_log(log, strlen(log), server);
        printf("%s", log);
    }
    else if (recieved_data == 1) {
        res = RES_UNKNOWN_POS;
        log = format_log("did not store position\n", cli_info, SEVERITY_WARNING);
        write_log(log, strlen(log), server);
        printf("%s", log);
    }
    else {
        res = RES_UNKNOWN_FORMAT;
        log = format_log("unknown data format\n", cli_info, SEVERITY_ERROR);
        write_log(log, strlen(log), server);
        printf("%s", log);
    }
    //Send back result
    if (snprintf(reply, 5, "%d\n", res) > 0) {
        if (write(*s_dial, reply, strlen(reply) + 1) == -1) {
            perror("Couldn't write on file descriptor");
        }
    }

    return next_state;
}

/**
 * @brief
 *
 * @return ClientStateType
 */
ClientStateType timeout_handler(int *s_dial) {
    return CLIENT_IDLE;
}

Driver *create_driver(Server *server, int s_dial, char *cli_info) {
    Driver *driver = (Driver *)malloc(sizeof(Driver));
    if (driver == NULL) {
        perror("Couldn't allocate driver memory");
        exit(EXIT_FAILURE);
    }
    driver->server = server;
    driver->s_dial = s_dial;
    driver->cli_info = cli_info;
    return driver;
}