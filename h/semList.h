/*
 * semList.h
 *
 *  Created on: May 4, 2021
 *      Author: OS1
 */

#ifndef SEMLIST_H_
#define SEMLIST_H_

#include "kernSem.h"

class KernelSem;

struct SemaphoreListElem {
	KernelSem* semaphore;
	SemaphoreListElem* next;
};

class SemaphoreList
{
public:
	SemaphoreListElem* front;
	SemaphoreListElem* rear;
	int size;
	SemaphoreList() {
		front = rear = 0;
		size = 0;
	};
	void put(KernelSem* sem);
	void deleteElem(KernelSem* sem);
	int getSize() {
		return this->size;
	};
	void deleteList();
};


#endif /* SEMLIST_H_ */
