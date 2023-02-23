/*
 * semaphore.cpp
 *
 *  Created on: May 2, 2021
 *      Author: OS1
 */
#include "kernSem.h"
#include "semaphor.h"

Semaphore::Semaphore(int init){
	this->myImpl = KernelSem::createSemaphore(this,init);
};

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
};

void Semaphore::signal(){
	myImpl->signal();
};

int Semaphore::val() const{
	return myImpl->getValue();
};

Semaphore::~Semaphore(){
	delete myImpl;
};

