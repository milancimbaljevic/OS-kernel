/*
 * hello.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: OS1
 */

#include <iostream.h>
#include "timer.h"
#include "pcb.h"
#include "idle.h"
#include "thread.h"
#include "mainThr.h"
#include "idle.h"
#include "kernSem.h"
#include "KernelEv.h"
#include "semaphor.h"

extern int userMain(int argc, char* argv[]);

class MainRun : public Thread{
public:
	int argc;
	char** argv;
	StackSize ss;
	Time ts;

	MainRun(int argc,char** argv,StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice) : Thread(stackSize,timeSlice){
		this->argc = argc;
		this->argv = argv;
		this->ss = stackSize;
		this->ts = timeSlice;
	};

	Thread* clone() const {
		return new MainRun(argc,argv,ss,ts);
	}

	virtual void run();

	virtual ~MainRun(){
		waitToComplete();
	}
};

void MainRun::run(){
	userMain(argc,argv);
};

void main(int argc, char* argv[])
{

	// moram da napravim nit za main
	PCB::allPCB = new PCBList();
	KernelSem::allSem = new SemaphoreList();
	MainThread::mainThread = new MainThread();
	idle::idleThread = new idle();
	PCB::running = PCB::allPCB->findById(MainThread::mainThread->getId());
	PCB::running->ready = 1;

	MainRun* mt = new MainRun(argc,argv,1000,defaultTimeSlice);
	mt->start();
	inic();
	delete mt;
	restore();

	delete idle::idleThread;
	delete MainThread::mainThread;
	KernelSem::allSem->deleteList();

	if(PCB::allPCB->size != 0){
		PCBListElem* temp = PCB::allPCB->front;
		PCBListElem* next = 0;
		while(temp != 0){
			delete temp->pcb;
			next = temp->next;
			delete temp;
			temp = next;
		}
	}

	PCB::allPCB = 0;
	KernelSem::allSem = 0;
	delete [] KernelEv::allEvents;

	cout << "Kraj main()." << endl;
}



