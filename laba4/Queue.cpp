#include "Queue.h"
#include <mutex>
#include <iostream>
#include <Windows.h>
std::mutex mutex;

void Queue::push(Task task) 
{
	mutex.lock();
	queue->push(task);
	mutex.unlock();
}
Task Queue::pop()
{
	mutex.lock();
	Task result;
	if (queue->empty()) {
		result = NULL;
	}
	else
	{
		result = queue->front();
		queue->pop();
	}
	mutex.unlock();
	return result;
}
void Queue::TEST()
{
	//mutex.lock();
	std::cout << "Thred with id "<< GetCurrentThreadId()<<std::endl;	
	//mutex.unlock();
}
bool Queue::empty()
{
	return queue->empty();
}