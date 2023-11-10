#include "motor.h"

extern float vx_car, vy_car, w; 

void int2hex(int32_t data,uint8_t hexdata[4])
{
	union
	{
		int32_t a;
		uint8_t hexs[4];
	}thing;
	thing.a = data;
	memcpy(hexdata,thing.hexs,4);
}

void uint2hex(uint32_t data,uint8_t hexdata[2])
{
	union
	{
		uint32_t a;
		uint8_t hexs[2];
	}thing;
	thing.a=data;
	memcpy(hexdata,thing.hexs,2);
}

void drive_wheels(UART_HandleTypeDef *huart, uint32_t intID, int32_t speed)
{
	int32_t w=(speed*100*180)/(3.14*20);
	uint8_t data[10];
	data[0]=0x3E;
	data[1]=0xA2;
	data[2] = intID;
	data[3]=0x04;
	data[4] = data[0] + data[1] + data[2] + data[3];
	int2hex(w,data+5*sizeof(uint8_t));
	data[9] = data[5] + data[6] + data[7] + data[8];
	HAL_UART_Transmit_IT(huart,(uint8_t *)data,sizeof(data));
	//LCD_ShowString(35,205,240,50,32, "start");
	HAL_Delay(2) ;
	//LCD_ShowString(85,205,240,50,32, "finish");
}

void drive_speed(int32_t x_speed, int32_t y_speed, int32_t z_speed)
{
	int32_t HALF_OF_H = 85; // 85 millimeter
	int32_t HALF_OF_W = 121; // 121 millimeter
	int32_t RADIUS = 50; //millimeter
	int32_t targetVelocity1, targetVelocity2, targetVelocity3, targetVelocity4;
	int32_t car_size = 20; // HALF_OF_H+HALF_OF_W
	
	targetVelocity1 = x_speed - y_speed - z_speed*car_size;
	targetVelocity2 = x_speed + y_speed - z_speed*car_size;
	targetVelocity3 = x_speed - y_speed + z_speed*car_size;
	targetVelocity4 = x_speed + y_speed + z_speed*car_size;
	
	drive_wheels(&huart1, 2, -targetVelocity1);

	drive_wheels(&huart1, 4, -targetVelocity2);

	drive_wheels(&huart1, 1, targetVelocity3);

	drive_wheels(&huart1, 3, targetVelocity4);
}

void car_stop()
{
	vx_car = vy_car = w = 0;
}




