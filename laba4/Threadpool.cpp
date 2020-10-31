#include "Threadpool.h"
#include <iostream>
Threadpool::Threadpool(int threadcount)
{

	isworking.lock();
	threadcnt = threadcount;
	finishhandle = CreateThread(NULL, 0, &CreateThreads, this, NULL, NULL);
}
DWORD WINAPI ProcessQueueItem(LPVOID obj) {
	Threadpool* self = (Threadpool*)obj;
	Queue* tasks = (&self->queue);
	Task tmp;
	while ( !(  (!self->ismorework)  &&  (tasks->empty())  ) )
	{
		if ((tmp = tasks->pop()) != NULL) {
			tmp();
		}
	}
	ExitThread(0);
	return 0;
}
void Threadpool::Stop()
{
	ismorework = 0;
}
DWORD WINAPI Threadpool::CreateThreads(LPVOID obj) 
{
	Threadpool* self = (Threadpool*)obj;
	int threadcount = self->threadcnt;
	
	self->threads = (HANDLE*)malloc(sizeof(HANDLE) * threadcount);
	for (int i = 0; i < threadcount; i++)
		self->threads[i] = CreateThread(NULL, 0, &ProcessQueueItem, obj, NULL, NULL);
	WaitForMultipleObjects(threadcount, self->threads, TRUE, INFINITE);
	std::cout << "Main threadpool thread " << GetCurrentThreadId() << " stops "<< std::endl;
	//Sleep(1000);
	ExitThread(0);
	return 0;
}
void Threadpool::QueueUserWorkItem(Task task)
{
	queue.push(task);
}
HANDLE Threadpool::GetFinishHandle()
{
	return finishhandle;
}