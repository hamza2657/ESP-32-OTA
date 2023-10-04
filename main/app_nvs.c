/*
 * app_nvs.c
 *
 *  Created on: Oct 2, 2023
 *      Author: hamxa
 */
#include "app_nvs.h"
#include "wifi_app.h"
#include "stdio.h"
#include "string.h"

#include "esp_log.h"
#include "nvs_flash.h"

//TAG for logging to the monitor
static const char TAG[] = "NVS";

//nvs namespace used for station mode credentials
const char app_nvs_sta_creds_nampespace[] = "stacreds";


esp_err_t app_nvs_save_sta_creds(void)
{
	nvs_handle handle;
	esp_err_t esp_errcheck;
	ESP_LOGI(TAG,"app_nvs_save_sta_creds: Saving station mode credentials to flash");
	wifi_config_t *wifi_sta_config = wifi_app_get_wifi_config();
	if(wifi_sta_config)
	{
		esp_errcheck = nvs_open(app_nvs_sta_creds_nampespace, NVS_READWRITE, &handle);
		if(esp_errcheck != ESP_OK)
		{
			printf("app_nvs_save_sta_creds: Error (%s) opening NVS handle!\n",esp_err_to_name(esp_errcheck));
			return esp_errcheck;
		}
		//set SSID
		esp_errcheck = nvs_set_blob(handle, "ssid", wifi_sta_config->sta.ssid, MAX_SSID_LENGTH);
		if(esp_errcheck != ESP_OK)
		{
			printf("app_nvs_save_sta_creds: Error (%s) setting ssid to NVS!\n", esp_err_to_name(esp_errcheck));
			return esp_errcheck;
		}
		//set Password
		esp_errcheck = nvs_set_blob(handle, "password", wifi_sta_config->sta.password, MAX_PASSWORD_LENGTH);
		if(esp_errcheck != ESP_OK)
		{
			printf("app_nvs_save_sta_creds: Error (%s) setting password to NVS!\n", esp_err_to_name(esp_errcheck));
			return esp_errcheck;
		}
		//commit credential to NVS
		esp_errcheck = nvs_commit(handle);
		if(esp_errcheck != ESP_OK)
		{
			printf("app_nvs_save_sta_creds: Error (%s) committing credentials to NVS!\n",esp_err_to_name(esp_errcheck));
			return esp_errcheck;
		}
		nvs_close(handle);
		ESP_LOGI(TAG,"app_nvs_save_sta_creds: wrote wifi credentials : stations SSID: %s  Password: %s",wifi_sta_config->sta.ssid,wifi_sta_config->sta.password);
	}
	printf("app_nvs_save_sta_creds:Saving Operation successful\n");
	return ESP_OK; 
}



bool app_nvs_load_sta_creds(void)
{
	nvs_handle handle;
	esp_err_t esp_errcheck;
	ESP_LOGI(TAG, "app_nvs_load_sta_creds: Loading Wifi credentials from flash");
	if(nvs_open(app_nvs_sta_creds_nampespace, NVS_READONLY, &handle) == ESP_OK)
	{
		wifi_config_t *wifi_sta_config = wifi_app_get_wifi_config();
		if(wifi_sta_config == NULL)
		{
			wifi_sta_config = (wifi_config_t*)malloc(sizeof(wifi_config_t));
		}
		memset(wifi_sta_config,0x00,sizeof(wifi_config_t));
		//Allocate buffer
		size_t wifi_config_size = sizeof(wifi_config_t);
		uint8_t *wifi_config_buff = (uint8_t*)malloc(sizeof(uint8_t) *wifi_config_size);
		memset(wifi_config_buff,0x00,sizeof(wifi_config_size));
		
		//load SSID
		wifi_config_size = sizeof(wifi_sta_config->sta.ssid);
		esp_errcheck = nvs_get_blob(handle, "ssid", wifi_config_buff, &wifi_config_size);
		if(esp_errcheck != ESP_OK)
		{
			free(wifi_config_buff);
			printf("app_nvs_load_sta_creds: (%s) No station SSID found in NVS\n",esp_err_to_name(esp_errcheck));
			return false;
		}
		memcpy(wifi_sta_config->sta.ssid,wifi_config_buff,wifi_config_size );
		
		//load Password
		wifi_config_size = sizeof(wifi_sta_config->sta.password);
		esp_errcheck = nvs_get_blob(handle, "password", wifi_config_buff, &wifi_config_size);
		if(esp_errcheck != ESP_OK)
		{
			free(wifi_config_buff);
			printf("app_nvs_load_sta_creds: (%s) retrieving password\n",esp_err_to_name(esp_errcheck));
			return false;
		}
		memcpy(wifi_sta_config->sta.password,wifi_config_buff,wifi_config_size );
		
		free(wifi_config_buff);
		nvs_close(handle);
		
		printf("app_nvs_load_sta_creds: SSID: %s\tPassword: %s\t",wifi_sta_config->sta.ssid,wifi_sta_config->sta.password);
		return wifi_sta_config->sta.ssid[0] != '\0';
	}
	else
	{
		return false;
	}
}

esp_err_t app_nvs_clear_sta_creds(void)
{
	nvs_handle handle;
	esp_err_t esp_errcheck;
	ESP_LOGI(TAG,"app_nvs_clear_sta_creds: Clearing Wifi station mode credentials from flash");
	esp_errcheck = nvs_open(app_nvs_sta_creds_nampespace, NVS_READWRITE, &handle);
	if(esp_errcheck != ESP_OK)
	{
		printf("app_nvs_clear_sta_creds: Error (%s) opening NVS handle\n",esp_err_to_name(esp_errcheck));
		return esp_errcheck;
	}
	
	//erase credential
	esp_errcheck = nvs_erase_all(handle);
	if(esp_errcheck != ESP_OK)
	{
		printf("app_nvs_clear_sta_creds: Error (%s) erasing flash\n",esp_err_to_name(esp_errcheck));
		return esp_errcheck;
	}
	
	//commit clearing credential from NVS
	esp_errcheck = nvs_commit(handle);
	if(esp_errcheck != ESP_OK)
	{
		printf("app_nvs_clear_sta_creds: Error (%s) NVS commit\n",esp_err_to_name(esp_errcheck));
		return esp_errcheck;
	}
	nvs_close(handle);
	printf("app_nvs_clear_sta_creds: Operation done successfully\n");
	return ESP_OK;
	
}






























