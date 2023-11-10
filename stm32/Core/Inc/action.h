#ifndef ACTION_H
#define ACTION_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "motor.h"

#define T 20.0
void CleanData(UART_HandleTypeDef *huart);
float w_loss(float target,float current);
float pid_x(float target);
float pid_y(float target);
float pid_angle(float target);
void move2position(float pos_x, float pos_y, float angle);

#endif
