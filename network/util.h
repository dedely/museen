#ifndef _UTIL_H_
#define _UTIL_H_

#include <arpa/inet.h> //for inet_addr()

int create_tcp_server(in_addr_t ip, int port, int clients_max);
char *get_timestamp();
in_addr_t set_ip(char *ip);
char *malloc_str(int length);
char *log_info(char *message, char *ip);
void append_str(char *dest, char *str, int max_size);
#endif /* _UTIL_H_ */