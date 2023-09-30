/*
 * wifi_app.h
 *
 *  Created on: Sep 16, 2023
 *      Author: hamxa
 */

#ifndef MAIN_WIFI_APP_H_
#define MAIN_WIFI_APP_H_

#include "esp_netif.h"
#include "freertos/FreeRTOS.h"



//wifi application setting
#define WIFI_AP_SSID    			"ESP-32 AP"  //AP name
#define WIFI_AP_PASSWORD			"12345678" //AP password
#define WIFI_AP_CHANNEL				1			//AP Channel
#define WIFI_AP_SSID_HIDDEN			0			//AP visibility
#define WIFI_AP_MAX_CONNECTIONS		5			//AP max clients
#define WIFI_AP_BEACON_INTERVAL		100			//Recommended by default

#define WIFI_AP_IP					"192.168.10.1" //AP default IP
#define WIFI_AP_GATEWAY				"192.168.0.1" //Default gateway
#define WIFI_AP_NETMASK				"255.255.255.0" //default subnet mask
#define WIFI_AP_BANDWIDTH			WIFI_BW_HT20 //AP bandwidth
#define WIFI_STA_POWER_SAVE			WIFI_PS_NONE //power save none
#define MAX_SSID_LENGTH				32 //IEEE standard maximum
#define MAX_PASSWORD_LENGTH			64 //IEEE standard
#define MAX_CONNECTION_RETRIES		5

//netif object for the station and AP

extern esp_netif_t* esp_netif_sta;
extern esp_netif_t* esp_netif_ap;
/**
 * Message IDS for the Wifi application task
 * @none Expand this on you application requirement
 */

typedef enum wifi_app_message{
	WIFI_APP_MSG_START_HTTP_SERVER = 0,
	WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER,
	WIIF_APP_MSG_STA_CONNECTED_GOT_IP,
}wifi_app_message_e;

/**
 * structure for the message queue
 * @note expand this based on your application  e.g add another parameter or type as per your need
 */
typedef struct wifi_app_queue_message{
	wifi_app_message_e msgID;
}wifi_app_queue_message_t;

/**
 * sends a message to the queue
 * @param msgID from the wifi_app_message_e enum 
 * @return pdTRUE if an item was successfully sent to the queue and vice vers
 * @note Expand the parameter based on your requiremnet
 */

BaseType_t wifi_app_send_message(wifi_app_message_e msgID);

/**
 * start the WiFI RTOS task
 */
void wifi_app_start(void);



















#endif /* MAIN_WIFI_APP_H_ */
