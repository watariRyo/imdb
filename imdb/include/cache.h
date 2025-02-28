#define _GNU_SOURCE

#ifndef CACHE
#define CACHE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <stddef.h>
#include <stdarg.h>

#include <assert.h>
#include <errno.h>
#ifdef _WIN32
    // Windows
    #include <winsock2.h>
    #pragma comment(lib, "Ws2_32.lib")
# else
    // UNIX
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
# endif

#include "tree.h"

#pragma GCC diagnostic push

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

typedef int32 (*Callback)(Client*, int8*, int8*);

struct s_cmdHandler {
    int8* cmd;
    Callback handler;
};
typedef struct s_cmdHandler CmdHandler;

// void zero(int8*, int16);
void childloop(Client*);
void mainloop(int);
int initserver(int16);
int main(int,char**);

#endif