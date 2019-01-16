/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f1xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void prvSetupHardware() {
	SystemCoreClockUpdate(); //Need this for update default clock (8 mhz)

	HAL_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef gpio;

	gpio.Pin = GPIO_PIN_5; //Led Pin Config
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio);
}

void vTaskLED(void * pvParameters) {
	for (;;) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //Led on
		vTaskDelay(500 / portTICK_RATE_MS);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //Led off
		vTaskDelay(500 / portTICK_RATE_MS);
	}
}

int main(void) {
	//Hardware init
	prvSetupHardware();

	//Blinking led Task create
	xTaskCreate(vTaskLED, ( const portCHAR * ) "LED", 128, NULL, 1, NULL);

	//OS start (neverending scheduler function)
	vTaskStartScheduler();

	//It will never happen if everything is ok
	return 0;
}
