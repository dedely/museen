#ifndef _UTIL_H_
#define _UTIL_H_

#include </usr/include/postgresql/libpq-fe.h>

#define LOCAL_DB_INFO "host = localhost port = 5432 dbname = museen user = museendba password = museen"
#define ONLINE_DB_INFO "host = ec2-54-247-94-127.eu-west-1.compute.amazonaws.com port = 5432 dbname = dfs70dvplbikds user = ygwugebnudkysx password = a59c2f8aab4b282f6ef4ad405a84dfa1d5949a18ef49e08a60c72635c4527ccc"
#define SERVER_PORT 42424

int create_tcp_server(int port, int clients_max);
PGconn *connect_db();

#endif /* _UTIL_H_ */