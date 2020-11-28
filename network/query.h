#ifndef _QUERY_H_
#define _QUERY_H_
#include </usr/include/postgresql/libpq-fe.h>
#include "types.h"

void insert_test(PGconn *conn);
void auth_test(PGconn *conn);

#endif /* _QUERY_H_ */