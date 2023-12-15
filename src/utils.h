#ifndef UTILS_H
#define UTILS_H

#include <stdio.h> 
#include "../client.h"
#include "../server.h"

void signal_handler(int signal);
void cleanup_network_resources();
FILE* open_file(const char *filename, const char *mode);
int create_socket(int ai_family, int ai_socktype, int ai_protocol);

#endif // UTILS_H