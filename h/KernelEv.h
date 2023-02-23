/*
 * KernelEv.h
 *
 *  Created on: May 5, 2021
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "pcb.h"

typedef unsigned char IVTNo;

class Event;

class KernelEv{
public:
	static KernelEv* allEvents[256];
	void wait();
	void signal();
	IVTNo ivtNo;
	static KernelEv* createEvent(IVTNo ivtNo,Event* ev);
	~KernelEv();
private:
	KernelEv(IVTNo ivtNo);
	PCB* creator;
	PCB* blocked;
	int value;
	Event* myEvent;
};

#endif /* KERNELEV_H_ */









