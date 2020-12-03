#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <locale.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h> //for non blocking  options
#include "util.h"

#define INFO_SIZE 30

/**
 * @brief Create a tcp server object, i.e. a listening socket on the given port with
 *  a maximum number clients_max of queued clients.
 *
 * @param port
 * @param clients_max
 * @return int
 */
int create_tcp_server(in_addr_t ip, int port, int clients_max) {
    int s_listen;
    int so_reuseaddr = 1;
    struct sockaddr_in serv_addr;

    /* define which address/port we're using */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = ip; /*ip is now passed as paramater*/
    serv_addr.sin_port = htons(port);
    memset(&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    /* create a listening  socket and pass it the (address/port)*/
    s_listen = socket(PF_INET, SOCK_STREAM, 0);
    if (s_listen < 0) {
        perror("Couldn't create socket");
        exit(EXIT_FAILURE);
    }

    int r = setsockopt(s_listen, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr,
        sizeof so_reuseaddr);
    if (r < 0) {
        perror("Couldn't set server socket options");
        exit(EXIT_FAILURE);
    }

    r = bind(s_listen, (struct sockaddr *)&serv_addr, sizeof serv_addr);
    if (r < 0) {
        perror("Couldn't bind server socket");
        exit(EXIT_FAILURE);
    }

    /*Make the server socket nonblocking*/
    r = fcntl(s_listen, F_SETFL, O_NONBLOCK);
    if (r < 0) {
        perror("Couldn't make server socket nonblocking");
        exit(EXIT_FAILURE);
    }

    r = listen(s_listen, clients_max); /* define s_listen as a listening socket*/
    if (r < 0) {
        perror("Couldn't listen on server socket");
        exit(EXIT_FAILURE);
    }
    return s_listen;
}

/**
 * @brief Get the current timestamp
 *
 * @return char*
 */
char *get_timestamp() {
    time_t rawtime;
    struct tm *info;
    char *timestamp = (char *)malloc(30 * sizeof(char));
    if (timestamp == NULL) {
        perror("Couldn't allocate memory in function get_timestamp()");
        exit(0);
    }
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(timestamp, 30, "%Y-%m-%d %X", info);

    return timestamp;
}

/**
 * @brief Default setting : listen on all available ips
 *
 * @param ip
 * @return in_addr_t
 */
in_addr_t set_ip(char *ip) {
    in_addr_t s_ip;
    if (strcmp(ip, IP_DEFAULT) == 0) {
        s_ip = htonl(INADDR_ANY); /* accept any incoming message */
    }
    else {
        int r = inet_pton(AF_INET, ip, &s_ip);
        if (r != 1) {
            s_ip = htonl(INADDR_ANY);
            printf("Incorrect ip format [%s], using default settings instead...\n", ip);
        }
        else {
            printf("Provided ip is in the correct format.\n");
        }
    }
    return s_ip;
}

/**
 * @brief String memory allocation
 *
 * @param length
 * @return char*
 */
char *malloc_str(int length) {
    char *str = (char *)malloc(length + 1 * sizeof(char));
    if (str == NULL) {
        perror("Couldn't allocate memory");
        exit(EXIT_FAILURE);
    }
    return str;
}

/**
 * @brief Format log string given the severity(combined string length must be under 150 char);
 *
 * @param message
 * @param cli_info Expecting [ip:port] but any other format is valid as long as it allows to recognize the client.
 * @param severity
 * @return char*
 */
char *format_log(char *message, char *cli_info, LogSeverityType severity) {
    char *buf = malloc_str(150);
    snprintf(buf, 150, "[%s] %s %s %s", severity_types[severity], get_timestamp(), cli_info, message);
    return  buf;
}

/**
 * @brief Appends str to dest
 *
 * @param dest
 * @param str
 * @param max_size max size of dest ('\0' included)
 */
void append_str(char *dest, char *str, int max_size) {
    strncat(dest, str, max_size - (strlen(dest) + 1));
}

/**
 * @brief Formats client address as a string ip:port
 *
 * @param cli_addr
 * @return char*
 */
char *format_cli_info(struct sockaddr_in cli_addr) {
    char *cli_info = malloc_str(INFO_SIZE);
    char cli_port[10];
    bzero(cli_port, 10);
    char *cli_ip = inet_ntoa(cli_addr.sin_addr);
    sprintf(cli_port, ":%d", ntohs(cli_addr.sin_port));
    append_str(cli_info, cli_ip, INFO_SIZE);
    append_str(cli_info, cli_port, INFO_SIZE);
    return cli_info;
}

/**
 * @brief Checks if a string matches the timestamp format "%Y-%m-%d %X"
 * Note: I couldn't find any C function that does this...
 *
 * @param str
 * @return int 0 for false
 */
int is_timestamp(char *str) {
    int is_timestamp = 0;
    int year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;
    time_t time = 0;
    struct tm result = { 0 };
    //First parse the string
    if (sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d", &year, &month, &day, &hour, &min, &sec) == 6) {
        //Try to convert into time
        result.tm_year = year - 1900;
        result.tm_mon = month - 1;
        result.tm_mday = day;
        result.tm_hour = hour;
        result.tm_min = min;
        //mktime returns -1 for errors
        if ((time = mktime(&result)) != (time_t)-1) {
            is_timestamp = 1;
        }
    }

    return is_timestamp;
}

/**
 * @brief Only characters allowed are [A-Z], [a-z], [0-9] and '_'
 *
 * @param c
 * @return int 0 for false
 */
int is_allowed_char(char c) {
    return ((c >= 48) && (c <= 57)) || ((c >= 65) && (c <= 90)) || ((c >= 97) && (c <= 122)) || (c == 95);
}

/**
 * @brief visitor_id (s) must be between 4-20 characters long and the only characters allowed are [A-Z], [a-z], [0-9] and '_'
 *
 * @param str
 * @return int 0 for false
 */
int is_visitor_id(char *str) {
    int is_visitor_id = 0;
    int length = strlen(str);
    if ((length >= 4) && (length <= 20)) {
        int i = 0, stop = 0;
        while ((i < length) && !stop) {
            if (!is_allowed_char(str[i])) {
                stop = 1;
            }
            i++;
        }
        if (i == length) {
            is_visitor_id = 1;
        }
    }
    return is_visitor_id;
}

int is_location(char *str) {
    int is_location = 0;
    int loc;
    if ((loc = strtol(str, NULL, 10) != 0) && (errno != ERANGE)) {
        is_location = 1;
    }
    return is_location;
}

/**
 * @brief Checks is the string matches the sha256 result format: i.e. 64 characters long and no special characters.
 *
 * @param str
 * @return int 0 for false
 */
int is_auth_key(char *str) {
    int is_auth_key = 0;
    int length = strlen(str);
    if (length == 64) {
        int i = 0, stop = 0;
        while ((i < length) && !stop) {
            //Reuse the same fucntion
            if ((!is_allowed_char(str[i])) || (str[i] == 95)) {
                stop = 1;
            }
            i++;
        }
        if (i == length) {
            is_auth_key = 1;
        }
    }
    return is_auth_key;
}

/**
 * @brief This utility function can be used to parse data separated by a using string delimiter.
 * 
 * @param data 
 * @param fields_count Number of fields expected
 * @param separator 
 * @return char** NULL if the data is not in the correct format
 */
char **parse_data(char *data, int fields_count, char *separator) {
    int correct_format = 0, cpt = 0;
    char **fields = (char **)malloc(fields_count * sizeof(char *));
    if (fields == NULL) {
        perror("Couldn't allocate memory");
        exit(EXIT_FAILURE);
    }
    char *field = strtok(data, separator);
    while (field != NULL && cpt < fields_count) {
        fields[cpt] = field;
        cpt++;
        field = strtok(NULL, separator);
    }
    //Check data
    if ((field == NULL) && (cpt == fields_count)) {
        correct_format = 1;
    }
    return (correct_format)? fields : NULL;
}