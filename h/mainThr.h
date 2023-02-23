/*
 * mainThr.h
 *
 *  Created on: May 1, 2021
 *      Author: OS1
 */

#ifndef MAINTHR_H_
#define MAINTHR_H_


#include "thread.h"

class MainThread : public Thread{
public:
	static Thread* mainThread;
	MainThread();
	void run(){}
	~MainThread(){}
};



#endif /* MAINTHR_H_ */
