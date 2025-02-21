/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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
#define UWB1_IRQ_Pin GPIO_PIN_0
#define UWB1_IRQ_GPIO_Port GPIOB
#define UWB1_IRQ_EXTI_IRQn EXTI0_IRQn
#define UWB1_RESET_Pin GPIO_PIN_1
#define UWB1_RESET_GPIO_Port GPIOB
#define UWB1_CSn_Pin GPIO_PIN_2
#define UWB1_CSn_GPIO_Port GPIOB
#define UWB1_WAKEUP_Pin GPIO_PIN_7
#define UWB1_WAKEUP_GPIO_Port GPIOE
#define UWB2_IRQ_Pin GPIO_PIN_9
#define UWB2_IRQ_GPIO_Port GPIOE
#define UWB2_IRQ_EXTI_IRQn EXTI9_5_IRQn
#define UWB2_RESET_Pin GPIO_PIN_11
#define UWB2_RESET_GPIO_Port GPIOE
#define UWB2_CSn_Pin GPIO_PIN_12
#define UWB2_CSn_GPIO_Port GPIOE
#define UWB2_WAKEUP_Pin GPIO_PIN_13
#define UWB2_WAKEUP_GPIO_Port GPIOE
#define UWB3_IRQ_Pin GPIO_PIN_10
#define UWB3_IRQ_GPIO_Port GPIOD
#define UWB3_IRQ_EXTI_IRQn EXTI15_10_IRQn
#define UWB3_RESET_Pin GPIO_PIN_11
#define UWB3_RESET_GPIO_Port GPIOD
#define UWB3_CSn_Pin GPIO_PIN_12
#define UWB3_CSn_GPIO_Port GPIOD
#define UWB3_WAKEUP_Pin GPIO_PIN_13
#define UWB3_WAKEUP_GPIO_Port GPIOD
#define UWB4_IRQ_Pin GPIO_PIN_6
#define UWB4_IRQ_GPIO_Port GPIOC
#define UWB4_IRQ_EXTI_IRQn EXTI9_5_IRQn
#define UWB4_RESET_Pin GPIO_PIN_7
#define UWB4_RESET_GPIO_Port GPIOC
#define UWB4_CSn_Pin GPIO_PIN_8
#define UWB4_CSn_GPIO_Port GPIOC
#define UWB4_WAKEUP_Pin GPIO_PIN_9
#define UWB4_WAKEUP_GPIO_Port GPIOC
#define SYNC_EN_Pin GPIO_PIN_10
#define SYNC_EN_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
