/* USER CODE BEGIN Header */
/**
 * @file main.c
 * @author Huang Zifeng (Mitcher2022@outlook.com)
 * @brief Main body of the program.
 * @version 0.5
 * @date 2023-01-02
 *
 * @copyright Copyright (c) 2023
 *
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "motor.h"
#include "servo.h"
#include "app_motor.h"
#include "app_lineWalking.h"
#include "action.h"
#include "lcd.h"
#include "openmv.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define millis() HAL_GetTick()
#define PI acos(-1.0)
#define MAX_SIZE 20
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t time1 = 0;
uint32_t time2 = 0;
volatile float vx_action = 0, vy_action = 0, vx_car = 0, vy_car = 0, w = 0;
float target_posx, target_posy, target_angle;
float car_position[3] = {};
uint8_t grab_seq[3] = {1, 2, 3};
uint8_t color_seq[6] = {2, 3, 1, 1, 2, 3};

// flag variables
bool action_flag = false;
bool stop_flag = false;
bool x_flag = false;
bool y_flag = false;
bool angle_flag = false;
bool total_flag = false;
bool photo_flag = true;
bool key1 = false; // K1
bool key2 = false; // K0
bool color_flag = true;

// temporary variables
char x_str[MAX_SIZE] = {};
char y_str[MAX_SIZE] = {};
char angle_str[MAX_SIZE] = {};
char grab1_str[MAX_SIZE] = {};
char grab2_str[MAX_SIZE] = {};
char grab3_str[MAX_SIZE] = {};
uint8_t buffer_action[56];
uint8_t buffer_openmv;
uint8_t startOpenmv = 1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_UART4_Init();
  MX_USART3_UART_Init();
  MX_FSMC_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, 1);
  LCD_Init();
  LCD_Clear(RED);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_UART_Receive_IT(&huart3, (uint8_t *)buffer_action, sizeof(buffer_action));
  /*Servo Init*/
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	bool servo_init_flag = 1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == 0)
    {
      HAL_Delay(100);
      if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == 0)
      {
        key1 = !key1;
      }
    }
    if (key1)
    {
			drive_wheels(&huart1, 1, )
			key1 = !key1;
    }
    else
    {
      car_stop();
    }
		//car_position[0]=car_position[0]+1;
    sprintf(x_str, "X %.1f", car_position[0]);
    sprintf(y_str, "Y %.1f", car_position[1]);
    sprintf(angle_str, "Angle %.1f", car_position[2]);
    sprintf(grab1_str, "%u", grab_seq[0]);
    sprintf(grab2_str, "%u", grab_seq[1]);
    sprintf(grab3_str, "%u", grab_seq[2]);
    LCD_ShowString(5, 5, 240, 50, 32, x_str);
    LCD_ShowString(5, 55, 240, 50, 32, y_str);
    LCD_ShowString(5, 105, 240, 50, 32, angle_str);
    LCD_ShowString(5, 155, 240, 50, 32, grab1_str);
    LCD_ShowString(5, 205, 240, 50, 32, grab2_str);
    LCD_ShowString(5, 255, 240, 50, 32, grab3_str);
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    HAL_Delay(20);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

// interrupt of TIM
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim2)
  {
    if (action_flag)
    {
      if (fabs(car_position[0] - target_posx) < 6)
        x_flag = true;
      else
        x_flag = false;
      if (fabs(car_position[1] - target_posy) < 6)
        y_flag = true;
      else
        y_flag = false;
      if (fabs(w_loss(target_angle, car_position[2])) < 3)
        angle_flag = true;
      else
        angle_flag = false;

      total_flag = x_flag && y_flag && angle_flag;
      vx_action = pid_x(target_posx);
      vy_action = pid_y(target_posy);
      w = pid_angle(target_angle);
      vx_car = vx_action * cos(car_position[2] * PI / 180) + vy_action * sin(car_position[2] * PI / 180);
      vy_car = vy_action * cos(car_position[2] * PI / 180) - vx_action * sin(car_position[2] * PI / 180);
    }
    drive_speed(vx_car, vy_car, w);
  }
}
// interrupt of serials

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
