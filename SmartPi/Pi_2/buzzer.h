#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define IN 0
#define OUT 1
#define PWM 0

#define LOW 0
#define HIGH 1
#define VALUE_MAX 256
#define DIRECTION_MAX 256

#define DEFAULT_PERIOD 2200000

// Using in sokect.c
void airplane_BUZZER(void);
void elise_BUZZER(void);
void school_BUZZER(void);