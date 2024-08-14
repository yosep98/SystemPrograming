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
#include "lcd.h"
#include "buzzer.h"

#define BUFFER_MAX 32
#define DIRECTION_MAX 256
#define VALUE_MAX 256

#define LOW 0
#define HIGH 1

//Get server information
typedef struct server {
    char* ip_addr;
    int port;
} Server;

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

void *Connect_server(void *arg);
void *Connect_server2(void *arg);

int main(int argc, char *argv[]) {

  pthread_t p_thread[2];
  int thr_id;
  Server* server = (Server*)malloc(sizeof(Server));
  Server* server2 = (Server*)malloc(sizeof(Server));
  
  clear_LCD();

  if(!(argc == 3 || argc == 5)){
    printf("Usage : %s <IP> <port>\n", argv[0]);
    printf("OR\n");
    printf("Usage : %s <IP> <port> <IP> <port>\n",argv[0]);
    exit(1);
  }

  //다중 서버 연결
  if(argc == 3){
    server->ip_addr = argv[1];
    server->port = atoi(argv[2]);

    thr_id = pthread_create(&p_thread[0], NULL, Connect_server, server);
    if (thr_id < 0) {
      perror("thread create error : ");
      exit(0);
    }
  }
  else if(argc == 5){
    server->ip_addr = argv[1];
    server->port = atoi(argv[2]);

    server2->ip_addr = argv[3];
    server2->port = atoi(argv[4]);

    thr_id = pthread_create(&p_thread[0], NULL, Connect_server, server);
    if (thr_id < 0) {
      perror("thread create error : ");
      exit(0);
    }
    thr_id = pthread_create(&p_thread[1], NULL, Connect_server2, server2);
    if (thr_id < 0) {
      perror("thread create error : ");
      exit(0);
    }
  }

  for(int i = 0; i < 2; i++){
    pthread_join(&p_thread[i], NULL);
  }
  
  return (0);
}

//주환 서버
void *Connect_server(void *arg){
  int sock;
  struct sockaddr_in serv_addr;
  char msg[32] = {0};
  int str_len;
  int light = 0;
  int prev_state = 0, state;
  int prev_state2 = 0, state2;
  Server* server = (Server*)arg;

  printf("IP: %s\n",server->ip_addr);
  printf("Port: %d\n",server->port);

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) error_handling("socket() error");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(server->ip_addr);
  serv_addr.sin_port = htons(server->port);

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("connect() error");

  printf("IP: %s Connection established\n",server->ip_addr);

  while (1) {

    str_len = read(sock, msg, sizeof(msg));
    if (str_len == -1) error_handling("read() error");

    printf("Receive message from %s : %s\n", server->ip_addr, msg);

    state = msg[0] - '0';
    state2 = msg[1] - '0';

    //in_sensor 판정
    if (prev_state == 0 && state == 1) {
      printf("%s: Change state!!\n",server->ip_addr);
      activate_LCD();
      airplane_BUZZER();
    }

    //out_sensor 판정
    if (prev_state2 == 0 && state2 == 1) {
      printf("%s: Change state!!\n",server->ip_addr);
      welcome_LCD();
      elise_BUZZER();
    }

    prev_state = state;
    prev_state2 = state2;

  }

  close(sock);
}

//현재 서버
void *Connect_server2(void *arg){
  int sock;
  struct sockaddr_in serv_addr;
  char msg[32] = {0};
  int str_len;
  int light = 0;
  int prev_state = 0, state;
  Server* server = (Server*)arg;

  printf("IP: %s\n",server->ip_addr);
  printf("Port: %d\n",server->port);

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) error_handling("socket() error");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(server->ip_addr);
  serv_addr.sin_port = htons(server->port);

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("connect() error");

  printf("IP: %s Connection established\n",server->ip_addr);

  while (1) {

    str_len = read(sock, msg, sizeof(msg));
    if (str_len == -1) error_handling("read() error");

    printf("Receive message from %s : %s\n", server->ip_addr, msg);

    state = msg[0] - '0';

    //카메라 판정
    if (prev_state == 0 && state == 1) {
      printf("%s: Change state!!\n",server->ip_addr);
      box_LCD();
      school_BUZZER();
    }

    prev_state = state;

  }
  
  close(sock);
}