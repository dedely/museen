#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "query.h"
#include "types.h"
#include "util.h"

/**
 * @brief Connects to the database server in a synchronous(blocking) fashion.
 *
 * @return PGconn*
 */
PGconn *connect_db(char *url) {
    PGconn *conn = PQconnectdb(url);
    if (PQstatus(conn) == CONNECTION_BAD) {
        printf("Connexion to database server failed : %s", PQerrorMessage(conn));
        exit(EXIT_FAILURE);
    }
    else if (PQstatus(conn) == CONNECTION_OK) {
        printf("Connected to database server\n");
    }
    return conn;
}

/**
 * @brief Checks if a visitor matches with the provided auth_key in the database.
 *
 * @param conn
 * @param auth_key
 * @return char*
 */
char *query_login(PGconn *conn, char *auth_key) {
    char *id = malloc_str(ID_SIZE);
    bzero(id, ID_SIZE);
    char query[Q_MAX_SIZE] = "SELECT visitor_id FROM visitor WHERE visitor_authkey_hash = \'";
    strncat(query, auth_key, Q_MAX_SIZE - (strlen(query) + 1));
    strncat(query, "\';", Q_MAX_SIZE - (strlen(query) + 1));
    //printf("%s\n", query);
    PGresult *result;
    result = PQexec(conn, query);
    ExecStatusType resultStatus;
    resultStatus = PQresultStatus(result);

    //printf("%s\n", PQresStatus(resultStatus));
    //printf("%s\n", PQresultErrorMessage(result));
    if ((resultStatus == PGRES_TUPLES_OK) && PQntuples(result) == 1) {
        strncat(id, PQgetvalue(result, 0, 0), ID_SIZE);
        //printf("result = %s\n", id);
    }
    return id;
}


int store_position(PGconn *conn, char *id, char *loc, char *time_in, char *time_out) {
    char query[Q_MAX_SIZE] = "INSERT INTO \"public\".\"location_history\" (location_visitor_id, location_id, location_time_in, location_time_out) VALUES(\'";
    strncat(query, id, Q_MAX_SIZE - strlen(query));
    strncat(query, "\', \'", Q_MAX_SIZE - strlen(query));
    strncat(query, loc, Q_MAX_SIZE - strlen(query));
    strncat(query, "\', \'", Q_MAX_SIZE - strlen(query));
    strncat(query, time_in, Q_MAX_SIZE - strlen(query));
    strncat(query, "\', \'", Q_MAX_SIZE - strlen(query));
    strncat(query, time_out, Q_MAX_SIZE - strlen(query));
    strncat(query, "\');", Q_MAX_SIZE - strlen(query));
    printf("%s\n", query);
    PGresult *result;
    result = PQexec(conn, query);
    ExecStatusType resultStatus;
    resultStatus = PQresultStatus(result);

    printf("%s\n", PQresStatus(resultStatus));
    printf("%s\n", PQresultErrorMessage(result));
    return (resultStatus == PGRES_COMMAND_OK);
}


char *query_info(PGconn *conn, char *loc){
    
}

/**
 * @brief Data insertion test
 *
 * @param conn
 */
void insert_test(PGconn *conn) {
    printf("Running INSERT INTO query test...\n");
    char query[Q_MAX_SIZE] = "INSERT INTO \"public\".\"location_history\" (location_visitor_id, location_id, location_time_in, location_time_out) VALUES(\'jtest\', 1,\'";
    char *timestamp = get_timestamp();
    strncat(query, timestamp, Q_MAX_SIZE - strlen(query));
    strncat(query, "\', \'", Q_MAX_SIZE - strlen(query));
    strncat(query, timestamp, Q_MAX_SIZE - strlen(query));
    strncat(query, "\');", Q_MAX_SIZE - strlen(query));
    printf("%s\n", query);
    PGresult *result;
    result = PQexec(conn, query);
    ExecStatusType resultStatus;
    resultStatus = PQresultStatus(result);

    printf("%s\n", PQresStatus(resultStatus));
    printf("%s\n", PQresultErrorMessage(result));
}

/**
 * @brief Performs an authentification test
 *
 * @param conn
 */
void auth_test(PGconn *conn) {
    printf("Running authentification query test...\n");
    query_login(conn, DEBUG_KEY);
}