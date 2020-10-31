#pragma once
#include <queue>
#include <functional>
typedef std::function<void()> Task;
class Queue
{
private: 
	std::queue<Task>* queue = new std::queue<Task>;
public:
	void push(Task task);
	Task pop();
	bool empty();
	void TEST();
};
