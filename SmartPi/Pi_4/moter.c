#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include "moter.h"

#define LOW 0
#define HIGH 1

#define SERVO1 17 // GPIO 17
#define SERVO2 27 // GPIO 27
#define SERVO3 22 // GPIO 22
#define LED1 23   // GPIO 23
#define LED2 24   // GPIO 24
#define LED3 25   // GPIO 25
#define EXTRA_LED 4 // GPIO 4

void gpio_export(int pin) {
    char buffer[3];
    int len = snprintf(buffer, sizeof(buffer), "%d", pin);
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, buffer, len);
    close(fd);
}

void gpio_unexport(int pin) {
    char buffer[3];
    int len = snprintf(buffer, sizeof(buffer), "%d", pin);
    int fd = open("/sys/class/gpio/unexport", O_WRONLY);
    write(fd, buffer, len);
    close(fd);
}

void gpio_direction(int pin, const char *direction) {
    char path[35];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);
    int fd = open(path, O_WRONLY);
    write(fd, direction, strlen(direction));
    close(fd);
}

void gpio_write(int pin, int value) {
    char path[30];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
    int fd = open(path, O_WRONLY);
    if (value == HIGH)
        write(fd, "1", 1);
    else
        write(fd, "0", 1);
    close(fd);
}

void delay_ms(int milliseconds) {
    usleep(milliseconds * 1000);
}
// 서보 모터 위치 설정
void set_servo_position(int servo_pin, int position) {
    int pulse_width = 1000 + (position * 10); // position에 따라 1000us~2000us로 맵핑
    for (int i = 0; i < 50; ++i) { // 20ms 주기 (50Hz)
        gpio_write(servo_pin, HIGH);
        usleep(pulse_width);
        gpio_write(servo_pin, LOW);
        usleep(20000 - pulse_width);
    }
}
// 서보 모터 핀 설정
void setup_servo(int pin) {
    gpio_export(pin);
    gpio_direction(pin, "out");
}
// 서보 모터와 LED를 제어 스레드 함수
void* control_servo_and_led_thread(void* arg) {
    int* pins = (int*)arg;
    int servo_pin = pins[0];
    int led_pin = pins[1];

    // LED 켜기
    gpio_write(led_pin, HIGH);

    // 서보 모터 제어 
    set_servo_position(servo_pin, 0); // 0도
    delay_ms(2000);
    set_servo_position(servo_pin, 180); // 180도
    delay_ms(2000);
    set_servo_position(servo_pin, 0); // 원상태로
    delay_ms(2000);

    // 5초 대기 후 LED 끄기
    delay_ms(5000);
    gpio_write(led_pin, LOW);

    free(pins);
    return NULL;
}
// 주어진 비트마스크에 따라 서보 모터와 LED 제어(수신된 날씨정보 제어)
void control_servo_and_led(const char *bitmask) {
    pthread_t threads[3];
    int thread_count = 0;

    for (int i = 0; i < 3; i++) {
        int servo_pin = (i == 0) ? SERVO1 : (i == 1) ? SERVO2 : SERVO3;
        int led_pin = (i == 0) ? LED1 : (i == 1) ? LED2 : LED3;

        if (bitmask[i] == '1') {
            // 스레드 생성해 서보 모터와 LED를 제어
            int* pins = malloc(2 * sizeof(int));
            pins[0] = servo_pin;
            pins[1] = led_pin;
            pthread_create(&threads[thread_count++], NULL, control_servo_and_led_thread, (void*)pins);
        } else {
            // LED 끄기
            gpio_write(led_pin, LOW);
        }
    }

    // 모든 스레드 종료될 때까지 대기
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
}
// 모든 LED 켜거나 끄는 함수
void control_all_leds(int state) {
    gpio_write(LED1, state);
    gpio_write(LED2, state);
    gpio_write(LED3, state);
    gpio_write(EXTRA_LED, state);
}
// 추가 LED를 제어하는 함수(택배 인식 LED)
void control_extra_led(int state) {
    gpio_write(EXTRA_LED, state);
}
// GPIO 핀 설정 및 초기화 함수
void initialize_hardware() {
    // 서보 모터와 LED 핀을 출력 모드로 설정
    setup_servo(SERVO1);
    setup_servo(SERVO2);
    setup_servo(SERVO3);
    
    gpio_export(LED1);
    gpio_export(LED2);
    gpio_export(LED3);
    gpio_export(EXTRA_LED);

    gpio_direction(LED1, "out");
    gpio_direction(LED2, "out");
    gpio_direction(LED3, "out");
    gpio_direction(EXTRA_LED, "out");

    // 초기 상태 설정
    gpio_write(EXTRA_LED, LOW);
}
