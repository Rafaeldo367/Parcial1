/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define G7s_Pin GPIO_PIN_0
#define G7s_GPIO_Port GPIOA
#define F7s_Pin GPIO_PIN_1
#define F7s_GPIO_Port GPIOA
#define A7s_Pin GPIO_PIN_2
#define A7s_GPIO_Port GPIOA
#define B7s_Pin GPIO_PIN_3
#define B7s_GPIO_Port GPIOA
#define led1_Pin GPIO_PIN_6
#define led1_GPIO_Port GPIOA
#define led2_Pin GPIO_PIN_7
#define led2_GPIO_Port GPIOA
#define led3_Pin GPIO_PIN_0
#define led3_GPIO_Port GPIOB
#define led4_Pin GPIO_PIN_1
#define led4_GPIO_Port GPIOB
#define led5_Pin GPIO_PIN_2
#define led5_GPIO_Port GPIOB
#define buzzerPin_Pin GPIO_PIN_10
#define buzzerPin_GPIO_Port GPIOB
#define C7s_Pin GPIO_PIN_6
#define C7s_GPIO_Port GPIOB
#define D7s_Pin GPIO_PIN_7
#define D7s_GPIO_Port GPIOB
#define E7s_Pin GPIO_PIN_8
#define E7s_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
