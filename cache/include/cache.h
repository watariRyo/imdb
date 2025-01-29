#ifndef CACHE
#define CACHE

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#include <assert.h>
#include <errno.h>
// Windows
#include <winsock2.h>
// #pragma comment(lib, "Ws2_32.lib")
// UNIX
// #include <arpa/inet.h>
// #include <sys/socket.h>
// #include <netinet/in.h>

void win_assert_perror(int errnum) {
    if (errnum != 0) {
        fprintf(stderr, "Error: %s\n", strerror(errnum));
        exit(EXIT_FAILURE);
    }
}

#define HOST        "127.0.0.1"
#define PORT        12049

typedef unsigned int int32;
typedef unsigned short int int16;
typedef unsigned char int8;

struct s_client {
    int s;
    char ip[16];
    int16 port;
};
typedef struct s_client Client;

void zero(int8*, int16);
void childloop(Client*);
void mainloop(int);
int initserver(int16);
int main(int,char**);

#endif