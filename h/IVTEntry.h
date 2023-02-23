/*
 * IVTEntry.h
 *
 *  Created on: May 5, 2021
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "KernelEv.h"

#define PREPAREENTRY(ivtNo, callOldRoutine)\
void interrupt interruptRoutine##ivtNo(...);\
IVTEntry entry##ivtNo(ivtNo, interruptRoutine##ivtNo);\
void interrupt interruptRoutine##ivtNo(...) {\
	entry##ivtNo.signal();\
	if (callOldRoutine == 1)\
		entry##ivtNo.oldRoutine();\
}

typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

class IVTEntry{
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newInterruptRoutine);
	void oldRoutine();
	void signal();
	~IVTEntry();
private:
	KernelEv* ev;
	pInterrupt oldInterruptRoutine;
	pInterrupt newInterruptRoutine;
	IVTNo ivtNo;
};

#endif /* IVTENTRY_H_ */
