#include "Queue.h"
#include <iostream.h>

PCB* Queue::get()
{
	if (size == 0) return 0;

	PCB* pcb = this->front->pcb;
	QueueElem* temp = this->front;

	this->front = this->front->next;
	this->size--;

	if (this->front == 0) this->rear = 0;

	delete temp;
	return pcb;
};

void Queue::put(PCB* pcb, int maxTimeBlocked) {
	QueueElem* temp = new QueueElem();
	temp->pcb = pcb;
	temp->next = 0;
	temp->maxTimeBlocked = maxTimeBlocked;

	if (this->rear == 0) {
		this->rear = this->front = temp;
		this->size++;
		return;
	}

	this->rear->next = temp;
	this->rear = this->rear->next;
	this->size++;
}

void Queue::deleteElem(PCB* pcb)
{
	QueueElem* current;
	QueueElem* prev;
	QueueElem* next;

	current = this->front;
	next = current->next;
	prev = 0;

	while (current->pcb != pcb) {
		prev = current;
		current = next;
		next = next->next;
	}

	if (current->next == 0 && prev == 0) {
		this->front = this->rear = 0;
		delete current;
		this->size--;
		return;
	}

	if (current->next == 0) {
		this->rear = prev;
		this->rear->next = 0;
		this->size--;
		delete current;
		return;
	}

	if (prev == 0) {
		this->front = next;
		delete current;
		this->size--;
		return;
	}

	this->size--;
	prev->next = current->next;
	delete current;
}


void Queue::deleteQueue()
{
	if (this->front != 0) {
		QueueElem* temp = this->front;
		while (this->front != 0) {
			this->front = this->front->next;
			delete temp;
			temp = this->front;
		}
	}
	this->rear = 0;
	this->size = 0;
}
