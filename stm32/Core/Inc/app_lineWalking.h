#ifndef _APP_LINEWALKING_H
#define _APP_LINEWALKING_H


	
#include "main.h"

#define     LOW          0
#define     HIGH         1


#define LineWalk_L1_PIN		GPIO_PIN_6
#define LineWalk_L2_PIN		GPIO_PIN_5
#define LineWalk_R1_PIN		GPIO_PIN_7
#define LineWalk_R2_PIN		GPIO_PIN_4

#define LineWalk_L1_PORT	GPIOA
#define LineWalk_L2_PORT	GPIOA
#define LineWalk_R1_PORT	GPIOA
#define LineWalk_R2_PORT	GPIOA

void bsp_GetLineWalking(int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2);
//void app_LineWalking(void);

#endif
