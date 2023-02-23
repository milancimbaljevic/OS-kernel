/*
 * pcb.cpp
 *
 *  Created on: Apr 30, 2021
 *      Author: OS1
 */
#include "pcb.h"
#include "Timer.h"
#include "mainThr.h"
#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.h"

int PCB::gID = 0;
PCBList* PCB::allPCB = 0;
PCB* PCB::running = 0;



volatile unsigned userBP,userSS,userSP,kernelSP,kernelSS;


int interrupt _fork(){
	lock;
	asm{
		mov word ptr userSP, sp
		mov word ptr userBP, bp
		mov word ptr userSS, ss
	}
	PCB::running->userSP = userSP;
	PCB::running->userSS = userSS;
	PCB::running->userBP = userBP;
	unlock;

	unsigned* userStackBP = 0;
#ifndef BCC_BLOCK_IGNORE
	userStackBP = (unsigned*) MK_FP(PCB::running->userSS,PCB::running->userBP);
#endif
	unsigned* userStackTop = PCB::running->stack + PCB::running->stackSize - 1;

	Thread* childThread = PCB::running->myThread->clone(); // napravi klon niti oca

	lock;
	PCB* childPCB = PCB::allPCB->findById(childThread->getId());
	unlock;


	childPCB->fatherID = PCB::running->getID();
	childPCB->ready = 1;
	childPCB->started = 1;

	// kopiraj stek

	unsigned currUserBP = PCB::running->userBP;
	int bp_check = 0;

	// ===================================================== pocetak

	unsigned bp_done = 0;

	for(int i=0; i<PCB::running->stackSize; i++) {

		if(&(PCB::running->stack[i]) == (unsigned*) MK_FP(PCB::running->userSS,currUserBP) && !bp_done){

			unsigned* next =  (unsigned*)MK_FP(PCB::running->userSS,PCB::running->stack[i]);


			if(*next == 0) {
				bp_done = 1;
				continue;
			}


			unsigned long diff = next - (unsigned*) ( MK_FP(PCB::running->userSS,currUserBP) );

			childPCB->stack[i] = FP_OFF((unsigned*)(&(childPCB->stack[i])) + diff);
			currUserBP = PCB::running->stack[i];
			if(bp_check == 0){
				childPCB->bp = FP_OFF(&(childPCB->stack[i]));
				childPCB->sp = childPCB->bp;
				bp_check = 1;
			}

		}else{
			childPCB->stack[i] = PCB::running->stack[i];
		}

	}

	// ============================ kraj

	unsigned uSP = PCB::running->userSP;
	unsigned uSS = PCB::running->userSS;
	unsigned uBP = PCB::running->userBP;

	*(userStackBP + 8) = childPCB->getID();

	unsigned long elem_num = PCB::running->stack + PCB::running->stackSize - userStackBP;

#ifndef BCC_BLOCK_IGNORE
	childPCB->ss = FP_SEG(childPCB->stack+childPCB->stackSize - elem_num + 1);

	*((unsigned*)MK_FP(childPCB->ss,childPCB->bp)+8) = 0x0000;
#endif
	childPCB->sp = childPCB->bp;

	lock;
	Scheduler::put(childPCB);
	unlock;


	*(userStackBP+8) = childPCB->getID(); // podesavam ax

	lock;
	PCB::running->numOfChildrenActive++; // moguce je da je nit vec imala neko dijete
	unlock;

	lock;
	asm mov sp, ss:uBP;
	asm mov ss, ss:uSS;
	asm mov bp, ss:uBP;
	unlock;
}



unsigned rbp;
unsigned rss;

PCB::PCB(Thread* myThread,StackSize stackSize /* velicina u bajtovima */, Time timeSlice){
	this->myThread = myThread;
	this->timeSlice = timeSlice;
	this->numOfChildrenActive = 0;
	this->ready = 0;
	this->finished = 0;
	this->blocked = 0;
	this->started = 0;
	this->fatherID = -1;
	this->semRet = 0;
	this->isWaitingForChildren = 0;

	this->pcbID = ++PCB::gID;
	this->waitingForMeToFinish = new PCBList();
	this->stack = new unsigned[stackSize/sizeof(unsigned)];
	this->stackSize = stackSize/sizeof(unsigned); // velicina u djelovima od po 2 bajta

	// konfigurisi pocetne uslove steka

	this->stack[this->stackSize - 1] = 0x200; // postavljamo PSW ( I = 1 )


#ifndef BCC_BLOCK_IGNORE


	this->stack[this->stackSize - 2] = FP_SEG(&PCB::wrapper);
	this->stack[this->stackSize - 3] = FP_OFF(&PCB::wrapper);

	this->stack[this->stackSize - 12] = 0;

	this->ss = FP_SEG(this->stack + this->stackSize - 12);
	this->sp = FP_OFF(this->stack + this->stackSize - 12);

#endif


	this->bp = this->sp;
}

void PCB::wrapper(){
	PCB::running->myThread->run();

	// odblokiraj sve niti koje cekaju na mene

	/*
	 * Ako ne zakljucam ovu sekciju iz niti 1 moze da se pozove waitToComplete(2) i da se krene blokiranje te niti 1
	 * a u medjuvremenu nit 2 moze da oslobodi svoje niti koje blokira i da zezne nit 1 da ostane zauvijek blokirana
	 */

	PCB::running->finish();
}

void PCB::exit(){
	PCB::running->finish();
}

void PCB::waitForForkChildren(){
	// bez locka se moze desiti da se roditelj blokira i da se poslednje dijete zavrsi
	lock;
		if(PCB::running->numOfChildrenActive != 0){
				running->ready = 0;
				running->blocked = 1;
				running->isWaitingForChildren = 1;
				dispatch();
		}
	unlock;
}


ID PCB::fork(){
	unsigned res = _fork();
	asm mov ax, res;
}

void PCB::finish(){
	// moze neko u toku izvrsavanja finish-a da krene da se doda u waitToCompleteForMe listi
	// tekuce niti
	lock;
		PCBListElem* temp = PCB::running->waitingForMeToFinish->front;
		while(temp != 0){
			temp->pcb->ready = 1; // oznacim sve niti koje su cekale na zavrsetak ove niti kao spremne i stavim ih u rasporedjivac
			temp->pcb->blocked = 0;
			Scheduler::put(temp->pcb);
			temp = temp->next;
		}

		PCB::running->finished = 1;
		PCB::running->blocked = 0;
		PCB::running->ready = 0;
	unlock;
		if(this->fatherID != -1){
			PCB* father = PCB::getThreadById(this->fatherID)->myPCB;
			lock;
			int n = --(father->numOfChildrenActive);
			unlock;
			if(n==0 && father->isWaitingForChildren){
				// odblokiraj oca ako je blokiran zato sto ceka djecu
				father->ready = 1;
				father->blocked = 0;
				father->isWaitingForChildren = 0;
				lock;
				Scheduler::put(father);
				unlock;
			}
		}

		dispatch();
}

void PCB::dispatch(){
	lock;
	Timer::context_switch_on_demand = 1;
	timer();
	unlock;
}

ID PCB::getID(){
	return this->pcbID;
}

Thread* PCB::getThreadById(ID id){
	PCBListElem* temp = allPCB->front;
	while(temp != 0){
		if(temp->pcb->getID() == id) return temp->pcb->myThread;
		temp = temp->next;
	}
	return 0;
}

PCB* PCB::createPCB(Thread* myThread,StackSize stackSize, Time timeSlice){
	lock;
	PCB* myPCB = new PCB(myThread,stackSize,timeSlice);
	allPCB->append(myPCB);
	unlock;
	return myPCB;
}

void PCB::waitToComplete(){
	lock;
	if(!this->finished){
		running->ready = 0;
		running->blocked = 1;

		this->waitingForMeToFinish->append(running);
		dispatch();
	}
	unlock;
}

void PCB::start(){
	lock;
	if(!this->started){
		this->started = 1;
		this->ready = 1;
		Scheduler::put(this);
	}
	unlock;
}

void PCB::threadDestructor(){
	PCB* temp = this;
	lock;
	PCB::allPCB->removeById(temp->getID());
	unlock;
	delete temp;
}

PCB::~PCB(){
	// obrisi stack i obrisi listu waitingForMeToFinish
	PCBListElem* next;
	while(this->waitingForMeToFinish->front != 0){
		next = this->waitingForMeToFinish->front->next;
		delete this->waitingForMeToFinish->front;
		this->waitingForMeToFinish->front = next;
	}
	delete[] stack;
}




