#ifndef _MOTOR_H
#define _MOTOR_H

#include <stdint.h>
#include <string.h>
#include "usart.h"
#include "lcd.h"

void int2hex(int32_t data,uint8_t hexdata[4]);
void uint2hex(uint32_t data,uint8_t hexdata[2]);
void drive_wheels(UART_HandleTypeDef *huart, uint32_t intID, int32_t speed);
void drive_speed(int32_t x_speed,int32_t y_speed,int32_t z_speed);
void car_stop();

#endif
