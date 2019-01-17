#include "stm32f1xx.h"
#include "ClockConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <string.h>

UART_HandleTypeDef uart;

void vTaskLED(void * pvParameters) {
	for (;;) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //Led on
		vTaskDelay(500 / portTICK_RATE_MS);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //Led off
		vTaskDelay(500 / portTICK_RATE_MS);
	}
}

void prvSetupHardware() {

	HAL_Init();
	
	/* STM32F103xB HAL library initialization:
	 - Configure the Flash prefetch
	 - Systick timer is configured by default as source of time base, but user
	 can eventually implement his proper time base source (a general purpose
	 timer for example or other time source), keeping in mind that Time base
	 duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
	 handled in milliseconds basis.
	 - Set NVIC Group Priority to 4
	 - Low Level Initialization */

	/* Enable HSE oscillator and configure the PLL to reach the max system frequency (64 MHz)
	 when using HSE oscillator as PLL clock source. */
	 
	 /* Output SYSCLK on MCO1 pin(PA.08) */
	SystemClock_Config();
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	GPIO_InitTypeDef gpio;

	gpio.Pin = GPIO_PIN_5; //Led Pin Config
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio);
}

int main(void) {
	//Hardware init
	prvSetupHardware();

	//Blinking led Task create
	xTaskCreate(vTaskLED, (const portCHAR *) "LED", 128, NULL, 1, NULL);
	//OS start (neverending scheduler function)
	vTaskStartScheduler();

	//It will never happen if everything is ok
	return 0;
}
