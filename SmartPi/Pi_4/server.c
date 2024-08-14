#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "server.h"
#include "moter.h"
// 에러 경우
void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
// 첫 번째 서버 연결 스레드 함수
void *Connect_server1(void *arg) {
    int sock;
    struct sockaddr_in serv_addr;
    char msg[32];
    int str_len;
    Server* server = (Server*)arg;
    printf("Server 1 IP: %s\n", server->ip_addr);
    printf("Port: %d\n", server->port);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) error_handling("socket() error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server->ip_addr);
    serv_addr.sin_port = htons(server->port);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    printf("Server 1 Connection established\n");
    while (1) {
        str_len = read(sock, msg, sizeof(msg));
        if (str_len == -1) error_handling("read() error");
        printf("Receive message from Server 1: %s\n", msg);

        // 서버 1에서 받은 메시지를 처리
        if (strlen(msg) == 2 && msg[0] == '1') {
            control_extra_led(HIGH); // 입력값의 십의 자리 숫자가 1일 경우 추가된 LED를 켜기
        } else if (strlen(msg) == 2 && msg[0] == '0') {
            control_extra_led(LOW); // 입력값의 십의 자리 숫자가 0일 경우 추가된 LED를 끄기
        }
    }
    close(sock);
    return NULL;
}
// 두 번째 서버에 연결하는 스레드 함수
void *Connect_server2(void *arg) {
    int sock;
    struct sockaddr_in serv_addr;
    char msg[32];
    int str_len;
    Server* server = (Server*)arg;
    printf("Server 2 IP: %s\n", server->ip_addr);
    printf("Port: %d\n", server->port);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) error_handling("socket() error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server->ip_addr);
    serv_addr.sin_port = htons(server->port);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    printf("Server 2 Connection established\n");
    while (1) {
        str_len = read(sock, msg, sizeof(msg));
        if (str_len == -1) error_handling("read() error");
        printf("Receive message from Server 2: %s\n", msg);

        // 서버 2에서 받은 메시지를 처리
        if (strcmp(msg, "0") == 0) {
            control_all_leds(LOW); // 입력값이 0일 경우 모든 LED 끄기
        } else if (strlen(msg) == 3 && (msg[0] == '0' || msg[0] == '1') && (msg[1] == '0' || msg[1] == '1') && (msg[2] == '0' || msg[2] == '1')) {
            control_servo_and_led(msg); // 3자리 비트마스크에 따라 서보 모터와 LED 제어(수신된 날씨정보)
        } else {
            printf("Invalid input received: %s\n", msg);
        }
    }
    close(sock);
    return NULL;
}
