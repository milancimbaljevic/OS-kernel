/*
 * kernelsem.cpp
 *
 *  Created on: May 2, 2021
 *      Author: OS1
 */
#include "kernSem.h"
#include <dos.h>

#define lock asm cli
#define unlock asm sti

#include <iostream.h>

SemaphoreList* KernelSem::allSem = 0;

KernelSem::KernelSem(int init,Semaphore* mySem){
	this->value = init;
	this->init = init;
	this->mySem = mySem;
	this->blocked = new Queue();
};

int KernelSem::getValue() const{
	return value;
}

KernelSem* KernelSem::createSemaphore(Semaphore* mySem, int init){
	lock;
	KernelSem* kernSem = new KernelSem(init,mySem);
	KernelSem::allSem->put(kernSem);
	unlock;
	return kernSem;
};

void KernelSem::block(Time maxTimeToWait){
	PCB::running->blocked = 1;
	blocked->put(PCB::running,maxTimeToWait);
	dispatch();
};

void KernelSem::unblock(){
	PCB* pcb = blocked->get();
	pcb->blocked = 0;
	pcb->semRet = 1;
	Scheduler::put(pcb);
};

int KernelSem::wait(Time maxTimeToWait){
	PCB::running->semRet = 0;
	lock;
	if(--value < 0) block(maxTimeToWait);
	unlock;
	return PCB::running->semRet;
};

void KernelSem::signal(){
	lock;
	if(++value <= 0) unblock();
	unlock;
};

KernelSem::~KernelSem(){
	// izbrisi queue
	// izbrisi me iz liste allSem
	blocked->deleteQueue();
	blocked = 0; // dodao
	lock;
	KernelSem::allSem->deleteElem(this);
	unlock;
};

