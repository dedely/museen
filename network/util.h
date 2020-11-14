#ifndef _UTIL_H_
#define _UTIL_H_

#include </usr/include/postgresql/libpq-fe.h>

#define DB_INFO "host = localhost port = 5432 dbname = museen user = museendba password = museen"
#define SERVER_PORT 42424

int create_tcp_server(int port, int clients_max);
PGconn *connect_db();

#endif /* _UTIL_H_ */