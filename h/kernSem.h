/*
 * kernSem.h
 *
 *  Created on: May 4, 2021
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_


#include "semList.h"
#include "Queue.h"
#include "semaphor.h"

class SemaphoreList;
class Semaphore;

class KernelSem{
private:
	friend void interrupt timer();
	int value;
	int init;
	KernelSem(int init, Semaphore* mySem);
	void block(Time maxTimeToWait);
	void unblock();
public:
	Semaphore* mySem;
	static SemaphoreList* allSem;
	Queue* blocked;
	int getValue() const;
	int wait (Time maxTimeToWait);
	void signal();
	static KernelSem* createSemaphore(Semaphore* mySem, int init);
	~KernelSem();
};


#endif /* KERNSEM_H_ */
