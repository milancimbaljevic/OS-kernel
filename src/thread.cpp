#include "thread.h"
#include "pcb.h"
#include "timer.h"

void dispatch(){
	PCB::dispatch();
}

Thread::Thread(StackSize stackSize, Time timeSlice){
	myPCB = PCB::createPCB(this,stackSize,timeSlice);
}

ID Thread::fork(){
	PCB::fork();
}

void Thread::exit(){
	PCB::exit();
}

void Thread::waitForForkChildren(){
	PCB::waitForForkChildren();
}

Thread* Thread::clone() const{
	return new Thread(myPCB->stackSize*sizeof(unsigned),myPCB->timeSlice);
}

void Thread::start(){
	myPCB->start();
}

ID Thread::getId(){
	return this->myPCB->getID();
}

ID Thread::getRunningId(){
	return PCB::running->getID();
}

Thread* Thread::getThreadById(ID id){
	return PCB::getThreadById(id);
}

void Thread::waitToComplete(){
	this->myPCB->waitToComplete();
}

Thread::~Thread(){
	myPCB->threadDestructor();
}





