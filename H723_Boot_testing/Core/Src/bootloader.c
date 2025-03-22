/**
  ******************************************************************************
  * @file           : bootloader.c
  * @brief          : Source file for bootloader functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bootloader.h"
#include "main.h"

/**
  * @brief  Initialize bootloader peripherals
  * @retval None
  */
void Bootloader_Init(void)
{
  /* All required peripherals should be initialized by HAL_Init() and MX_GPIO_Init()
     in main.c before calling bootloader functions */
}

/**
  * @brief  Blink LED a specified number of times
  * @param  times: Number of times to blink
  * @param  delay_ms: Delay between ON and OFF states in milliseconds
  * @retval None
  */
void Bootloader_BlinkLed(uint8_t times, uint32_t delay_ms)
{
  for(uint8_t i = 0; i < times; i++)
  {
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
    HAL_Delay(delay_ms);
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
    HAL_Delay(delay_ms);
  }
}

/**
  * @brief  Jump to application located at specified address
  * @retval None
  */
void Bootloader_JumpToApplication(void)
{
  typedef void (*pFunction)(void);
  uint32_t JumpAddress;
  pFunction JumpToApplication;

  /* Check if valid stack address (RAM address) then jump */
//  if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFFFE00) == 0x20000000)
//  {
    /* Jump to user application */
    JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
    JumpToApplication = (pFunction) JumpAddress;

    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);

    /* Disable all interrupts */
    __disable_irq();

    /* Disable Systick timer */
//    SysTick->CTRL = 0;
    HAL_SuspendTick();

    /* Clear pending interrupts */
    SCB->ICSR |= SCB_ICSR_PENDSVCLR_Msk;

    /* Remap the vector table to the application's vector table */
    SCB->VTOR = APPLICATION_ADDRESS;

    HAL_ResumeTick();
    __enable_irq();

    /* Jump to application */
    JumpToApplication();
//  }

  /* Should never reach here unless there's no valid application */
  /* Error handling - stay in bootloader */
  while(1)
  {
    /* Error indication - blink red LED rapidly */
    HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
    HAL_Delay(100);
  }
}
