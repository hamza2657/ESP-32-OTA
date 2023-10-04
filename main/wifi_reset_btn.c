/*
 * wifi_reset_btn.c
 *
 *  Created on: Oct 2, 2023
 *      Author: hamxa
 */
#include "wifi_reset_btn.h"
#include "wifi_app.h"
#include "tasks_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "rom/gpio.h"
#include "esp_log.h"

static const char TAG[] = "wifi_rest_button";

//semaphore handle
SemaphoreHandle_t  wifi_reset_semaphore = NULL;
/**
 * @fn void wifi_reset_button_isr_handler(void*)
 * @brief	ISR handler for the Wifi reset(BOOT) button 
 * 
 * @param arg parameter which can be passed to ISR handler
 */
void IRAM_ATTR wifi_reset_button_isr_handler(void *arg)
{
	//Notify the button task
	xSemaphoreGiveFromISR(wifi_reset_semaphore,NULL);  
}

/**
 * @fn void wifi_reset_button_task(void*)
 * @brief 	Wifi reset button task reacts to a Boot button event by sending a message
 * 			to the wifi application to disconnect from wifi and clear the saved credentials.
 * 
 * @param pvParam paramter which can be passed to the task
 */
void wifi_reset_button_task(void *pvParam)
{
	while(1)
	{
		if(xSemaphoreTake(wifi_reset_semaphore,portMAX_DELAY) == pdTRUE)
		{
			ESP_LOGI(TAG, "WIFI RESET BUTTON INTERUPT OCCURED");
			//send a message to disconnect wifi and clear credentials  form flash
			wifi_app_send_message(WIFI_APP_MSG_USER_REQUESTED_STA_DISCONNECT);
			
			vTaskDelay(2000/portTICK_PERIOD_MS);
			
		}
	}
}


void wifi_reset_button_config(void)
{
	//create the binary semaphore
	wifi_reset_semaphore = xSemaphoreCreateBinary();
	
	//Configuration the button and set the direction
	gpio_pad_select_gpio(WIFI_RESET_BUTTON);
	gpio_set_direction(WIFI_RESET_BUTTON, GPIO_MODE_INPUT);
	
	//Enable interupt on the negative edge(falling edge)
	gpio_set_intr_type(WIFI_RESET_BUTTON, GPIO_INTR_NEGEDGE);
	
	//create the wifi reset button task
	xTaskCreatePinnedToCore(&wifi_reset_button_task, "wifi_reset_button_task", WIFI_RESET_BUTTON_TASK_STACK_SIZE, NULL, WIFI_RESET_BUTTON_TASK_PRIORITY, NULL, WIFI_RESET_BUTTON_TASK_CORE_ID);
	
	//install gpio isr service
	gpio_install_isr_service(ESP_INTR_FLAG_DEFUALT);
	
	//Attach the ISR
	gpio_isr_handler_add(WIFI_RESET_BUTTON, wifi_reset_button_isr_handler, NULL);
	
}

