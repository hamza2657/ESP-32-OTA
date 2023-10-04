/*
 * app_nvs.h
 *
 *  Created on: Oct 2, 2023
 *      Author: hamxa
 */

#ifndef MAIN_APP_NVS_H_
#define MAIN_APP_NVS_H_

#include "esp_err.h"
#include "stdbool.h"
/**
 * @fn esp_err_t app_nvs_save_sta_creds(void)
 * @brief Saves station mode wifi credentials to NVS
 * 
 * @return ESP_OK if successfull.
 */

esp_err_t app_nvs_save_sta_creds(void);

/**
 * @fn bool app_nvs_load_sta_creds(void)
 * @brief	loads the previously saved credential from NVS.
 *  
 * 
 * @return	true ifpreviously saved vale were found.
 */
bool app_nvs_load_sta_creds(void);

/**
 * @fn esp_err_t app_nvs_clear_sta_creds(void)
 * @brief  clear station mode credentials from NVS
 * 
 * @return ESP_OK if successful.
 */
esp_err_t app_nvs_clear_sta_creds(void);

#endif /* MAIN_APP_NVS_H_ */
