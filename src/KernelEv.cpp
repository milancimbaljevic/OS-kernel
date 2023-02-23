/*
 * KernelEv.cpp
 *
 *  Created on: May 5, 2021
 *      Author: OS1
 */

#include "KernelEv.h"

KernelEv* KernelEv::allEvents[256] = {0};

KernelEv::KernelEv(IVTNo ivtNo){
	this->value = 0;
	this->ivtNo = ivtNo;
	this->creator = PCB::running;
	this->blocked = 0;
	KernelEv::allEvents[this->ivtNo] = this;
};

void KernelEv::wait(){
	if(this->creator == PCB::running){
		lock;
		if(this->value == 0){
			PCB::running->blocked = 1;
			this->blocked = PCB::running;
			dispatch();
		}else{
			this->value = 0;
		}
		unlock;
	}
};


void KernelEv::signal(){
	lock;
	if(this->blocked == 0){
		this->value = 1;
	}else{
		this->blocked->blocked = 0;
		Scheduler::put(this->blocked);
		this->blocked = 0;
	}
	unlock;
}

KernelEv* KernelEv::createEvent(IVTNo ivtNo, Event* ev){
	lock;
	KernelEv* kev = new KernelEv(ivtNo);
	kev->myEvent = ev;
	unlock;
	return kev;
}

KernelEv::~KernelEv(){
	KernelEv::allEvents[ivtNo] = 0;
}


