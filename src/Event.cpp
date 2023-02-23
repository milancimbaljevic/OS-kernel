/*
 * Event.cpp
 *
 *  Created on: May 5, 2021
 *      Author: OS1
 */
#include "Event.h"
#include "KernelEv.h"

Event::Event(IVTNo ivtNo){
	myImpl = KernelEv::createEvent(ivtNo,this);
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}

Event::~Event(){
	delete myImpl;
}
