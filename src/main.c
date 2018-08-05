/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use GPIOs through 
  *          the STM32F4xx HAL API.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "string.h"
#include "lwip.h"
#include "usbhost.h"
#include <unistd.h>

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static char* pwelcomeMsg = "CubeMX console\r\n";
//static char ptestMsg[1024];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/
int _write(int32_t fd, char* ptr, int32_t len) {
	if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
		BSP_UART3_Send((uint8_t*)ptr, len);
	}
	return len;
}

int _lseek() {
	return 0;
}

int _close() {
	return 0;
}

int _read() {
	return 0;
}

int _isatty() {
	return 0;
}

int _fstat() {
	return 0;
}

void vLedTask( )

{
    for(;;)
    {
    	BSP_LED_Toggle(LED1);
    	BSP_LED_Toggle(LED2);

    	/* Insert delay 100 ms */
        vTaskDelay(100);
    }
}

void vUart3Task( )

{
	char rcvChar;
	//int currentTick;

	BSP_UART3_Send((uint8_t *)pwelcomeMsg, strlen(pwelcomeMsg));
    for(;;)
    {
    	if(BSP_UART3_Receive((uint8_t *)&rcvChar)==1)
    	{
    		BSP_UART3_Send((uint8_t *)&rcvChar, 1);
    	}

    	//memset(ptestMsg,0,1024);
    	//currentTick = xTaskGetTickCount();
    	//sprintf(ptestMsg,"1234567890,%010d\r\n", currentTick);
    	//BSP_UART3_Send((uint8_t *)ptestMsg, strlen(ptestMsg));

    	/* Insert delay 20 ms */
        //vTaskDelay(1000);
        //printf("Hello from STM32 board\r\n");
    }
}

void vLwipTask( )

{
	MX_LWIP_Init();

	for(;;)
    {
    	/* Insert delay 100 ms */
        vTaskDelay(1000);
        //printf("Hello from STM32 board\r\n");
    }
}

void vUSBTask( )

{
	char pMsgBuf[64];
	char pRcvBuf[64];

	MX_USB_HOST_Init();

    for(;;)
    {
    	/* Insert delay 100 ms */
        vTaskDelay(1000);
    	memset(pMsgBuf,0,64);
    	memset(pRcvBuf,0,64);
    	sprintf(pMsgBuf,"AT\r\n");

    	if(Appli_state == APPLICATION_READY)
    	{
    		USBH_CDC_Transmit(&hUsbHostFS,pMsgBuf,strlen(pMsgBuf));
    		USBH_CDC_Receive(&hUsbHostFS,pRcvBuf,64);
    	}
    }
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 84 MHz */
  SystemClock_Config();

  /* Configure PF9/10 IO in output push-pull mode to
         drive external LED */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /* UART3 init */
  BSP_UART3_Init();

  /* ETH init */
  BSP_ETH_Init();

  /* init code for USB_HOST */
  //MX_USB_HOST_Init();

  /* -3- Toggle PA05 IO in an infinite loop */  
  xTaskCreate( vLedTask, "LED", configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY+1, NULL );
  xTaskCreate( vUart3Task, "UART3", configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY+1, NULL );
  //xTaskCreate( vLwipTask, "LWIP", configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY+1, NULL );
  xTaskCreate( vUSBTask, "USB", configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY+1, NULL );
  vTaskStartScheduler();
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 84000000
  *            HCLK(Hz)                       = 84000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 336
  *            PLL_P                          = 4
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale2 mode
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  
  /* Enable HSI Oscillator and activate PLL with HSI as source */
  /*RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;*/

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
	  BSP_Error_Handler(__FILE__, __LINE__);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  /*RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;*/
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
	  BSP_Error_Handler(__FILE__, __LINE__);
  }
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
