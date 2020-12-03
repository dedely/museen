#ifndef _QUERY_H_
#define _QUERY_H_

#include <libpq-fe.h>
#include "types.h"

#define ID_SIZE 21 //20 + 1 : visitor_id(s) have a maximum length of 20 characters
#define Q_MAX_SIZE 250
#define SEPARATOR ";"

PGconn *connect_db();
char *query_login(PGconn *conn, char *auth_key);
int store_position(PGconn *conn, char *id, char *loc, char *time_in, char *time_out);
void insert_test(PGconn *conn);
void auth_test(PGconn *conn);

#endif /* _QUERY_H_ */