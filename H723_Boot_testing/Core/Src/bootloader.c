/**
  ******************************************************************************
  * @file           : bootloader.c
  * @brief          : Source file for bootloader functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bootloader.h"
#include "main.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
static UART_HandleTypeDef huart_bl;

/**
  * @brief  Initialize bootloader peripherals
  * @retval None
  */
void Bootloader_Init(void)
{
  /* All required peripherals should be initialized by HAL_Init() and MX_GPIO_Init()
     in main.c before calling bootloader functions */
  /* Initialize UART used for firmware update */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_LPUART1_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = BL_UART_TX_PIN | BL_UART_RX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = BL_UART_AF;
  HAL_GPIO_Init(BL_UART_TX_GPIO_PORT, &GPIO_InitStruct);

  huart_bl.Instance = LPUART1;
  huart_bl.Init.BaudRate = BL_UART_BAUDRATE;
  huart_bl.Init.WordLength = UART_WORDLENGTH_8B;
  huart_bl.Init.StopBits = UART_STOPBITS_1;
  huart_bl.Init.Parity = UART_PARITY_NONE;
  huart_bl.Init.Mode = UART_MODE_TX_RX;
  huart_bl.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart_bl.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart_bl.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart_bl.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart_bl);
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

/**
 * @brief  Receive application firmware over UART and program flash
 * @retval None
 */
void Bootloader_ReceiveFirmware(void)
{
  uint8_t size_buf[4];
  uint8_t data_buf[256];
  uint32_t addr = APPLICATION_ADDRESS;
  uint32_t firmware_size;
  uint32_t sector_error;
  FLASH_EraseInitTypeDef erase_init;

  /* Request firmware size */
  const char msg[] = "SEND";
  HAL_UART_Transmit(&huart_bl, (uint8_t*)msg, sizeof(msg)-1, HAL_MAX_DELAY);

  if (HAL_UART_Receive(&huart_bl, size_buf, sizeof(size_buf), HAL_MAX_DELAY) != HAL_OK)
    return;

  firmware_size = size_buf[0] | (size_buf[1] << 8) | (size_buf[2] << 16) | (size_buf[3] << 24);

  /* Erase necessary flash sectors */
  erase_init.TypeErase   = FLASH_TYPEERASE_SECTORS;
  erase_init.VoltageRange= FLASH_VOLTAGE_RANGE_3;
  erase_init.Banks       = FLASH_BANK_1;
  erase_init.Sector      = (addr - FLASH_BASE) / 0x20000U;
  erase_init.NbSectors   = (firmware_size + 0x1FFFFU) / 0x20000U;

  HAL_FLASH_Unlock();
  HAL_FLASHEx_Erase(&erase_init, &sector_error);

  for (uint32_t offset = 0; offset < firmware_size; offset += sizeof(data_buf))
  {
    uint32_t chunk = sizeof(data_buf);
    if (offset + chunk > firmware_size)
      chunk = firmware_size - offset;

    if (HAL_UART_Receive(&huart_bl, data_buf, chunk, HAL_MAX_DELAY) != HAL_OK)
      break;

    for (uint32_t i = 0; i < chunk; i += 8)
    {
      uint64_t word = 0xFFFFFFFFFFFFFFFFULL;
      memcpy(&word, &data_buf[i], (chunk - i >= 8) ? 8 : chunk - i);
      HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, addr + offset + i, word);
    }
  }
  HAL_FLASH_Lock();
}

/**
 * @brief  Check for update command and jump to application when done
 * @retval None
 */
void Bootloader_Process(void)
{
  if (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_RESET)
  {
    Bootloader_ReceiveFirmware();
  }

  Bootloader_JumpToApplication();
}
