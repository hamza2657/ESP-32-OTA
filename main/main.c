/**
 * @fn void app_main(void)
 * @brief application entry point 
 * 
 */

#include "nvs_flash.h"
#include "esp_log.h"
#include "sntp_time_sync.h"
#include "wifi_app.h"
#include "dht11.h"
//#include "aws_iot.h"
#include "wifi_reset_btn.h"


static const char TAG[] = "main";

int aws_iot_demo_main( int argc, char ** argv );

void wifi_application_connected_events(void)
{
	ESP_LOGI(TAG,"WIFI Application Connected!!");
	sntp_time_sync_task_start();
//	aws_iot_start();
	aws_iot_demo_main(0,NULL);
}
void app_main(void)
{
    //Initlize the NVS
	esp_err_t ret = nvs_flash_init();
	if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
	
	//start wifi
	wifi_app_start();
	
	//Wifi reset button config
	wifi_reset_button_config();
	
	//start DHT11 task
	DHT11_task_start();
	
	//set connected event callback
	wifi_app_set_callback(&wifi_application_connected_events);
}

