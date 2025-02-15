#include "cache.h"

int32 handle_hello(Client*, int8*, int8*);

bool scontinuation;
bool ccontinuation;

CmdHandler handlers[] = {
    { (int8 *)"hello", handle_hello }
};

Callback getcmd(int8 *cmd) {
    Callback cb;
    int16 n, arrlen;

    if (sizeof(handlers) < 16)
        return 0;

    arrlen = sizeof(handlers) / 16;

    cb = 0;
    for (n=0; n<arrlen; n++) 
        if(!strcmp((char *)cmd, (char *)handlers[n].cmd)) {
            cb = handlers[n].handler;
            break;
        }
        
    return cb;
};

int32 handle_hello(Client *cli, int8* folder, int8 *args) {
    dprintf(cli->s, "hello '%s'\n", folder);
    return 0;
}

void zero(int8* buf, int16 size) {
    int8 *p;
    int16 n;

    for (n=0, p=buf; n < size; n++, p++)
        *p = 0;

    return;
}

void childloop(Client *cli) {
    int8 buf[256];
    int8 *p, *f;
    int16 n;
    int8 cmd[256], folder[256], args[256];

    // write(cli->s, "hey\n", 4);
    // exit(0);

    zero(buf, 256);
    read(cli->s, (char *)buf, 255);
    n = (int16)strlen((char *)buf);

    if (n > 254)
        n = 254;

    for (p=buf; (*p) && (n--) && (*p != ' ') && (*p != '\n' && (*p != '\r')); p++);

    zero(cmd, 256);
    zero(folder, 256);
    zero(args, 256);

    if (!(*p) || (!n)) {
        strncpy((char *)cmd, (char *)buf, 255);
        goto done;
    } else if ((*p == '\n') || (*p == '\r')) {
        *p = 0; // 終端文字を入れる
        strncpy((char *)cmd, (char *)buf, 255);
        goto done;
    } else if (*p == ' ') {
        *p = 0;
        strncpy((char *)cmd, (char *)buf, 255);
    }

    for (p++, f=p; (*p) && (n--) && (*p != ' ') && (*p != '\n' && (*p != '\r')); p++);

    if (!(*p) || (!n)) {
        strncpy((char *)folder, (char *)f, 255);
        goto done;
    } else if ((*p == ' ') || (*p == '\n') || (*p == '\r')) {
        *p = 0;
        strncpy((char *)folder, (char *)f, 255);
    }

    p++;

    if (*p) {
        strncpy((char *)args, (char *)p, 255);
        for (p=args; ((*p) && (*p != '\n') && (*p != '\r')); p++);
        *p = 0;
    }

    done:
        dprintf(cli->s, "cmd:\t%s\n", cmd);
        dprintf(cli->s, "folder:\t%s\n", folder);
        dprintf(cli->s, "args:\t%s\n", args);

    return;
}

void mainloop(int s) {
    // do something
    struct sockaddr_in cli;
    int32 len;
    int s2;
    char *ip;
    int16 port;
    Client *client;
    // Unix
    pid_t pid;
    // Windows
    // STARTUPINFO si;
    // PROCESS_INFORMATION pi;

    s2 = accept(s, (struct sockaddr *)&cli, (unsigned int *)&len);
    if (s2 < 0) 
        return;

    port = (int16)htons((int)cli.sin_port);
    ip = inet_ntoa(cli.sin_addr);

    printf("Connection from %s:%d\n", ip, port);

    client = (Client *)malloc(sizeof(struct s_client));
    assert(client);

    zero((int8 *)client, sizeof(struct s_client));
    client->s = s2;
    client->port = port;
    strncpy(client->ip, ip, 15);

    // for Windows
    // ZeroMemory(&si, sizeof(si));
    // si.cb = sizeof(si);
    // ZeroMemory(&pi, sizeof(pi));

    // if (!CreateProcess(NULL, cmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
    //     perror("CreateProcess failed");
    //     free(client);
    //     return;
    // } else {
    //     dprintf(s2, "100 Connected to Cache server.\n");
    //     ccontinuation = true;
    //     while (ccontinuation)
    //         childloop(client);

    //     close(s2);
    //     // 子プロセスのハンドルを閉じる
    //     CloseHandle(pi.hProcess);
    //     CloseHandle(pi.hThread);
    //     free(client);
    //     return;
    // }

    // for Unix
    pid = fork();
    if (pid) {
        free(client);
        return;
    } else {
        dprintf(s2, "100 Connected to Cache server.\n");
        ccontinuation = true;
        while (ccontinuation)
            childloop(client);

        close(s2);
        free(client);
        return;
    }
}

int initserver(int16 port) {
    struct sockaddr_in sock;
    int s;

    printf("initserver called\n");

    sock.sin_family = AF_INET;
    sock.sin_port = htons(port);
    sock.sin_addr.s_addr = inet_addr(HOST);

    s = socket(AF_INET, SOCK_STREAM, 0);
    assert(s > 0);

    errno = 0;
    if (bind(s, (struct sockaddr *)&sock, sizeof(sock)))
        win_assert_perror(errno);

    errno = 0;
    if (listen(s, 20))
        win_assert_perror(errno);
    
    printf("Server listening on %s:%d\n", HOST, port);
    fflush(stdout);

    return s;
}

int main(int argc, char *argv[]) {
    int16 port;
    int s;

    Callback x;

    x = getcmd((int8 *)"hello");
    printf("%p\n", x);

    x = getcmd((int8 *)"asafa");
    printf("%p\n", x);
    return 0;
    // if (argc < 2) {
    //     port = (int16)PORT;
    // } else {
    //     char* sport = argv[1];
    //     port = (int16)atoi(sport);
    // }

    // // Windows
    // // WSADATA wsaData;
    // // Winsockの初期化
    // // if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    // //     fprintf(stderr, "WSAStartup failed\n");
    // //     exit(EXIT_FAILURE);
    // // }
    // s = initserver(port);
    
    // scontinuation = true;
    // while(scontinuation)
    //     mainloop(s);
    
    // // Windows
    // // closesocket(s);
    // // WSACleanup();

    // return 0;
}

#pragma GCC diagnostic pop