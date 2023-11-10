/**
 * @file openmv.c
 * @author 黄梓峰
 * @brief 实现与openmv之间进行通信的函数
 * @version 0.1
 * @date 2022-08-07
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "openmv.h"

extern uint8_t grab_seq[3];
extern uint8_t color_seq[6];
extern bool color_flag;
void OpenMV_Data_Receive(int16_t com_data)
{
    uint8_t i;
    static uint8_t RxCounter1 = 0; //计数
    static uint16_t RxBuffer1[10] = {0};
    static uint8_t RxState = 0;
    if (RxState == 0 && com_data == 0x2C) // 0x2C帧头
    {
		RxState = 1;
        RxBuffer1[RxCounter1++] = com_data;  
    }

    else if (RxState == 1 && com_data == 0x12) // 0x12帧头
    {
        RxState = 2;
        RxBuffer1[RxCounter1++] = com_data;
    }
    else if (RxState == 2)//color1, grab_seq[0]
    {
        RxBuffer1[RxCounter1++] = com_data;
        RxState = 3;        
    }
	else if(RxState == 3)//color2, grab_seq[1]
	{
		RxBuffer1[RxCounter1++] = com_data;
		RxState = 4;
	}
	else if(RxState == 4)//color3, grab_seq[2]
	{
		RxBuffer1[RxCounter1++] = com_data;
		RxState = 5;
	}
	else if(RxState == 5)//color4, grab_seq[3]
	{
		RxBuffer1[RxCounter1++] = com_data;
		RxState = 6;
	}
    else if(RxState == 6)//color5
	{
		RxBuffer1[RxCounter1++] = com_data;
		RxState = 7;
	}
    else if(RxState == 7)//color6
	{
		RxBuffer1[RxCounter1++] = com_data;
		RxState = 8;
	}
	else if(RxState == 8)
    { 
        if(com_data == 0x5B)//0x5B帧尾
        {
            RxState = 0;
            RxBuffer1[RxCounter1] = com_data;
            if(color_flag){
                color_seq[0] = RxBuffer1[RxCounter1-6];
								color_seq[1] = RxBuffer1[RxCounter1-5];
								color_seq[2] = RxBuffer1[RxCounter1-4];
								color_seq[3] = RxBuffer1[RxCounter1-3];
                color_seq[4] = RxBuffer1[RxCounter1-2];
								color_seq[5] = RxBuffer1[RxCounter1-1];
                color_flag = false;
            }
            else{
                grab_seq[0] = RxBuffer1[RxCounter1-6];
                grab_seq[1] = RxBuffer1[RxCounter1-5];
                grab_seq[2] = RxBuffer1[RxCounter1-4];
            }
        }
        else
        {
            RxState = 0;
            for(i=0;i<10;i++)
            {
                RxBuffer1[i]=0x00;
            }
        }	
    }	
}
