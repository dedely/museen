#include "types.h"

const char *query_code_types[QUERY_CODE_TYPES_COUNT] = { "EXIT", "LOGN", "INFO", "LOCA", "SUGG" };
const char *event_types[EVENT_TYPES_COUNT] = {"Disconnect", "Client requested exit", "Client is connected", "Client sent a login key", "Client sent an info query", "Client sent a suggestion query", "Client sent data", "UNKNOWN FORMAT", "Buf_overflow", "timeout end"};
const char *severity_types[SEVERITY_TYPES_COUNT] = { "EMERGENCY", "ALERT", "CRITICAL", "ERROR", "WARNING", "INFO", "DEBUG" };