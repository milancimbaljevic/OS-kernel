/*
 * Queue.h
 *
 *  Created on: May 4, 2021
 *      Author: OS1
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "pcb.h"

struct QueueElem {
	PCB* pcb;
	QueueElem* next;
	int maxTimeBlocked;
};

class Queue
{
private:
	friend interrupt void timer();
	QueueElem* front;
	QueueElem* rear;
	int size;
public:
	Queue() {
		front = rear = 0;
		size = 0;
	}
	PCB* get();
	void put(PCB* pcb, int maxTimeBlocked = 0);
	void deleteElem(PCB* pcb);
	int getSize() {
		return this->size;
	}
	void deleteQueue();
};

#endif
