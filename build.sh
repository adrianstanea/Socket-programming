#!/bin/bash
echo "Building the project"

gcc -o main main.c client.c server.c src/logs.c ./src/utils.c src/constants.c 