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
#include "client.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <LOG_LEVEL>\n", argv[0]);
        return -1;
    }
    log_level = (log_level_t)atoi(argv[1]);

    if (log_level < LOG_DEBUG || log_level > LOG_ERROR)
    {
        printf("Invalid log level must be between %d and %d\n", LOG_DEBUG, LOG_ERROR);
        return -1;
    }
    set_log_level(log_level);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    server.addr_in = resolve_server_address(SERVER_IP, SERVER_PORT);
    server.socket_fd = create_socket(PF_INET, SOCK_STREAM, 0);
    handle_bind(&server);

    while (1)
    {
        log_message(LOG_INFO, "Waiting for connections...");
        server_accept(&server);
        log_message(LOG_INFO, "Accepted connection from client");

        int command_state;
        do
        {
            log_message(LOG_INFO, "SERVER: Waiting for command from client");
            command_state = parse_command(&server);
            if (command_state == -1) // invalid command
            {
                server_send(&server, "Can't execute command\n");
                log_message(LOG_ERROR, "SERVER: Can't execute command");
            }
            if (command_state == 1) // valid command
            {
                client_t client;
                init_client(&client);
                send_http_request(&client, HTTP_GET_REQ);
                log_message(LOG_INFO, "CLIENT: Retrieved HTML page and saved it to file");
                parse_http_response(&client);
                free_client(&client);

                log_message(LOG_INFO, "SERVER: Sending response to client from file");
                FILE *fp = open_file(FILE_NAME, "r");

                ssize_t read;
                char line[BUFF_SIZE];
                log_message(LOG_INFO, "SERVER: Sending html file content to client");
                while ((read = fread(line, 1, BUFF_SIZE, fp)) > 0)
                {
                    if (send(server.client_fd, line, read, 0) == -1)
                    {
                        log_message(LOG_ERROR, "SERVER: Error when sending file content to client");
                        break;
                    }
                    else
                    {
                        log_message(LOG_DEBUG, "SERVER: Successfully sent file content to client");
                    }
                }
                fclose(fp);
            }
        } while (command_state); // when command_state is 0, client closed connection
        // Close current client connection
        if (close(server.client_fd) == 0)
            log_message(LOG_DEBUG, "SERVER: Closed connection with client");
        else
            log_message(LOG_ERROR, "SERVER: Error when closing client connection");
    }
    return 0;
}