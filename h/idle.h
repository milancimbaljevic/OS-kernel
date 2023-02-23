/*
 * idle.h
 *
 *  Created on: Apr 30, 2021
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h"
#include <iostream.h>

#define lock asm cli
#define unlock asm sti

class idle : public Thread {
public:
public:
	static int idleTest;
	static idle* idleThread;
	idle() : Thread(defaultStackSize,1) {}
	virtual void run();
};

#endif /* IDLE_H_ */
