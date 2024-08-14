#ifndef MOTER_H
#define MOTER_H

void initialize_hardware();
void control_servo_and_led(const char *bitmask);
void control_all_leds(int state);
void control_extra_led(int state);

#endif
