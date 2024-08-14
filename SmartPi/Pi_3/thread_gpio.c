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

#define IN_GPIO 7        // 들어옴을 관장하는 모션센서를 이어주는 gpio
#define OUT_GPIO 8       // 나감을 ..

#define BUFFER_MAX 32
#define DIRECTION_MAX 256
#define VALUE_MAX 256
#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1


void error_handling(char *message) {        // socket 실행 시 serv_sock값이 -1이면 함수 실행
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

static int GPIOExport(int pin) {
  char buffer[BUFFER_MAX];
  ssize_t bytes_written;
  int fd;
  fd = open("/sys/class/gpio/export", O_WRONLY);
  if (-1 == fd) {
    fprintf(stderr, "Failed to open export for writing!\n");
    return (-1);
  }
  bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
  write(fd, buffer, bytes_written);
  close(fd);
  return (0);
}
static int GPIODirection(int pin, int dir) {
  static const char s_directions_str[] = "in\0out";
  char path[DIRECTION_MAX] = "/sys/class/gpio/gpio%d/direction";
  int fd;
  snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
  fd = open(path, O_WRONLY);
  if (-1 == fd) {
    fprintf(stderr, "Failed to open gpio direction for writing!\n");
    return (-1);
  }
  if (-1 ==
      write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
    fprintf(stderr, "Failed to set direction!\n");
    return (-1);
  }
  close(fd);
  return (0);
}
static int GPIORead(int pin) {
  char path[VALUE_MAX];
  char value_str[3];
  int fd;
  snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
  fd = open(path, O_RDONLY);
  if (-1 == fd) {
    fprintf(stderr, "Failed to open gpio value for reading!\n");
    return (-1);
  }
  if (-1 == read(fd, value_str, 3)) {
    fprintf(stderr, "Failed to read value!\n");
    return (-1);
  }
  close(fd);
  return (atoi(value_str));
}
static int GPIOWrite(int pin, int value) {
  static const char s_values_str[] = "01";
  char path[VALUE_MAX];
  int fd;
  snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
  fd = open(path, O_WRONLY);
  if (-1 == fd) {
    fprintf(stderr, "Failed to open gpio value for writing!\n");
    close(fd);
    return (-1);
  }
  if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
    fprintf(stderr, "Failed to write value!\n");
    close(fd);
    return (-1);
  }
  close(fd);
  return (0);
}

#include <stdio.h>

int *file_read(void) {
    FILE *file;
    int* numbers = malloc(sizeof(int)*3);
    int i = 0;

    file = fopen("condition.txt", "r"); // 숫자가 저장된 파일을 읽기 모드로 열기
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return NULL;
    }

    while (fscanf(file, "%d", &numbers[i]) != EOF && i < 3) {
        i++;
    }

    fclose(file); // 파일 닫기

    return numbers;
}

void *thread_main(void *arg) {
    char in_value[3], out_value[3];
    int clnt_sock = *((int*)arg);   // 받아드리는 client 값
    char msg[32] = {0};

    printf("Client %d is connected\n", clnt_sock);  // client의 연결정보 print

    while (1) {
        in_value = GPIORead(IN_GPIO);   // 인풋 gpio에 대한 값을 읽어옴. default 0, 움직임 감지 = 1
        out_value = GPIORead(OUT_GPIO); // 위와 동

        if (clnt_sock==5){   // 먼저 들어온 client
            if (strcmp(in_value, "0") == 0 || strcmp(out_value, "0") == 0){          // in, out에 대해 변화가 없다면
                snprintf(msg, 32, "%d%d", strcmp(in_value, "0") == 0, strcmp(out_value, "0") == 0);  // msg에 01, 00 등과 같이 저장
                write(clnt_sock, msg, sizeof(msg));
                printf("%d %d\n", strcmp(in_value, "0") == 0, strcmp(out_value, "0") == 0);  // 보낸 데이터를 나도 확인함
                usleep(1000 * 1000 * 1);     // 1초에 한번씩 정보를 보냄
            } else {           // In, Out에 대해 변화가 있음
                snprintf(msg, 32, "%d%d", strcmp(in_value, "0") == 0, strcmp(out_value, "0") == 0);
                write(clnt_sock, msg, sizeof(msg));
                printf("%d %d\n", strcmp(in_value, "0") == 0, strcmp(out_value, "0") == 0);
                Usleep(1000 * 1000 * 30);   // In 또는 out을 인식하면 데이터를 보내고, 중복을 막기 위해 30초간 중지
            }
        }
        if (clnt_sock==6){   // 두번째 들어온 client
            int *weather;
            weather = file_read();  // 위의 정의한 함수를 통해 데이터를 배열로 받기

            snprintf(msg, 32, "%d%d%d", weather[0], weather[1], weather[2]);   // 닐씨 데이터를 msg에 넣음
            write(clnt_sock, msg, sizeof(msg));
            printf("%d %d %d \n", weather[0], weather[1], weather[2]);
            usleep(1000 * 1000 * 20);  // in 또는 out을 뒤에는 20초가 정지
        }
        usleep(1000*1000);

    }
    close(clnt_sock);
    return NULL;
}

int main(int argc, char *argv[]) {


    int serv_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    pthread_t t_id;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
    }

    if (-1 == GPIOExport(IN_GPIO) || -1 == GPIOExport(OUT_GPIO)) return (1);

    if (-1 == GPIODirection(IN_GPIO, IN) || -1 == GPIODirection(OUT_GPIO, OUT)) return (2);

    if (-1 == GPIOWrite(IN_GPIO, IN) || -1 == GPIOWrite(OUT_GPIO, OUT)) return (3);
  

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1) error_handling("listen() error");

    while(1) {
        clnt_addr_size = sizeof(clnt_addr);
        int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);  // client의 요청을 accept함
        pthread_create(&t_id, NULL, thread_main, (void*)&clnt_sock);   // client의 요청을 받아들이면 thread_main함수 실행
        pthread_detach(t_id);
    }

    close(serv_sock);
    return (0);
}