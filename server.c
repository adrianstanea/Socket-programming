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

#include "server.h"

void free_server(server_t *server)
{
    if (server == NULL)
    {
        log_message(LOG_ERROR, "Error freeing memory for server");
        return;
    }

    if (server->socket_fd != -1)
    {
        log_message(LOG_DEBUG, "Closed socket %d", server->socket_fd);
        close(server->socket_fd);
    }
    if (server->client_fd != -1)
    {
        log_message(LOG_DEBUG, "Closed socket %d", server->client_fd);
        close(server->client_fd);
    }
}


struct sockaddr_in resolve_server_address(const char *server_ip, const int server_port)
{
    log_message(LOG_INFO, "Resolving server address");
    log_message(LOG_DEBUG, "Server IP: %s, service: %d", server_ip, server_port);
	struct sockaddr_in myaddr;

    myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(server_port);

	int ret = inet_pton(myaddr.sin_family, server_ip, &(myaddr.sin_addr));
    if (ret != 1)
    {
        log_message(LOG_ERROR, "Error when calling inet_pton");
        exit(-1);
    }
    else
    {
        log_message(LOG_DEBUG, "Successfully called inet_pton");
    }
    
    return myaddr;
}

void handle_bind(server_t *server)
{
    log_message(LOG_INFO, "Binding socket");
    int ret = bind(server->socket_fd, (struct sockaddr *)&server->addr_in, sizeof(server->addr_in));
    if (ret != 0)
    {
        log_message(LOG_ERROR, "Error when calling bind");
        exit(-1);
    }
    else
    {
        log_message(LOG_DEBUG, "Successfully called bind");
    }

    const int BACKLOG = 10;
    log_message(LOG_INFO, "Listening on socket");
    ret = listen(server->socket_fd, BACKLOG);
    if (ret != 0)
    {
        log_message(LOG_ERROR, "Error when calling listen");
        exit(-1);
    }
    else
    {
        log_message(LOG_DEBUG, "Successfully called listen");
    }
}

void server_accept(server_t *server)
{
    log_message(LOG_INFO, "Accepting connection");
    server->client_addr_len = sizeof(server->client_addr);
    server->client_fd = accept(server->socket_fd, (struct sockaddr *)&server->client_addr, &server->client_addr_len);
    if (server->client_fd == -1)
    {
        log_message(LOG_ERROR, "Error when calling accept");
        exit(-1);
    }
    else
    {
        log_message(LOG_DEBUG, "Successfully called accept");
    }
}

int parse_command(server_t *server)
{
    log_message(LOG_INFO, "Parsing command from client");
    char client_cmd[10];

    int bytes_received = recv(server->client_fd, client_cmd, sizeof(client_cmd), 0);
    log_message(LOG_DEBUG, "Received %d bytes", bytes_received);
    log_message(LOG_DEBUG, "Received data: %s", client_cmd);
    if (bytes_received == -1)
    {
        log_message(LOG_ERROR, "Error when parsing response");
        exit(-1);
    }
    else if (bytes_received == 0)
    {
        log_message(LOG_INFO, "Connection closed by peer");
        return 0;
    }
    else
    {
        log_message(LOG_DEBUG, "Received data: %s", client_cmd);
    }

    return strcmp(client_cmd, "04#") == 0? 1 : -1;
}

void server_send(server_t *server, char *message)
{
    log_message(LOG_INFO, "Sending message to client");
    int bytes_sent = send(server->client_fd, message, strlen(message), 0);
    if (bytes_sent == -1)
    {
        log_message(LOG_ERROR, "Error when sending message");
        exit(-1);
    }
    else
    {
        log_message(LOG_DEBUG, "Successfully sent message");
    }

}