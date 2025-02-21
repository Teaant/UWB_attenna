/*
 * aoa_qeuue.c
 *
 *  Created on: Feb 21, 2025
 *      Author: 24848
 */

#include "aoa_queue.h"

#define	QUEUE_SIZE	5

//U_Task tasks_fifo[QUEUE_SIZE];
uint8_t params_fifo[QUEUE_SIZE];
uint8_t front = 0, rear;


uint8_t enqueueData( uint8_t id){
	//队列满
	if((rear+1)%QUEUE_SIZE == front){
		return 0;
	}else{
		params_fifo[rear] = id;
		rear = (rear+1)%QUEUE_SIZE;
		return 1;
	}
}

uint8_t dequeueData(void){
	//队列空
	uint8_t dwt_id;

	if(front == rear){
		return 255;
	}else{
		dwt_id = params_fifo[front];
		front = (front+1)%QUEUE_SIZE;
		return dwt_id;
	}
}

void empty_fifo(void){
	front = rear;
}

uint8_t getQueueSize(void){
	return ((rear- front + QUEUE_SIZE)%QUEUE_SIZE);
}
