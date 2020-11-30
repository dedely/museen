#ifndef _UTIL_H_
#define _UTIL_H_

#include <arpa/inet.h> //for inet_addr()
#include "types.h"

int create_tcp_server(in_addr_t ip, int port, int clients_max);
char *get_timestamp();
in_addr_t set_ip(char *ip);
char *malloc_str(int length);
char *format_log(char *message, char *ip, LogSeverityType severity);
void append_str(char *dest, char *str, int max_size);
char *format_cli_info(struct sockaddr_in cli_addr);
int is_timestamp(char *str);
int is_visitor_id(char *str);
int is_auth_key(char *str);

#endif /* _UTIL_H_ */