#ifndef CUSTOM_TYPES_H
#define CUSTOM_TYPES_H
#include <arpa/inet.h>

#define  BUFF_SIZE 1024

typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_NONE
} log_level_t;

typedef struct {
    int socket_fd;                      // Socket file descriptor
    struct addrinfo* addr_info;         // Address information
    char buffer[BUFF_SIZE];
} client_t;

typedef struct {
    int socket_fd;                      // Socket file descriptor
    struct sockaddr_in addr_in;         // Address in
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len;
    int client_fd;
} server_t;

#endif // CUSTOM_TYPES_H