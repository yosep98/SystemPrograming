#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
// #include <Python.h>

#define ll long long
#define BUF_SIZE 32
#define BUFFER_MAX 3
#define DIRECTION_MAX 256
#define VALUE_MAX 256

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
void *handle_client(void *arg)
{
    int clnt_sock = *((int *)arg);
    free(arg);
    char msg[BUF_SIZE] = {0};
    int state = 1, prev_state = 0, cnt = 0;
    FILE *file;
    ll ls = -1, cs = 100, push = 0;

    while (1)
    {
        file = fopen("detection_result.txt", "r");
        if (file == NULL)
        {
            perror("Error opening file");
            close(clnt_sock);
            return NULL;
        }

        if (fgets(msg, sizeof(msg), file) != NULL)
            state = atoi(msg);

        if (prev_state == 0 && state == 1)
        {
            snprintf(msg, BUF_SIZE, "%d%d", state, 4);
            write(clnt_sock, msg, sizeof(msg));
            printf("msg = %s\n", msg);
        }
        else if (prev_state == 1 && state == 0)
        {
            snprintf(msg, BUF_SIZE, "%d%d", state, 4);
            write(clnt_sock, msg, sizeof(msg));
            printf("msg = %s\n", msg);
        }

        prev_state = state;
        fclose(file);

        sleep(10.0);
    }
    close(clnt_sock);
    return NULL;
}

int main(int argc, char *argv[])
{
    int serv_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    pthread_t t_id;

    // 터미널에서 받아온 포트번호가 없으면 오류 출력
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 서버 소켓 생성 ipv4,tcp
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    // serv_addrr구조체 설정정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 생성된 소켓을 IP주소와 포트에 바인딩
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    while (1)
    {
        // clnt_addr 사이즈와 연결한 client의 sock를 사용해 스레드 생성
        clnt_addr_size = sizeof(clnt_addr);
        int *clnt_sock = malloc(sizeof(int));
        *clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (*clnt_sock == -1)
            error_handling("accept() error");

        printf("Connection established\n");
        pthread_create(&t_id, NULL, handle_client, (void *)clnt_sock);
        pthread_detach(t_id);
    }

    close(serv_sock);

    return 0;
}
