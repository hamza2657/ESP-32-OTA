/*
 * sntp_time_sync.c
 *
 *  Created on: Oct 2, 2023
 *      Author: hamxa
 */
#include "sntp_time_sync.h"
#include "wifi_app.h"
#include "http_server.h"
#include "tasks_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/apps/sntp.h"
#include "esp_log.h"

static const char TAG[] = "sntp_time_sync";

//SNTP operating mode set status
static bool sntp_op_mode_set = false;

/**
 * @fn void sntp_time_sync_init_sntp(void)
 * @brief	Initialize SNTP service using SNTP_OPMODE_POLL mode 
 * 
 */
static void sntp_time_sync_init_sntp(void)
{
	ESP_LOGI(TAG,"Initializing The SNTP service");
	if(!sntp_op_mode_set)
	{
		//set the operating mode
		sntp_setoperatingmode(SNTP_OPMODE_POLL);
		sntp_op_mode_set = true;
	}
	sntp_setservername(0, "pool.ntp.org");
	
	//initialize the servers
	sntp_init();
	
	//Let the http_server know service is initialized
	http_server_monitor_send_message(HTTP_MSG_TIME_SERVICE_INITIALIZED);
	
}
/**
 * @fn void sntp_time_sync_obtain_time(void)
 * @brief	Gets the current time and if the current time is not upto date
 * 			the sntp_time_sync_init_sntp function is called 
 * 
 */
static void sntp_time_sync_obtain_time(void)
{
	time_t now = 0;
	struct tm time_info = {0};
	time(&now);
	localtime_r(&now, &time_info);
	//check the time, in case we need to initialize /reinitialize
	if(time_info.tm_year < (2016 - 1900))
	{
		sntp_time_sync_init_sntp();
		//set the local time zone
		setenv("TZ", "PKT-5", 1);
		tzset();
	}
}
/**
 * @fn void sntp_time_sync(void*)
 * @brief	The SNTP synchronization task. 
 * 
 * @param pvParam
 */
static void sntp_time_sync(void *pvParam)
{
	while(1)
	{
		sntp_time_sync_obtain_time();
		vTaskDelay(10000/portTICK_PERIOD_MS);
	}
	
	vTaskDelete(NULL);
}

char* sntp_time_sync_get_time(void)
{
	static char time_buffer[100] = {0};
	
	time_t now = 0;
	struct tm time_info = {0};
	time(&now);
	localtime_r(&now,&time_info);
	if(time_info.tm_year < (2016 - 1900))
	{
		ESP_LOGI(TAG,"Time is not set yet");
	}
	else
	{
		 strftime(time_buffer, sizeof(time_buffer), "%d/%m/%Y %H:%M:%S", &time_info);
		ESP_LOGI(TAG,"Current time info: %s",time_buffer);
	}
	return time_buffer;
}

void sntp_time_sync_task_start(void)
{
	xTaskCreatePinnedToCore(&sntp_time_sync, "sntp_time_sync", SNTP_TIME_SYNC_TASK_TASK_STACK_SIZE, NULL, SNTP_TIME_SYNC_TASK_TASK_PRIORITY, NULL, SNTP_TIME_SYNC_TASK_TASK_CORE_ID);
}


