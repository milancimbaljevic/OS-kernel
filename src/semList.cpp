/*
 * semList.cpp
 *
 *  Created on: May 4, 2021
 *      Author: OS1
 */

#include "semList.h"
#include <iostream.h>

void SemaphoreList::put(KernelSem* sem) {
	SemaphoreListElem* temp = new SemaphoreListElem();
	temp->semaphore = sem;
	temp->next = 0;

	if (this->rear == 0) {
		this->rear = this->front = temp;
		this->size++;
		return;
	}

	this->rear->next = temp;
	this->rear = this->rear->next;
	this->size++;
}

void SemaphoreList::deleteElem(KernelSem* sem)
{
	SemaphoreListElem* current;
	SemaphoreListElem* prev;
	SemaphoreListElem* next;

	current = this->front;
	next = current->next;
	prev = 0;

	while (current->semaphore != sem) {
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

void SemaphoreList::deleteList()
{
	if (this->front != 0) {
		SemaphoreListElem* temp = this->front;
		while (this->front != 0) {
			this->front = this->front->next;
			delete temp;
			temp = this->front;
		}
	}
	this->rear = 0;
	this->size = 0;
}
