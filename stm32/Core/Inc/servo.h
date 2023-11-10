#ifndef _SERVO_H
#define _SERVO_H

#include <stdint.h>
#include <string.h>
#include "usart.h"
#include "action.h"
#include "tim.h"

#define ARR 20000.0f
#define SERVO_STEP_SIZE 50
#define SERVOR_INIT_1 1500

void int162hex(int16_t data, uint8_t hexdata[2]);
void uint162hex(uint16_t data, uint8_t hexdata[2]);
void int82hex(int8_t data, uint8_t hexdata[1]);
void SMS_SetAction(UART_HandleTypeDef *huart,uint32_t intID, uint16_t Position, int16_t Speed, int8_t a);
void Servo_Angle_180(int id, float angle, int time);
void Servo_Angle_360(int id, int speed, int time);
void Servo_Init(); 
void Grab(uint16_t seq);
void Put_Down_1(uint16_t seq);
void Put_Down_2(uint16_t seq);


#endif
