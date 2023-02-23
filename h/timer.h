/*
 * timer.h
 *
 *  Created on: Apr 30, 2021
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

void interrupt timer();
void inic();
void restore();

class Timer{
public:
	static volatile int counter;
	static volatile int context_switch_on_demand;
};


#endif /* TIMER_H_ */
