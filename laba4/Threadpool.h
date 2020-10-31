#pragma once
#include <mutex>
#include <Windows.h>
#include "Queue.h"
class Threadpool
{
private:

	std::mutex isworking;
	HANDLE* threads;
	HANDLE finishhandle;
	int threadcnt;
public:
	Threadpool(int threadcount);
	void QueueUserWorkItem(Task task);
	static DWORD WINAPI CreateThreads(LPVOID obj);
	void Stop();
	HANDLE GetFinishHandle();
	Queue queue;
	int ismorework =1;
};