/*
 * timer.cpp
 *
 *  Created on: Apr 30, 2021
 *      Author: OS1
 */
#include "timer.h"
#include "pcb.h"
#include "idle.h"
#include "PCBList.h"
#include "SemList.h"
#include "kernSem.h"
#include "Queue.h"
#include <dos.h>

unsigned oldTimerOFF, oldTimerSEG;

volatile int Timer::counter = defaultTimeSlice;
volatile int Timer::context_switch_on_demand = 0;

unsigned tsp;
unsigned tss;
unsigned tbp;

#include <iostream.h>

unsigned tempRet;

PCB* temp = 0;

extern void tick();

void interrupt timer(){

	// prodji kroz listu svih semafora i ako je nekoj niti isteklo vrijeme blokiranosti odblokiraj je

	if(!Timer::context_switch_on_demand){

		asm int 0x60;

		tick();

		SemaphoreListElem* semHead = semHead = KernelSem::allSem->front;// nova izmjena
		QueueElem* blockHead = 0;

		while(semHead != 0){

			blockHead = semHead->semaphore->blocked->front; // nova izmjena

			while(blockHead != 0){
				if(blockHead->maxTimeBlocked != 0){
					blockHead->maxTimeBlocked--;
					if(blockHead->maxTimeBlocked == 0){
						temp = blockHead->pcb;
						QueueElem* n = blockHead->next;
						blockHead->pcb->blocked = 0;
						blockHead->pcb->semRet = -1;
						Scheduler::put(blockHead->pcb);
						semHead->semaphore->blocked->deleteElem(temp);
						semHead->semaphore->value++;
						//if(semHead->semaphore->value > semHead->semaphore->init) semHead->semaphore->value = semHead->semaphore->init;
						blockHead = n;
					}else{
						blockHead = blockHead->next;
					}
				}else{
					blockHead = blockHead->next;
				}


			}

			semHead = semHead->next;
		}

	}

	if (!Timer::context_switch_on_demand) Timer::counter--;
	if (Timer::counter == 0 || Timer::context_switch_on_demand) {

		asm{
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if(PCB::running->ready == 1 && !PCB::running->finished && !PCB::running->blocked) Scheduler::put(PCB::running);

		// scheduler
		PCB::running = Scheduler::get();



		if(PCB::running == 0) PCB::running = PCB::allPCB->findById(idle::idleThread->getId());

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		Timer::counter = PCB::running->timeSlice;

		asm{
			// restaurira sp
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
	}

	if(Timer::counter < 0) Timer::counter = 0;
	Timer::context_switch_on_demand = 0;

}

void inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax // es = 0

		// pamti staru rutinu
		mov ax, word ptr es:0x0022
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0x0020
		mov word ptr oldTimerOFF, ax

		// postavlja novu rutinu
		mov word ptr es:0x0022, seg timer
		mov word ptr es:0x0020, offset timer

		// postavlja staru rutinu
       		// na int 60h - zasto?
		mov ax, oldTimerSEG
		mov word ptr es:0x0182, ax
		mov ax, oldTimerOFF
		mov word ptr es:0x0180, ax
 
		pop ax
		pop es
		sti
	}
}

void restore(){
	asm {
		cli
		push es
		push ax
 
		mov ax,0
		mov es,ax
 
		mov ax, word ptr oldTimerSEG
		mov word ptr es:0x0022, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0x0020, ax
 
		pop ax
		pop es
		sti
	}
}
