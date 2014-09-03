/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @date    02/09/2014 21:06:16
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2014 STMicroelectronics
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
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

/* External variables --------------------------------------------------------*/

extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi2_rx;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern DMA_HandleTypeDef hdma_spi3_rx;
extern DMA_HandleTypeDef hdma_spi3_tx;
extern DMA_HandleTypeDef hdma_spi4_rx;
extern DMA_HandleTypeDef hdma_spi4_tx;
extern DMA_HandleTypeDef hdma_spi5_tx;
extern DMA_HandleTypeDef hdma_spi5_rx;
extern DMA_HandleTypeDef hdma_spi6_rx;
extern DMA_HandleTypeDef hdma_spi6_tx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles DMA2 Stream2 global interrupt.
*/
void DMA2_Stream2_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA2_Stream2_IRQn);
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
}

/**
* @brief This function handles DMA2 Stream3 global interrupt.
*/
void DMA2_Stream3_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA2_Stream3_IRQn);
  HAL_DMA_IRQHandler(&hdma_spi5_rx);
}

/**
* @brief This function handles DMA1 Stream0 global interrupt.
*/
void DMA1_Stream0_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA1_Stream0_IRQn);
  HAL_DMA_IRQHandler(&hdma_spi3_rx);
}

/**
* @brief This function handles DMA1 Stream6 global interrupt.
*/
void DMA1_Stream6_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA1_Stream6_IRQn);
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

/**
* @brief This function handles DMA2 Stream0 global interrupt.
*/
void DMA2_Stream0_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA2_Stream0_IRQn);
  HAL_DMA_IRQHandler(&hdma_spi4_rx);
}

/**
* @brief This function handles DMA2 Stream6 global interrupt.
*/
void DMA2_Stream6_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA2_Stream6_IRQn);
  HAL_DMA_IRQHandler(&hdma_spi6_rx);
  
}

/**
* @brief This function handles DMA2 Stream1 global interrupt.
*/
void DMA2_Stream1_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA2_Stream1_IRQn);
  HAL_DMA_IRQHandler(&hdma_spi4_tx);
}

/**
* @brief This function handles DMA1 Stream3 global interrupt.
*/
void DMA1_Stream3_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA1_Stream3_IRQn);
  HAL_DMA_IRQHandler(&hdma_spi2_rx);
}

/**
* @brief This function handles DMA2 Stream4 global interrupt.
*/
void DMA2_Stream4_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA2_Stream4_IRQn);
  HAL_DMA_IRQHandler(&hdma_spi5_tx);
}

/**
* @brief This function handles DMA1 Stream4 global interrupt.
*/
void DMA1_Stream4_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA1_Stream4_IRQn);
  HAL_DMA_IRQHandler(&hdma_spi2_tx);
}

/**
* @brief This function handles DMA1 Stream5 global interrupt.
*/
void DMA1_Stream5_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA1_Stream5_IRQn);
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
}

/**
* @brief This function handles DMA1 Stream7 global interrupt.
*/
void DMA1_Stream7_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA1_Stream7_IRQn);
  HAL_DMA_IRQHandler(&hdma_spi3_tx);
}

/**
* @brief This function handles DMA2 Stream5 global interrupt.
*/
void DMA2_Stream5_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(DMA2_Stream5_IRQn);
  HAL_DMA_IRQHandler(&hdma_spi6_tx);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
