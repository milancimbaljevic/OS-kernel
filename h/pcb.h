/*
 * pcb.h
 *
 *  Created on: Apr 30, 2021
 *      Author: OS1
 */

#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "PCBList.h"
#include "SCHEDULE.h"

#define lock asm cli
#define unlock asm sti

class PCBList;

class PCB{
public:
	unsigned semRet;
	friend Thread* Thread::clone() const;
	static void dispatch();
	friend void main(int argc, char* argv[]);
	friend void interrupt timer();
	unsigned started;
	ID getID();
	static Thread* getThreadById(ID id);
	static PCB* createPCB(Thread* myThread,StackSize stackSize, Time timeSlice);
	static PCBList* allPCB;
	static PCB* running;
	void threadDestructor();
	void finish();
	static void exit();
	static void waitForForkChildren();
	static ID fork();
	~PCB();
private:
	friend class Timer;
	friend class KernelSem;
	friend class KernelEv;
	friend void Thread::start();
	friend void Thread::waitToComplete();
	void start();
	friend int interrupt _fork();
	static ID gID;
	Thread* myThread;
	unsigned
	int numOfChildrenActive;
	int fatherID;
	int isWaitingForChildren;
	unsigned* stack;
	unsigned userSS;
	unsigned userSP;
	unsigned userBP;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned ready;
	unsigned finished;
	unsigned blocked;
	ID pcbID;
	StackSize stackSize;
	Time timeSlice;
	PCBList* waitingForMeToFinish; // pokazivac na listu pcb-ova koji cekaju da ja zavrsim
	PCB(Thread* myThread,StackSize stackSize, Time timeSlice);
	static void wrapper();
	void waitToComplete();
};
#endif
