/*
 * idle.cpp
 *
 *  Created on: Apr 30, 2021
 *      Author: OS1
 */

#include "idle.h"
#include "SCHEDULE.h"

int idle::idleTest = 0;
idle* idle::idleThread = 0;

void idle::run(){
	int i = 0;
	while(i<10) {
		i++;
		i=0;
	}
}
