#include "app_motor.h"

/**
* Function       Car_Run
* @author        liusen
* @date          2017.07.17    
* @brief         小车前进
* @param[in]     Speed  （0~7200） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_Run(int Speed)
{
	RS485_SetSpeed(&huart1,1,-Speed);
	RS485_SetSpeed(&huart1,2,Speed);
	RS485_SetSpeed(&huart1,3,-Speed);
	RS485_SetSpeed(&huart1,4,Speed);
	
//	LeftMotorPWM(Speed);		  
//	RightMotorPWM(Speed);	
	
}

/**
* Function       Car_Back
* @author        liusen
* @date          2017.07.17    
* @brief         小车后退
* @param[in]     Speed  （0~7200） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_Back(int Speed)
{
	RS485_SetSpeed(&huart1,1,Speed);
	RS485_SetSpeed(&huart1,2,-Speed);
	RS485_SetSpeed(&huart1,3,Speed);
	RS485_SetSpeed(&huart1,4,-Speed);
//	LeftMotorPWM(Speed);		  
//	RightMotorPWM(Speed);	
}

/**
* Function       Car_Left
* @author        liusen
* @date          2017.07.17    
* @brief         小车左转
* @param[in]     Speed  （0~7200） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_Left(int Speed)
{
	RS485_SetSpeed(&huart1,1,0);
	RS485_SetSpeed(&huart1,2,Speed);
	RS485_SetSpeed(&huart1,3,0);
	RS485_SetSpeed(&huart1,4,Speed);

//	LeftMotorPWM(0);		  
//	RightMotorPWM(Speed);
//	OLED_Clear();
//	OLED_ShowString(1,1,"left");
}

/**
* Function       Car_Right
* @author        liusen
* @date          2017.07.17    
* @brief         小车右转
* @param[in]     Speed  （0~7200） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_Right(int Speed)
{
	RS485_SetSpeed(&huart1,1,-Speed);
	RS485_SetSpeed(&huart1,2,0);
	RS485_SetSpeed(&huart1,3,-Speed);
	RS485_SetSpeed(&huart1,4,0);

//	LeftMotorPWM(Speed);		  
//	RightMotorPWM(0);
//	OLED_Clear();
//	OLED_ShowString(1,1,"Right");	
}

/**
* Function       Car_Stop
* @author        liusen
* @date          2017.07.17    
* @brief         小车刹车
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_Stop(void)
{
	RS485_SetSpeed(&huart1,1,0);
	RS485_SetSpeed(&huart1,2,0);
	RS485_SetSpeed(&huart1,3,0);
	RS485_SetSpeed(&huart1,4,0);

//	LeftMotorPWM(0);		  
//	RightMotorPWM(0);	
//	OLED_Clear();
//	OLED_ShowString(1,1,"Stop");
}

/**
* Function       Car_SpinLeft
* @author        liusen
* @date          2017.07.17    
* @brief         小车左旋
* @param[in]     LeftSpeed：左电机速度  RightSpeed：右电机速度 取值范围：（0~7200）
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_SpinLeft(int LeftSpeed, int RightSpeed)
{
	RS485_SetSpeed(&huart1,1,LeftSpeed);
	RS485_SetSpeed(&huart1,2,RightSpeed);
	RS485_SetSpeed(&huart1,3,LeftSpeed);
	RS485_SetSpeed(&huart1,4,RightSpeed);

//	LeftMotorPWM(LeftSpeed);		  
//	RightMotorPWM(RightSpeed);
//	OLED_Clear();
//	OLED_ShowString(1,1,"leftSpin");	
}

/**
* Function       Car_SpinRight
* @author        liusen
* @date          2017.07.17    
* @brief         小车右旋
* @param[in]     LeftSpeed：左电机速度  RightSpeed：右电机速度 取值范围：（0~7200）
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_SpinRight(int LeftSpeed, int RightSpeed)
{
	RS485_SetSpeed(&huart1,1,-LeftSpeed);
	RS485_SetSpeed(&huart1,2,-RightSpeed);
	RS485_SetSpeed(&huart1,3,-LeftSpeed);
	RS485_SetSpeed(&huart1,4,-RightSpeed);

//	LeftMotorPWM(LeftSpeed);		  
//	RightMotorPWM(RightSpeed);	
//	OLED_Clear();
//	OLED_ShowString(1,1,"RightSpin");
}

