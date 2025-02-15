#include "cache.h"

bool scontinuation;
bool ccontinuation;

void zero(int8* buf, int16 size) {
    int8 *p;
    int16 n;

    for (n=0, p=buf; n < size; n++, p++)
        *p = 0;

    return;
}

void childloop(Client *cli) {
    sleep(1);

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
    char cmdLine[256];

    s2 = accept(s, (struct sockaddr *)&cli, (unsigned int *)&len);
    if (s2 < 0) 
        return;

    port = (int16)htons((int)cli.sin_port);
    ip = inet_ntoa(cli.sin_addr);

    printf("Connection from %s:%d\n", ip, port);

    client = (Client *)malloc(sizeof(struct s_client));
    assert(client);

    zero((int8 *)client, sizeof(struct s_client));
    client->s = s;
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
    //     printf("%d 100 Connected to Cache server.\n" ,s2);
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
        printf("%d 100 Connected to Cache server.\n" ,s2);
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

    if (argc < 2) {
        port = (int16)PORT;
    } else {
        char* sport = argv[1];
        port = (int16)atoi(sport);
    }

    // Windows
    // WSADATA wsaData;
    // Winsockの初期化
    // if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    //     fprintf(stderr, "WSAStartup failed\n");
    //     exit(EXIT_FAILURE);
    // }
    s = initserver(port);
    
    scontinuation = true;
    while(scontinuation)
        mainloop(s);
    
    // Windows
    // closesocket(s);
    // WSACleanup();

    return 0;
}