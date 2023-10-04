/*
 * sntp_time_sync.h
 *
 *  Created on: Oct 2, 2023
 *      Author: hamxa
 */

#ifndef MAIN_SNTP_TIME_SYNC_H_
#define MAIN_SNTP_TIME_SYNC_H_

/**
 * @fn void sntp_time_sync_task_start(void)
 * @brief Start the SNTP server synchronization task
 * 
 */
void sntp_time_sync_task_start(void);

/**
 * @fn char sntp_time_sync_get_time*(void)
 * @brief return local time if set
 * 
 * @return local time buffer
 */
char* sntp_time_sync_get_time(void);
#endif /* MAIN_SNTP_TIME_SYNC_H_ */
