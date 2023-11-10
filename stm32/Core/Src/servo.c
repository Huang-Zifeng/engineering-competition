#include "servo.h"

static float angle1=((SERVOR_INIT_1/ARR)-0.025f)*1800;

void int162hex(int16_t data,uint8_t hexdata[2])
{
	union
	{
		int16_t a;
		uint8_t hexs[2];
	}thing;
	thing.a = data;
	memcpy(hexdata,thing.hexs,2);
}

void int82hex(int8_t data,uint8_t hexdata[1])
{
	union
	{
		int8_t a;
		uint8_t hexs[1];
	}thing;
	thing.a = data;
	memcpy(hexdata,thing.hexs,1);
}

void uint162hex(uint16_t data,uint8_t hexdata[2])
{
	union
	{
		uint16_t a;
		uint8_t hexs[2];
	}thing;
	thing.a=data;
	memcpy(hexdata,thing.hexs,2);
}

void SMS_SetAction(UART_HandleTypeDef *huart,uint32_t intID, uint16_t Position, int16_t Speed, int8_t a)
{
	uint8_t data[14];
	data[0]=0xFF;
  	data[1]=0xFF;
	data[2]=intID;
	data[3]=0x0A;
	data[4]=0x03;
	data[5]=0x29;
	int82hex(a,data+6*sizeof(uint8_t));
	uint162hex(Position,data+7*sizeof(uint8_t));
	data[9]=0x00;
	data[10]=0x00;
	int162hex(Speed,data+11*sizeof(uint8_t));
	data[13]=255-(data[2]+data[3]+data[4]+data[5]+data[6]+data[7]+data[8]+data[9]+data[10]+data[11]+data[12])%256;
	HAL_UART_Transmit(huart,(uint8_t*)data,(uint16_t)sizeof(data),0xffff);
}

void Servo_Angle_180(int id, float angle, int time)
{
	int temp1 = (int)((angle/1800.0f+0.025f)*ARR);
	int temp2;
	switch (id)
	{
	case 1:
		//__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_2,temp1);
		temp2 = (int)((angle1/1800.0f+0.025f)*ARR);
		if(temp1>temp2)
		{
			int counter = (temp1-temp2)/SERVO_STEP_SIZE;
			int bias = (temp1-temp2)%SERVO_STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2+=SERVO_STEP_SIZE;
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,temp2);
				HAL_Delay(time);			
			}
			temp2+=bias;
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,temp2);
		}
		else
		{
			int counter = (temp2-temp1)/SERVO_STEP_SIZE;
			int bias = (temp2-temp1)%SERVO_STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2-=SERVO_STEP_SIZE;
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,temp2);
				HAL_Delay(time);
			}
				temp2-=bias;
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,temp2);
			}
		angle1=angle;
		break;
	case 2:
		//__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_2,temp1);
		temp2 = (int)((angle1/1800.0f+0.025f)*ARR);
		if(temp1>temp2)
		{
			int counter = (temp1-temp2)/SERVO_STEP_SIZE;
			int bias = (temp1-temp2)%SERVO_STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2+=SERVO_STEP_SIZE;
				__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,temp2);
				HAL_Delay(time);			
			}
			temp2+=bias;
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,temp2);
		}
		else
		{
			int counter = (temp2-temp1)/SERVO_STEP_SIZE;
			int bias = (temp2-temp1)%SERVO_STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2-=SERVO_STEP_SIZE;
				__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,temp2);
				HAL_Delay(time);
			}
			temp2-=bias;
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,temp2);
		}
		angle1=angle;
		break;
	default:
		break;
	}
}
/**
 * @brief control 360-dgree servo
 * 
 * @param id the id of servo, eg:1, 2, 3...
 * @param speed from -10 to 10
 * @param time ms
 */
void Servo_Angle_360(int id, int speed, int time)
{
	int pulse_width = ((1.5-0.1*speed)/20)*ARR; 
	switch (id)
	{
	case 1:
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, pulse_width);
		HAL_Delay(time);
		break;
	
	default:
		break;
	}
}

void Load(void)
{
	SMS_SetAction(&huart4,1,1350,15,0);//2531
	HAL_Delay(500);
	SMS_SetAction(&huart4,2,2500,15,0);//2116
	SMS_SetAction(&huart4,3,2210,21,0);
	SMS_SetAction(&huart4,4,2600,15,0);
	HAL_Delay(1500);

	SMS_SetAction(&huart4,1,850,15,0);
	SMS_SetAction(&huart4,4,1300,15,0);
	HAL_Delay(500);
	
	SMS_SetAction(&huart4, 3, 1647, 15, 0);
	HAL_Delay(1500);
	Servo_Angle_180(2,20,10);
	HAL_Delay(2000);
	SMS_SetAction(&huart4,4,2600,15,0);
	HAL_Delay(1000);
	Servo_Init();
}

void Grab(uint16_t seq)
{
	switch (seq)
	{
	case 1:		//left
		//Servo_Angle_180(1, 110, 10);
		SMS_SetAction(&huart4,1,2450,15,0);//2531
		SMS_SetAction(&huart4,2,1840,15,0);//2116
		SMS_SetAction(&huart4,3,2740,15,0);
		Servo_Angle_180(2,20,10);
		HAL_Delay(200);
		SMS_SetAction(&huart4,4,2870,15,0);
		HAL_Delay(1000);
		Servo_Angle_180(2,-5,10);
		HAL_Delay(1000);

		Load();
		break;

	case 2:		//middle
		SMS_SetAction(&huart4,1,2400,15,0);//2531
		SMS_SetAction(&huart4,2,1950,15,0);//2116
		SMS_SetAction(&huart4,3,2785,15,0);
		Servo_Angle_180(2,20,10);
		HAL_Delay(200);
		SMS_SetAction(&huart4,4,3050,15,0);
		HAL_Delay(1000);
		Servo_Angle_180(2,-5,10);
		HAL_Delay(1000);
		Load();
		break;

	case 3:		//right
		SMS_SetAction(&huart4,1,2450,20,0);//2531
		SMS_SetAction(&huart4,2,1840,20,0);//2116
		SMS_SetAction(&huart4,3,2740,20,0);
		Servo_Angle_180(2,20,10);
		HAL_Delay(150);
		SMS_SetAction(&huart4,4,3280,20,0);
		HAL_Delay(1000); 
		Servo_Angle_180(2,-5,10);
		HAL_Delay(1000);
		Load();
		break;
	
	case 4:
		SMS_SetAction(&huart4,3,3500,20,0);
		HAL_Delay(1000);
		SMS_SetAction(&huart4,1,2800,15,0);
		SMS_SetAction(&huart4,2,2200,15,0);//2116
		Servo_Angle_180(2,20,10);
		SMS_SetAction(&huart4,4,2880,15,0);
		HAL_Delay(3000);
		SMS_SetAction(&huart4,2,1722,10,0);//2116
		SMS_SetAction(&huart4,3,3020,10,0);
		HAL_Delay(1000);
		Servo_Angle_180(2,-5,10);
		HAL_Delay(1000);
		SMS_SetAction(&huart4,3,3500,13,0);
		SMS_SetAction(&huart4,2,2200,10,0);//2116
		HAL_Delay(2000);
	
		SMS_SetAction(&huart4,1,1350,15,0);//2531
		//HAL_Delay(300);
		SMS_SetAction(&huart4,2,2500,15,0);//2116
		SMS_SetAction(&huart4,3,2210,21,0);
		SMS_SetAction(&huart4,4,2600,15,0);
		HAL_Delay(1500);

		SMS_SetAction(&huart4,1,850,15,0);
		SMS_SetAction(&huart4,4,1300,15,0);
		HAL_Delay(500);
	  
		SMS_SetAction(&huart4, 3, 1647, 15, 0);
		HAL_Delay(1800);
		Servo_Angle_180(2,25,10);
		HAL_Delay(2000);
		SMS_SetAction(&huart4,4,2600,15,0);
		HAL_Delay(1000);
		Servo_Init();
		break;
	case 5:
		break;
	case 6:
		break;
	default:
		break;
	}
}

void Servo_Init()
{
	SMS_SetAction(&huart4,1,1350,30,0);//2531
  	SMS_SetAction(&huart4,2,2500,30,0);//2116
	SMS_SetAction(&huart4,4,2600,30,0);
	HAL_Delay(600);
	SMS_SetAction(&huart4,3,1700,40,0);
	Servo_Angle_180(2,-5,10);
	HAL_Delay(400);
}

void Put_Down_1(uint16_t seq)
{
	switch (seq)
	{
	case 1:
		//move2position(890, -1824, 270);//green
		SMS_SetAction(&huart4,1,900,15,0);
		SMS_SetAction(&huart4,2,2527,15,0);
		SMS_SetAction(&huart4,3,2058,15,0);
		SMS_SetAction(&huart4,4,1300,15,0);
	
		Servo_Angle_180(2,20,10);
	
		HAL_Delay(2000);
		SMS_SetAction(&huart4,3,1595,15,0);
		HAL_Delay(2000);
		Servo_Angle_180(2,-5,10);
		HAL_Delay(600);
		SMS_SetAction(&huart4,3,2000,15,0);//2531
		HAL_Delay(1000);
		Servo_Init();
		
		SMS_SetAction(&huart4,1,2815,15,0);
		SMS_SetAction(&huart4,2,1721,15,0);
		SMS_SetAction(&huart4,3,2985,15,0);
		
		HAL_Delay(300);
		SMS_SetAction(&huart4,4,2750,10,0);
		HAL_Delay(5000);
		Servo_Angle_180(2,20,10);
		HAL_Delay(200);
		Servo_Init();
		break;
	case 2:
			//move2position(890, -1824, 270);//green
		SMS_SetAction(&huart4,1,900,15,0);
		SMS_SetAction(&huart4,2,2527,15,0);
		SMS_SetAction(&huart4,3,2058,15,0);
		SMS_SetAction(&huart4,4,1300,15,0);
	
		Servo_Angle_180(2,20,10);
	
		HAL_Delay(2000);
		SMS_SetAction(&huart4,3,1595,15,0);
		HAL_Delay(2000);
		Servo_Angle_180(2,-5,10);
		HAL_Delay(600);
		SMS_SetAction(&huart4,3,2000,15,0);//2531
		HAL_Delay(1000);
		Servo_Init();
		
		SMS_SetAction(&huart4,1,2800,10,0);
		SMS_SetAction(&huart4,2,1765,10,0);
		SMS_SetAction(&huart4,3,3000,10,0);
		
		HAL_Delay(300);
		SMS_SetAction(&huart4,4,2988,10,0);
		HAL_Delay(3500);
		Servo_Angle_180(2,20,10);
		HAL_Delay(200);
		Servo_Init();
		break;
	case 3:
		//move2position(890, -1824, 270);//green
		SMS_SetAction(&huart4,1,900,15,0);
		SMS_SetAction(&huart4,2,2527,15,0);
		SMS_SetAction(&huart4,3,2058,15,0);
		SMS_SetAction(&huart4,4,1300,15,0);
	
		Servo_Angle_180(2,20,10);
	
		HAL_Delay(2000);
		SMS_SetAction(&huart4,3,1595,15,0);
		HAL_Delay(1200);
		Servo_Angle_180(2,-5,10);
		HAL_Delay(600);
		SMS_SetAction(&huart4,3,2000,15,0);//2531
		HAL_Delay(1000);
		Servo_Init();
		
		SMS_SetAction(&huart4,1,2815,10,0);
		SMS_SetAction(&huart4,2,1713,10,0);
		SMS_SetAction(&huart4,3,2981,10,0);
		
		HAL_Delay(300);
		SMS_SetAction(&huart4,4,3230,10,0);
		HAL_Delay(3000);
		Servo_Angle_180(2,20,10);
		HAL_Delay(200);
		Servo_Init();
		break;
	default:
		break;
	}		
}

void Put_Down_2(uint16_t seq)
{
 switch (seq)
 {
 case 1:
		//move2position(1805, -1047, 0);
		SMS_SetAction(&huart4,1,900,34,0);
		SMS_SetAction(&huart4,2,2527,34,0);
		SMS_SetAction(&huart4,3,2058,34,0);
		SMS_SetAction(&huart4,4,1300,34,0);
	 
		Servo_Angle_180(2,20,10);
	 
		HAL_Delay(650);
		SMS_SetAction(&huart4,3,1595,35,0);
		HAL_Delay(500);
		Servo_Angle_180(2,-5,10);
		HAL_Delay(350);
		SMS_SetAction(&huart4,3,2000,35,0);//2531
		HAL_Delay(300);
		
		SMS_SetAction(&huart4,1,1350,32,0);//2531
		SMS_SetAction(&huart4,4,2600,25,0);
		HAL_Delay(500);
		
		SMS_SetAction(&huart4,1,2745,12,0);
		SMS_SetAction(&huart4,2,1722,12,0);
		SMS_SetAction(&huart4,3,2960,12,0);
		
		HAL_Delay(200);
		SMS_SetAction(&huart4,4,2762,12,0);
		HAL_Delay(2400);
		Servo_Angle_180(2,20,10);
		HAL_Delay(200);
		Servo_Init();
		break;
 case 2:
		SMS_SetAction(&huart4,1,900,37,0);
		SMS_SetAction(&huart4,2,2527,37,0);
		SMS_SetAction(&huart4,3,2058,37,0);
		SMS_SetAction(&huart4,4,1300,37,0);
	 
		Servo_Angle_180(2,20,10);
	 
		HAL_Delay(650);
		SMS_SetAction(&huart4,3,1595,35,0);
		HAL_Delay(500);
		Servo_Angle_180(2,-5,10);
		HAL_Delay(350);
		SMS_SetAction(&huart4,3,2000,40,0);//2531
		HAL_Delay(300);
		
		SMS_SetAction(&huart4,1,1350,32,0);//2531
		SMS_SetAction(&huart4,4,2600,25,0);
		HAL_Delay(500);
		
		SMS_SetAction(&huart4,1,2715,12,0);
		SMS_SetAction(&huart4,2,1825,12,0);
		SMS_SetAction(&huart4,3,2948,17,0);
		
		HAL_Delay(200);
		SMS_SetAction(&huart4,4,2988,12,0);
		HAL_Delay(2400);
		Servo_Angle_180(2,20,10);
		HAL_Delay(200);
		Servo_Init();
		break;
 case 3:
		//move2position(1805, -1047, 0);
		SMS_SetAction(&huart4,1,900,37,0);
		SMS_SetAction(&huart4,2,2527,37,0);
		SMS_SetAction(&huart4,3,2058,37,0);
		SMS_SetAction(&huart4,4,1300,37,0);
	 
		Servo_Angle_180(2,20,10);
	 
		HAL_Delay(650);
		SMS_SetAction(&huart4,3,1595,35,0);
		HAL_Delay(500);
		Servo_Angle_180(2,-5,10);
		HAL_Delay(350);
		SMS_SetAction(&huart4,3,2000,40,0);//2531
		HAL_Delay(300);
		
		SMS_SetAction(&huart4,1,1350,32,0);//2531
		SMS_SetAction(&huart4,4,2600,25,0);
		HAL_Delay(500);
		
		SMS_SetAction(&huart4,1,2745,12,0);
		SMS_SetAction(&huart4,2,1722,12,0);
		SMS_SetAction(&huart4,3,2960,17,0);
		
		HAL_Delay(200);
		SMS_SetAction(&huart4,4,3200,12,0);
		HAL_Delay(2400);
		Servo_Angle_180(2,20,10);
		HAL_Delay(200);
		Servo_Init();
		break;
 case 4:
		SMS_SetAction(&huart4,1,900,37,0);
		SMS_SetAction(&huart4,2,2527,37,0);
		SMS_SetAction(&huart4,3,2058,37,0);
		SMS_SetAction(&huart4,4,1300,37,0);
	 
		Servo_Angle_180(2,20,10);
	 
		HAL_Delay(650);
		SMS_SetAction(&huart4,3,1595,35,0);
		HAL_Delay(500);
		Servo_Angle_180(2,-5,10);
		HAL_Delay(350);
		SMS_SetAction(&huart4,3,2000,40,0);//2531
		HAL_Delay(300);
		
		SMS_SetAction(&huart4,1,1350,32,0);//2531
		SMS_SetAction(&huart4,4,2600,25,0);
		HAL_Delay(500);
		
		SMS_SetAction(&huart4,1,2520,12,0);
		SMS_SetAction(&huart4,2,1827,12,0);
		SMS_SetAction(&huart4,3,2782,17,0);
		
		HAL_Delay(200);
		SMS_SetAction(&huart4,4,2762,12,0);
		HAL_Delay(2400);
		Servo_Angle_180(2,20,10);
		HAL_Delay(200);
		Servo_Init();
		break;
	case 5:
		SMS_SetAction(&huart4,1,900,37,0);
		SMS_SetAction(&huart4,2,2527,37,0);
		SMS_SetAction(&huart4,3,2058,37,0);
		SMS_SetAction(&huart4,4,1300,37,0);
	 
		Servo_Angle_180(2,20,10);
	 
		HAL_Delay(650);
		SMS_SetAction(&huart4,3,1595,35,0);
		HAL_Delay(500);
		Servo_Angle_180(2,-5,10);
		HAL_Delay(350);
		SMS_SetAction(&huart4,3,2000,40,0);//2531
		HAL_Delay(300);
		
		SMS_SetAction(&huart4,1,1350,32,0);//2531
		SMS_SetAction(&huart4,4,2600,25,0);
		HAL_Delay(500);
		
		SMS_SetAction(&huart4,1,2500,12,0);
		SMS_SetAction(&huart4,2,1915,12,0);
		SMS_SetAction(&huart4,3,2810,17,0);
		
		HAL_Delay(200);
		SMS_SetAction(&huart4,4,2988,12,0);
		HAL_Delay(2400);
		
		Servo_Angle_180(2,20,10);
		HAL_Delay(200);
		Servo_Init();
		break;
	case 6:
		SMS_SetAction(&huart4,1,900,37,0);
		SMS_SetAction(&huart4,2,2527,37,0);
		SMS_SetAction(&huart4,3,2058,37,0);
		SMS_SetAction(&huart4,4,1300,37,0);
	 
		Servo_Angle_180(2,20,10);
	 
		HAL_Delay(650);
		SMS_SetAction(&huart4,3,1595,35,0);
		HAL_Delay(500);
		Servo_Angle_180(2,-5,10);
		HAL_Delay(350);
		SMS_SetAction(&huart4,3,2000,40,0);//2531
		HAL_Delay(300);
		
		SMS_SetAction(&huart4,1,1350,32,0);//2531
		SMS_SetAction(&huart4,4,2600,25,0);
		HAL_Delay(500);
		
		SMS_SetAction(&huart4,1,2520,12,0);
		SMS_SetAction(&huart4,2,1827,12,0);
		SMS_SetAction(&huart4,3,2782,17,0);
		
		HAL_Delay(200);
		SMS_SetAction(&huart4,4,3200,12,0);
		HAL_Delay(2400);
		Servo_Angle_180(2,20,10);
		HAL_Delay(200);
		Servo_Init();
		break;
		break;
	default:
		break;
	}
}





