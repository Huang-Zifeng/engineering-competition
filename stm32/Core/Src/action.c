#include "action.h"

float vx_limit = 200;
float vy_limit = 200;
float w_limit = 10;
extern bool action_flag;
extern bool stop_flag;
extern bool x_flag;
extern bool y_flag;
extern bool angle_flag;
extern bool total_flag;
extern float floatdata[6];
extern float target_posx, target_posy, target_angle;

void CleanData(UART_HandleTypeDef *huart)
{
 	for(int i=0;i<10;i++)
 	{
 		HAL_UART_Transmit_IT(huart, (uint8_t *)"ACT0", 5);
		HAL_Delay(10);
 	}
}
 
/**
 * @brief 
 * 
 * @param target 
 * @param current 
 * @return float range[-180，180]
 */
float w_loss(float target, float current)
{
  float loss = target - current;
  if(fabs(loss)<180)
    return loss;
  else if(loss>0)
    return loss - 360.0;
  else if(loss<0)
    return 360.0 + loss;
  else
	return 0;
}

//adjusting x coordinates, return delta x coord
float pid_x(float target)
{
	static float x_u, x_e1, x_e2;
	float Kp_x = 1;
	float Ti_x = 0.5;
	x_e1 = target + floatdata[4];
	x_u += Kp_x * (x_e1 - x_e2) + (Kp_x * Ti_x * x_e1) / T;  //Incremental PI controller
	x_e2 = x_e1;
	if(x_u >= vx_limit) 
		x_u = vx_limit;
	else if(x_u <= -vx_limit)
		x_u = -vx_limit;
	return x_u;
}

//adjusting y coordinates ,return delta y coord
float pid_y(float target)
{
	static float y_u, y_e1, y_e2;
	float Kp_y = 1;
	float Ti_y = 0.5;
	y_e1 = target - floatdata[3];
	y_u += Kp_y * (y_e1 - y_e2) + (Kp_y * Ti_y * y_e1) / T;
	y_e2 = y_e1;
	if(y_u >= vy_limit) 
		y_u = vy_limit;
	else if(y_u <= -vy_limit) 
		y_u = -vy_limit;
	return y_u;
}

//adjusting angle,return delta angle
float pid_angle(float target)
{
	static float angle_u, angle_e1, angle_e2;
	float Kp_a = 1.5;
	float Ti_a = 1;
	angle_e1 = w_loss(target, floatdata[0]);
	angle_u += Kp_a * (angle_e1 - angle_e2) + (Kp_a * Ti_a * angle_e1) /T;
	angle_e2 = angle_e1;
	if(angle_u >= w_limit) 
		angle_u = w_limit;
	else if(angle_u<=-w_limit) 
		angle_u = -w_limit;
	return angle_u;
}

void move2position(float pos_x, float pos_y, float angle)
{
	// time1 = millis();
	action_flag = true;
	target_posx = pos_x;
	target_posy = pos_y;
	target_angle = angle;
	x_flag = y_flag = angle_flag = total_flag = false;
    while (!total_flag);
	action_flag = false;
	car_stop();
}
