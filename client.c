#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "src/custom_types.h"
#include "src/constants.h"
#include "src/logs.h"
#include "src/utils.h"

#include "client.h"

void init_client(client_t *client)
{
    log_message(LOG_INFO, "Initializing client");
    client->addr_info = resolve_host_address(HOST_ADDR, HOST_PORT);
    client->socket_fd = create_socket(PF_INET6, SOCK_STREAM, 0);
    connect_to_server(client);
}

void free_client(client_t *client)
{
    if (client == NULL)
    {
        log_message(LOG_ERROR, "Error freeing memory for client");
        return;
    }

    if (client->socket_fd != -1)
    {
        log_message(LOG_DEBUG, "Closed socket %d", client->socket_fd);
        close(client->socket_fd);
    }
    if(client->addr_info != NULL)
    {
        log_message(LOG_DEBUG, "Freed addr_info");
        freeaddrinfo(client->addr_info);
    }
}

struct addrinfo* resolve_host_address(const char *host, const char *service_port)
{
    log_message(LOG_INFO, "Resolving host address");
    log_message(LOG_DEBUG, "Host: %s, service: %s", host, service_port);
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM; // TCP stream

    int ret = getaddrinfo(host, service_port, &hints, &res);
    if (ret)
    {
        log_message(LOG_ERROR, "Error when calling getaddrinfo");
        exit(-1);
    }
    else
    {
        log_message(LOG_DEBUG, "Successfully called getaddrinfo");
        return res;
    }
}

int connect_to_server(client_t *client)
{
    log_message(LOG_INFO, "Connecting to server");
    int ret = connect(client->socket_fd, 
                    client->addr_info->ai_addr, 
                    client->addr_info->ai_addrlen);
    if (ret != 0)
    {
        log_message(LOG_ERROR, "Error when calling connect");
        exit(-1);
    }
    else
    {
        log_message(LOG_DEBUG, "Succesfully called connect");
    }
}

int send_http_request(client_t *client, const char *http_request)
{
    log_message(LOG_INFO, "Sending HTTP request");
    int ret = send(client->socket_fd, http_request, strlen(http_request), 0);
    if (ret == -1)
    {
        log_message(LOG_ERROR, "Error when sending HTTP request");
        exit(-1);
    }
    else
    {
        log_message(LOG_DEBUG, "Succesfully sent HTTP request");
    }
}

int parse_http_response(client_t *client)
{
    FILE *fp = open_file(FILE_NAME, "w");
    int bytes_received = 0;
    do {
        bytes_received = recv(client->socket_fd, client->buffer, BUFF_SIZE, 0);

        if (bytes_received == -1)
        {
            log_message(LOG_ERROR, "Error when receiving data");
            exit(-1);
        }
        else if (bytes_received == 0)
        {
            log_message(LOG_INFO, "Connection closed by peer");
        }
        else
        {
            log_message(LOG_DEBUG, "Received %d bytes", bytes_received);
            log_message(LOG_DEBUG, "Received data: %s", client->buffer);
            fwrite(client->buffer, 1, bytes_received, fp);
        }
    } while (bytes_received > 0);
    fclose(fp);
}