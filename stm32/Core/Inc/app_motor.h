#ifndef _APP_MOTOR_H
#define _APP_MOTOR_H

#include "main.h"
#include "motor.h"
#include "usart.h"

void Car_Run(int Speed);
void Car_Back(int Speed);
void Car_Left(int Speed);
void Car_Right(int Speed);
void Car_Stop(void);
void Car_SpinLeft(int LeftSpeed, int RightSpeed);
void Car_SpinRight(int LeftSpeed, int RightSpeed);

#endif
