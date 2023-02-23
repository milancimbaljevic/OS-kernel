#include "PCBList.h"
#include <iostream.h>

void PCBList::append(PCB* pcb) {
	PCBListElem* pcbElem = new PCBListElem();
	pcbElem->pcb = pcb;
	pcbElem->next = pcbElem->prev = 0;

	if (this->front == 0) {
		this->front = this->end = pcbElem;
		pcbElem->next = pcbElem->prev = 0;
		this->size++;
		return;
	}

	this->end->next = pcbElem;
	pcbElem->next = 0;
	pcbElem->prev = this->end;
	this->end = pcbElem;

	this->size++;

	return;
}

void PCBList::remove(PCB* pcb) {
	if (this->front == 0) return;
	PCBListElem* temp = this->front;
	while (temp != 0) {
		if (temp->pcb->getID() == pcb->getID()) {
			// obrisi element

			if (temp->prev == 0) {
				if (temp->next == 0) {
					this->front = this->end = 0;
					delete temp;
					this->size--;
					return;
				}
				else {
					this->front = temp->next;
					this->front->prev = 0;
				}
				this->size--;
				delete temp;
				return;
			}

			if (temp->next == 0) {
				this->end = temp->prev;
				this->end->next = 0;
				this->size--;
				delete temp;
				return;
			}

			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			this->size--;
			delete temp;
			return;
		}
		temp = temp->next;
	}
	return;
}

void PCBList::removeById(int id) {
	if (this->front == 0) return;
		PCBListElem* temp = this->front;
		while (temp != 0) {
			if (temp->pcb->getID() == id) {
				// obrisi element

				if (temp->prev == 0) {
					if (temp->next == 0) {
						this->front = this->end = 0;
						delete temp;
						this->size--;
						return;
					}
					else {
						this->front = temp->next;
						this->front->prev = 0;
					}
					this->size--;
					delete temp;
					return;
				}

				if (temp->next == 0) {
					this->end = temp->prev;
					this->end->next = 0;
					this->size--;
					delete temp;
					return;
				}

				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				this->size--;
				delete temp;
				return;
			}
			temp = temp->next;
		}
		return;

}

PCB* PCBList::findById(int id){
	PCBListElem* temp = this->front;
	while(temp != 0){
		if(temp->pcb->getID() == id) return temp->pcb;
		temp = temp->next;
	}
	return 0;
}

void PCBList::printID() {
	PCBListElem* temp = this->front;
	while (temp != 0) {
		temp = temp->next;
	}
}
