#ifndef _QUERY_H_
#define _QUERY_H_

#include <libpq-fe.h>
#include "types.h"

#define ID_SIZE 21 //20 + 1 : visitor_id(s) have a maximum length of 20 characters
#define INSERT_QUERY_MAX_SIZE 250
#define INFO_QUERY_MAX_SIZE 450
#define SEPARATOR ";"

PGconn *connect_db();
char *query_login(PGconn *conn, char *auth_key);
int store_position(PGconn *conn, char *id, char *loc, char *time_in, char *time_out);
void insert_test(PGconn *conn);
void auth_test(PGconn *conn);
char *query_info(PGconn *conn, char *loc);
char *query_sugg(PGconn *conn, char *id);

#endif /* _QUERY_H_ */