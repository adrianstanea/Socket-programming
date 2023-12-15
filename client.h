#ifndef CLIENT_H
#define CLIENT_H

#include "src/custom_types.h"
#include "src/constants.h"

void init_client(client_t *client);
void free_client(client_t *client);

struct addrinfo* resolve_host_address(const char *host, const char *service_port);
int connect_to_server(client_t *client);
int send_http_request(client_t *client, const char *http_request);
int parse_http_response(client_t *client);


#endif // CLIENT_H
