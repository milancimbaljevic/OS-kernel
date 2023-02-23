/*
 * IVTEntry.cpp
 *
 *  Created on: May 5, 2021
 *      Author: OS1
 */
#include "IVTEntry.h"
#include "KernelEv.h"
#include <dos.h>


IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newInterruptRoutine){


	this->ev = 0;
	this->ivtNo = ivtNo;
	this->newInterruptRoutine = newInterruptRoutine;

#ifndef BCC_BLOCK_IGNORE
	this->oldInterruptRoutine = getvect(ivtNo);
	lock
	setvect(this->ivtNo,this->newInterruptRoutine);
	unlock
#endif
};

void IVTEntry::oldRoutine(){
	this->oldInterruptRoutine();
};

void IVTEntry::signal(){
	KernelEv::allEvents[ivtNo]->signal();
};

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	lock
	setvect(this->ivtNo, this->oldInterruptRoutine);
	this->oldInterruptRoutine();
	unlock
#endif
};
