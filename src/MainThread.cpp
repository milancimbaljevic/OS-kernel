/*
 * MainThread.cpp
 *
 *  Created on: May 1, 2021
 *      Author: OS1
 */
#include "mainThr.h"

Thread* MainThread::mainThread = 0;
MainThread::MainThread() : Thread(defaultStackSize,defaultTimeSlice) {}
