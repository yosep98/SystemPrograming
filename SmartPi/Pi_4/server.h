#ifndef SERVER_H
#define SERVER_H

typedef struct server {
    char* ip_addr;
    int port;
} Server;

void error_handling(char *message);
void *Connect_server1(void *arg);
void *Connect_server2(void *arg);

#endif // SERVER_H
