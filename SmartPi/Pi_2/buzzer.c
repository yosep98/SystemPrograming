#include "buzzer.h"
#include <math.h>

#define BUFFER_MAX 3

static int PWMExport(int pwmnum) {

  char buffer[BUFFER_MAX];
  int fd, byte;

  // TODO: Enter the export path.
  fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
  if (-1 == fd) {
    fprintf(stderr, "Failed to open export for export!\n");
    return (-1);
  }

  byte = snprintf(buffer, BUFFER_MAX, "%d", pwmnum);
  write(fd, buffer, byte);
  close(fd);

  sleep(1);

  return (0);
}

static int PWMEnable(int pwmnum) {
  static const char s_enable_str[] = "1";

  char path[DIRECTION_MAX];
  int fd;

  // TODO: Enter the enable path.
  snprintf(path, DIRECTION_MAX, "/sys/class/pwm/pwmchip0/pwm0/enable", pwmnum);
  fd = open(path, O_WRONLY);
  if (-1 == fd) {
    fprintf(stderr, "Failed to open in enable!\n");
    return -1;
  }

  write(fd, s_enable_str, strlen(s_enable_str));
  close(fd);

  return (0);
}

static int PWMDisable(int pwmnum) {
  static const char s_enable_str[] = "0";

  char path[DIRECTION_MAX];
  int fd;

  // TODO: Enter the enable path.
  snprintf(path, DIRECTION_MAX, "/sys/class/pwm/pwmchip0/pwm0/enable", pwmnum);
  fd = open(path, O_WRONLY);
  if (-1 == fd) {
    fprintf(stderr, "Failed to open in enable!\n");
    return -1;
  }

  write(fd, s_enable_str, strlen(s_enable_str));
  close(fd);

  return (0);
}

static int PWMWritePeriod(int pwmnum, int value) {
  char s_value_str[VALUE_MAX];
  char path[VALUE_MAX];
  int fd, byte;

  // TODO: Enter the period path.
  snprintf(path, VALUE_MAX, "/sys/class/pwm/pwmchip0/pwm0/period", pwmnum);
  fd = open(path, O_WRONLY);
  if (-1 == fd) {
    fprintf(stderr, "Failed to open in period!\n");
    return (-1);
  }
  byte = snprintf(s_value_str, VALUE_MAX, "%d", value);

  if (-1 == write(fd, s_value_str, byte)) {
    //fprintf(stderr, "Failed to write value in period!\n");
    close(fd);
    return -1;
  }
  close(fd);

  return (0);
}

static int PWMWriteDutyCycle(int pwmnum, int value) {
  char s_value_str[VALUE_MAX];
  char path[VALUE_MAX];
  int fd, byte;

  // TODO: Enter the duty_cycle path.
  snprintf(path, VALUE_MAX, "/sys/class/pwm/pwmchip0/pwm0/duty_cycle", pwmnum);
  fd = open(path, O_WRONLY);
  if (-1 == fd) {
    fprintf(stderr, "Failed to open in duty cycle!\n");
    return (-1);
  }
  byte = snprintf(s_value_str, VALUE_MAX, "%d", value);

  if (-1 == write(fd, s_value_str, byte)) {
    fprintf(stderr, "Failed to write value in duty cycle!\n");
    close(fd);
    return -1;
  }
  close(fd);

  return (0);
}

//라
void A3(){
    PWMWritePeriod(PWM, DEFAULT_PERIOD);
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD * 0.9)); // 90% volume
}

//라#
void A3_shap(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 1.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 1.0) * 0.9)); // 90% volume
}

//시
void B3(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 2.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 2.0) * 0.9)); // 90% volume
}

//도
void C3(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 3.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 3.0) * 0.9)); // 90% volume
}

//도#
void C3_shap(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 4.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 4.0) * 0.9)); // 90% volume
}

//레
void D3(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 5.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 5.0) * 0.9)); // 90% volume
}

//레#
void D3_shap(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 6.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 6.0) * 0.9)); // 90% volume
}

//미
void E3(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 7.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 7.0) * 0.9)); // 90% volume
}

//파
void F3(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 8.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 8.0) * 0.9)); // 90% volume
}

//파#
void F3_shap(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 9.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 9.0) * 0.9)); // 90% volume
}

//솔
void G3(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 10.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 10.0) * 0.9)); // 90% volume
}

//솔#
void G3_shap(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 11.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 11.0) * 0.9)); // 90% volume
}

//높은 라
void A4(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 12.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 12.0) * 0.9)); // 90% volume
}

//높은 라#
void A4_shap(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 13.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 13.0) * 0.9)); // 90% volume
}

//높은 시
void B4(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 14.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 14.0) * 0.9)); // 90% volume
}

//높은 도
void C4(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 15.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 15.0) * 0.9)); // 90% volume
}

//높은 도#
void C4_shap(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 16.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 16.0) * 0.9)); // 90% volume
}

//높은 레
void D4(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 17.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 17.0) * 0.9)); // 90% volume
}

//높은 레#
void D4_shap(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 18.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 18.0) * 0.9)); // 90% volume
}

//높은 미
void E4(){
    PWMWritePeriod(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 19.0)));
    PWMWriteDutyCycle(PWM, (int)(DEFAULT_PERIOD / pow(1.06, 19.0) * 0.9)); // 90% volume
}

//Turn off the buzzer a few time
void duration(){
    PWMWriteDutyCycle(PWM, 0); // Turn off
    PWMWritePeriod(PWM, 0);
    usleep(10000);
}

//떴다떴다 비행기
void airplane_BUZZER(void) {

  PWMDisable(PWM); // Disable the PWM

    PWMExport(PWM); // Export the PWM
    PWMWritePeriod(PWM, DEFAULT_PERIOD); // Set the period to DEFAULT_PERIOD
    PWMWriteDutyCycle(PWM, 0); // Set initial duty cycle to 0
    PWMEnable(PWM); // Enable the PWM
    
    E3();
    usleep(700000); // 0.7 seconds
    duration();

    D3();
    usleep(300000); // 0.3 seconds
    duration();

    C3();
    usleep(450000); // 0.45 second
    duration();

    D3();
    usleep(500000); // 0.5 second
    duration();

    E3();
    usleep(500000); // 0.5 seconds
    duration();

    E3();
    usleep(500000); // 0.5 seconds
    duration();
    
    E3();
    usleep(500000); // 0.5 seconds
    duration();
    
    PWMDisable(PWM); // Disable the PWM
}

//엘리제를 위하여
void elise_BUZZER(void){
  PWMDisable(PWM); // Disable the PWM

  PWMExport(PWM); // Export the PWM
  PWMWritePeriod(PWM, DEFAULT_PERIOD); // Set the period to DEFAULT_PERIOD
  PWMWriteDutyCycle(PWM, 0); // Set initial duty cycle to 0
  PWMEnable(PWM); // Enable the PWM

  E4();
  usleep(250000); // 0.25 seconds
  duration();

  D4_shap();
  usleep(250000); // 0.25 seconds
  duration();

  E4();
  usleep(250000); // 0.25 seconds
  duration();

  D4_shap();
  usleep(250000); // 0.25 seconds
  duration();

  E4();
  usleep(250000); // 0.25 seconds
  duration();

  B4();
  usleep(250000); // 0.25 seconds
  duration();

  D4();
  usleep(250000); // 0.25 seconds
  duration();

  C4();
  usleep(250000); // 0.25 seconds
  duration();

  A4();
  usleep(400000); // 0.4 seconds
  duration();
    
  PWMDisable(PWM); // Disable the PWM

}

//학교종이 땡땡땡
void school_BUZZER(void){

  PWMDisable(PWM); // Disable the PWM

  PWMExport(PWM); // Export the PWM
  PWMWritePeriod(PWM, DEFAULT_PERIOD); // Set the period to DEFAULT_PERIOD
  PWMWriteDutyCycle(PWM, 0); // Set initial duty cycle to 0
  PWMEnable(PWM); // Enable the PWM

  G3();
  usleep(500000); // 0.5 seconds
  duration();

  G3();
  usleep(500000); // 0.5 seconds
  duration();

  A4();
  usleep(500000); // 0.5 seconds
  duration();
  
  A4();
  usleep(500000); // 0.5 seconds
  duration();
  
  G3();
  usleep(500000); // 0.5 seconds
  duration();

  G3();
  usleep(500000); // 0.5 seconds
  duration();

  E3();
  usleep(1000000); // 1 second
  duration();
    
  PWMDisable(PWM); // Disable the PWM
}