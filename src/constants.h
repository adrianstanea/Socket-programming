#include "custom_types.h"
#ifndef CONSTANTS_H
#define CONSTANTS_H
#define LOG_PREFIX_LEN 10

extern const char *HOST_ADDR;
extern const char *HOST_PORT;
extern const char *HTTP_GET_REQ;

extern const char *FILE_NAME;
extern const char *SERVER_IP;
extern const int SERVER_PORT;


extern log_level_t log_level;
extern client_t client;
extern server_t server;

#endif // CONSTANTS_H
