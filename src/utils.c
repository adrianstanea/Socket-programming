#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#include "custom_types.h"
#include "constants.h"
#include "logs.h"
#include "../client.h"
#include "../server.h"

#include "utils.h"

void signal_handler(int signal) {
    switch (signal)
    {
    case SIGINT: // Ctrl+C
    case SIGTERM : // kill
        log_message(LOG_INFO, "Exiting program with signal %d - network resources cleaned up.", signal);
        free_client(&client);
        free_server(&server);
        exit(0);
        break;
    default:
        log_message(LOG_ERROR, "Unhandled signal %d", signal);
        break;
    }
}

FILE* open_file(const char *filename, const char *mode)
{
    log_message(LOG_INFO, "Opening file");
    log_message(LOG_DEBUG, "Filename: %s, mode: %s", filename, mode);
    FILE *fp = fopen(filename, mode);
    if (fp == NULL)
    {
        log_message(LOG_ERROR, "Error when opening file");
        exit(-1);
    }
    else
    {
        log_message(LOG_DEBUG, "Succesfully opened file");
    }
    return fp;
}

int create_socket(int ai_family, int ai_socktype, int ai_protocol)
{
    log_message(LOG_INFO, "Creating socket");
    log_message(LOG_DEBUG, "Family: %d, type: %d, protocol: %d", ai_family, ai_socktype, ai_protocol);
    int sock_fd = socket(ai_family, ai_socktype, ai_protocol);
    if (sock_fd == -1)
    {
        log_message(LOG_ERROR, "Error when creating socket");
        exit(-1);
    }
    else
    {
        log_message(LOG_DEBUG, "Succesfully created socket file descriptor");
        return sock_fd;
    }
}
