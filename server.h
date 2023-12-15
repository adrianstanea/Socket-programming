#ifndef SERVER_H
#define SERVER_H

void free_server(server_t *server);

struct sockaddr_in resolve_server_address(const char *server_ip, const int server_port);
void handle_bind(server_t *server);
void server_accept(server_t *server);
int parse_command(server_t *server);
void server_send(server_t *server, char *message);

#endif // SERVER_H
