/*-------------------------------------------------------------------------
 *
 * types.h
 *	  This file contains the types of server replies
 *    and their corresponding integer code.
 *    It also contains states used for internal logic.
 *
 *-------------------------------------------------------------------------
 */
#ifndef _TYPE_H_
#define _TYPE_H_

 /**
  * @brief Types of server reply during the client authentification.
  *
  */
enum ConnectionStatusType {
    CONN_AUTH_OK = 110, /*Client is connected and authentified.*/
#define CONN_AUTH_OK CONN_AUTH_OK
    CONN_FAILED_UKN = 120, /*Connection failed: User is unknown.*/
#define CONN_FAILED_UKN CONN_FAILED_UKN
    CONN_FAILED_NOT_PREM = 130, /*Connection failed: User is not premium.*/
#define CONN_FAILED_NOT_PREM CONN_FAILED_NOT_PREM
    CONN_FAILED_NOT_READY = 140, /*Connection failed: Server is not ready.*/
#define CONN_FAILED_NOT_READY CONN_FAILED_NOT_READY
};

/**
 * @brief Types of server reply to a client request type.
 *
 */
typedef enum {
    REPLY_FORMAT_ERROR = 200, /*Incorrect  data format*/
#define REPLY_FORMAT_ERROR REPLY_FORMAT_ERROR
    REPLY_FORMAT_OK = 210, /*Client query is in the correct format and the server
                            is ready to recieve data*/
#define REPLY_FORMAT_OK REPLY_FORMAT_OK
                            REPLY_CLIENT_TIMEOUT = 220, /*Client has been timed out*/
#define REPLY_CLIENT_TIMEOUT REPLY_CLIENT_TIMEOUT
                            REPLY_CLIENT_NOT_AUTH = 230, /*Client is not logged in*/
#define REPLY_CLIENT_NOT_AUTH REPLY_CLIENT_NOT_AUTH
                            REPLY_SERVER_NOT_READY = 240, /*Server is not ready*/
#define REPLY_SERVER_NOT_READY REPLY_SERVER_NOT_READY
} ServerReplyType;


typedef enum {
    CODE_EXIT,
    CODE_LOGN,
    CODE_INFO,
    CODE_LOCA,
    CODE_SUGG
}  QueryCodeType;

#define QUERY_CODE_LENGTH 4
#define QUERY_CODE_TYPES_COUNT 5
extern const char *query_code_types[QUERY_CODE_TYPES_COUNT]; 

/**
 * @brief Types of result after running or trying to run the client's query
 *        i.e. after both the query type and its data were recieved by the server.
 *
 */
enum ResultStatusType {
    RES_UNKNOWN_FORMAT = 300,
#define RES_UNKNOWN_FORMAT RES_UNKNOWN_FORMAT
    RES_COMMAND_OK = 310, /*The command was successful*/
#define RES_COMMAND_OK RES_COMMAND_OK
    RES_UNKNOWN_POS = 320, /*The position is unknown*/
#define RES_UNKNOWN_POS RES_UNKNOWN_POS
};

typedef enum {
    CLIENT_INIT = 0, /*The initial state :TCP connection between the server and the client is established*/
    CLIENT_IDLE, /*Client is logged in and the connection is established but the client has not made any query*/
    CLIENT_MADE_QUERY, /*Client made a query*/
    CLIENT_TIMED_OUT /*Client has been timed out by the server*/
} ClientStateType;

typedef enum {
    EVENT_DISCONNECT = 0,
    EVENT_EXIT,
    EVENT_CONNECTED,
    EVENT_AUTH, /*Client sent a login key*/
    EVENT_INFO, /*Client sent an info query*/
    EVENT_SUGG, /*Client sent a suggestion query*/
    EVENT_DATA, /*Client sent data*/
    EVENT_UKN, /*Client sent unknow data format*/
    EVENT_BUF_OVERFLOW, /*Buffer overlow*/
    EVENT_TIMEOUT_END /*The client timeout has ended*/
} EventType;

#define EVENT_TYPES_COUNT 10
extern const char *event_types[EVENT_TYPES_COUNT]; 

enum ServerModeType {
    RUN_EXIT = 0, /*exit*/
#define RUN_EXIT RUN_EXIT
    RUN_DEFAULT = 1, /*Default mode*/
#define RUN_DEFAULT RUN_DEFAULT
    RUN_DB_DEBUG = 2/*Database debug mode*/
#define RUN_DB_DEBUG RUN_DB_DEBUG
};

typedef enum {
    SEVERITY_EMERGENCY,
    SEVERITY_ALERT,
    SEVERITY_CRITICAL,
    SEVERITY_ERROR,
    SEVERITY_WARNING,
    SEVERITY_INFO,
    SEVERITY_DEBUG
}LogSeverityType;

#define SEVERITY_TYPES_COUNT 7
extern const char *severity_types[SEVERITY_TYPES_COUNT]; 
#define IP_DEFAULT "default"
#define PORT_DEFAULT 42424
#define LOG_PATH_DEFAULT "./server.log"
#define DEBUG_KEY "8af763ee3a7549af94f472543bff710e09db9b217ab8e35a6c542db8e6330b0d"
#define LOCAL_DB_INFO "host = localhost port = 5432 dbname = postgres user = postgres password = chess"
#define ONLINE_DB_INFO "postgres://vpyfxlkr:Sjq64v_di28D9xpqzF4Y_XIiVkv6Upza@kandula.db.elephantsql.com:5432/vpyfxlkr"

#endif /*_TYPE_H_*/