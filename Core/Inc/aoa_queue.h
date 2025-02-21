/*
 * aoa_queue.h
 *
 *  Created on: Feb 21, 2025
 *      Author: 24848
 */

#ifndef INC_AOA_QUEUE_H_
#define INC_AOA_QUEUE_H_

#include "main.h"


uint8_t enqueueData( uint8_t id);

uint8_t dequeueData();

void empty_fifo(void);

uint8_t getQueueSize(void);


#endif /* INC_AOA_QUEUE_H_ */
