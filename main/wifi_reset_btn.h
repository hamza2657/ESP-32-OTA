/*
 * wifi_reset_btn.h
 *
 *  Created on: Oct 2, 2023
 *      Author: hamxa
 */

#ifndef MAIN_WIFI_RESET_BTN_H_
#define MAIN_WIFI_RESET_BTN_H_

//Default interupt flag
#define ESP_INTR_FLAG_DEFUALT		0

//wifi reset button is the BOOT button on the ESP32
#define WIFI_RESET_BUTTON			0

/**
 * @fn void wifi_reset_button_config(void)
 * @brief Configure wifi reset button and interrupt configuration 
 * 
 */
void wifi_reset_button_config(void);


#endif /* MAIN_WIFI_RESET_BTN_H_ */
