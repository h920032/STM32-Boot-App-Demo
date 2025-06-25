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
#include "stm32h7xx_hal_uart.h"

/* Exported defines ----------------------------------------------------------*/
#define APPLICATION_ADDRESS     0x8005000U
#define BL_UART_BAUDRATE        115200U
#define BL_UART_TX_PIN          GPIO_PIN_7
#define BL_UART_TX_GPIO_PORT    GPIOG
#define BL_UART_RX_PIN          GPIO_PIN_8
#define BL_UART_RX_GPIO_PORT    GPIOG
#define BL_UART_AF              GPIO_AF8_LPUART1

/* Exported functions prototypes ---------------------------------------------*/
void Bootloader_Init(void);
void Bootloader_BlinkLed(uint8_t times, uint32_t delay_ms);
void Bootloader_JumpToApplication(void);
void Bootloader_Process(void);
void Bootloader_ReceiveFirmware(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOOTLOADER_H */
