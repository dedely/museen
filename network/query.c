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
    char query[INSERT_QUERY_MAX_SIZE] = "SELECT visitor_id FROM visitor WHERE visitor_authkey_hash = \'";
    strncat(query, auth_key, INSERT_QUERY_MAX_SIZE - (strlen(query) + 1));
    strncat(query, "\';", INSERT_QUERY_MAX_SIZE - (strlen(query) + 1));
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

/**
 * @brief
 *
 * @param conn
 * @param id
 * @param loc
 * @param time_in
 * @param time_out
 * @return int
 */
int store_position(PGconn *conn, char *id, char *loc, char *time_in, char *time_out) {
    char query[INSERT_QUERY_MAX_SIZE] = "INSERT INTO \"public\".\"location_history\" (location_visitor_id, location_id, location_time_in, location_time_out) VALUES(\'";
    strncat(query, id, INSERT_QUERY_MAX_SIZE - strlen(query));
    strncat(query, "\', \'", INSERT_QUERY_MAX_SIZE - strlen(query));
    strncat(query, loc, INSERT_QUERY_MAX_SIZE - strlen(query));
    strncat(query, "\', \'", INSERT_QUERY_MAX_SIZE - strlen(query));
    strncat(query, time_in, INSERT_QUERY_MAX_SIZE - strlen(query));
    strncat(query, "\', \'", INSERT_QUERY_MAX_SIZE - strlen(query));
    strncat(query, time_out, INSERT_QUERY_MAX_SIZE - strlen(query));
    strncat(query, "\');", INSERT_QUERY_MAX_SIZE - strlen(query));
    printf("%s\n", query);
    PGresult *result;
    result = PQexec(conn, query);
    ExecStatusType resultStatus;
    resultStatus = PQresultStatus(result);

    printf("%s\n", PQresStatus(resultStatus));
    printf("%s\n", PQresultErrorMessage(result));
    return (resultStatus == PGRES_COMMAND_OK);
}

/**
 * @brief Queries the database to find the available informations about an artwork given its location
 *
 * @param conn
 * @param loc should be a string
 * @return char*
 */
char *query_info(PGconn *conn, char *loc) {
    char *infos = NULL;
    char query[INFO_QUERY_MAX_SIZE] = "SELECT ATWK.artwork_title, ATWK.artwork_type, ATWK.artwork_date, AR.artist_first_name, AR.artist_last_name, AR.artist_birth, AR.artist_death, AR.artist_bio, AM.artistic_movement_name, L.loc_room_name ";

    strncat(query, "FROM artwork AS ATWK JOIN location AS L ON L.loc_id = ATWK.artwork_location JOIN artist AS AR ON ATWK.artwork_artist = AR.artist_id ", INFO_QUERY_MAX_SIZE - strlen(query));
    strncat(query, "JOIN artistic_movement AS AM ON ATWK.artwork_movement_id = AM.artistic_movement_id WHERE L.loc_id = ", INFO_QUERY_MAX_SIZE - strlen(query));
    strncat(query, loc, INFO_QUERY_MAX_SIZE - strlen(query));
    strncat(query, ";", INFO_QUERY_MAX_SIZE - strlen(query));

    PGresult *result;
    result = PQexec(conn, query);
    ExecStatusType resultStatus;
    resultStatus = PQresultStatus(result);

    //Format results
    if ((resultStatus == PGRES_TUPLES_OK) && PQntuples(result) == 1) {
        int col = PQnfields(result);
        int size = col - 1;
        int i;
        char *tmp;
        for (i = 0; i < col; i++) {
            size += PQgetlength(result, 0, i);
        }
        printf("size = %d\n", size);
        infos = malloc_str(size);
        for (int i = 0; i < col; i++) {
            tmp = (PQgetisnull(result, 0, i)) ? "?" : PQgetvalue(result, 0, i);
            //Call strncpy first
            if (i == 0) {
                strncpy(infos, tmp, size - strlen(infos));
            }
            else {
                strncat(infos, SEPARATOR, size - strlen(infos));
                strncat(infos, tmp, size - strlen(infos));
            }
        }
    }
    return infos;
}

/**
 * @brief Returns a list of artwork_id separated with strings.
 *
 * @param conn
 * @param id
 * @return char*
 */
char *query_sugg(PGconn *conn, char *id) {

    char query[INFO_QUERY_MAX_SIZE] = "SELECT ATWK.artwork_id, ATWK.artwork_artist, (ATWK.artwork_popularity * AMP.amp_score) AS Ordre ";
    strncat(query, "FROM visitor as  V JOIN artistic_movement_preference AS AMP ON V.visitor_id = AMP.amp_visitor_id ", INFO_QUERY_MAX_SIZE - strlen(query));
    strncat(query, "JOIN artwork AS ATWK ON AMP.amp_artistic_movement_id = ATWK.artwork_movement_id WHERE V.visitor_id = '", INFO_QUERY_MAX_SIZE - strlen(query));
    strncat(query, id, INFO_QUERY_MAX_SIZE - strlen(query));
    strncat(query, "' ORDER BY Ordre DESC LIMIT 15;", INFO_QUERY_MAX_SIZE - strlen(query));
    printf("%s\n", query);
    PGresult *result;
    result = PQexec(conn, query);
    ExecStatusType resultStatus;
    resultStatus = PQresultStatus(result);

    printf("%s\n", PQresStatus(resultStatus));
    printf("%s\n", PQresultErrorMessage(result));

    if (resultStatus == PGRES_TUPLES_OK) {
        int rows = PQntuples(result);
        rows = (rows > 5) ? 5 : rows;


    }
}

/**
 * @brief Data insertion test
 *
 * @param conn
 */
void insert_test(PGconn *conn) {
    printf("Running INSERT INTO query test...\n");
    char query[INSERT_QUERY_MAX_SIZE] = "INSERT INTO \"public\".\"location_history\" (location_visitor_id, location_id, location_time_in, location_time_out) VALUES(\'jtest\', 1,\'";
    char *timestamp = get_timestamp();
    strncat(query, timestamp, INSERT_QUERY_MAX_SIZE - strlen(query));
    strncat(query, "\', \'", INSERT_QUERY_MAX_SIZE - strlen(query));
    strncat(query, timestamp, INSERT_QUERY_MAX_SIZE - strlen(query));
    strncat(query, "\');", INSERT_QUERY_MAX_SIZE - strlen(query));
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