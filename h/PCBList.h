/*
 * PCBList.h
 *
 *  Created on: Apr 30, 2021
 *      Author: OS1
 */

#ifndef _PCBLIST_H_
#define _PCBLIST_H_

#include "pcb.h"

struct PCBListElem{
	PCB* pcb;
	PCBListElem* next;
	PCBListElem* prev;
};

class PCBList{
public:
	int size;
	PCBListElem* front;  	// points to first node of list
	PCBListElem* end;   	// points to first last of list
	PCBList(){
		this->front = 0;
		this->end = 0;
		this->size = 0;
	};
	void append(PCB* pcb);
	PCB* findById(int id);
	void remove(PCB* pcb);
	void removeById(int id);
	void printID();
	void deleteRemainingPCB(); // da obrise stek niti djece
};


#endif /* PCBLIST_H_ */
