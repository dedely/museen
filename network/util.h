#ifndef _UTIL_H_
#define _UTIL_H_

#include </usr/include/postgresql/libpq-fe.h>

#define LOCAL_DB_INFO "host = localhost port = 5432 dbname = museen user = museendba password = museen"
#define ONLINE_DB_INFO "postgres://vpyfxlkr:Sjq64v_di28D9xpqzF4Y_XIiVkv6Upza@kandula.db.elephantsql.com:5432/vpyfxlkr"

int create_tcp_server(in_addr_t ip, int port, int clients_max);
PGconn *connect_db();
char *get_timestamp();
in_addr_t set_ip(char *ip);

#endif /* _UTIL_H_ */