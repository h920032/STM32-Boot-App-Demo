/**
  ******************************************************************************
  * @file           : bootloader.h
  * @brief          : Header file for bootloader functions
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Exported defines ----------------------------------------------------------*/
#define APPLICATION_ADDRESS     0x8005000U

/* Exported functions prototypes ---------------------------------------------*/
void Bootloader_Init(void);
void Bootloader_BlinkLed(uint8_t times, uint32_t delay_ms);
void Bootloader_JumpToApplication(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOOTLOADER_H */
