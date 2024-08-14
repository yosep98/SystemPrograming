#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "moter.h"
#include "server.h"

int main(int argc, char *argv[]) {
    pthread_t p_thread[2];
    int thr_id;
    Server server1, server2;

    if (argc != 5) {
        printf("Usage : %s <IP1> <port1> <IP2> <port2>\n", argv[0]);
        exit(1);
    }

    initializeHardware(); // 하드웨어 초기화

    // 첫 번째 서버 설정
    server1.ip_addr = argv[1];
    server1.port = atoi(argv[2]);

    // 두 번째 서버 설정
    server2.ip_addr = argv[3];
    server2.port = atoi(argv[4]);
    // 첫 번째 서버 연결 스레드 생성
    thr_id = pthread_create(&p_thread[0], NULL, Connect_server1, &server1);
    if (thr_id < 0) {
        perror("thread create error : ");
        exit(0);
    }
    // 두 번째 서버 연결 스레드 생성
    thr_id = pthread_create(&p_thread[1], NULL, Connect_server2, &server2);
    if (thr_id < 0) {
        perror("thread create error : ");
        exit(0);
    }
    // 스레드 종료 대기
    for (int i = 0; i < 2; i++) {
        pthread_join(p_thread[i], NULL);
    }

    return 0;
}
