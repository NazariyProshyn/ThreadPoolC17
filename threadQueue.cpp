#include "threadQueue.h"


void ThreadQueue::push(std::function<void()> funct)
{
	std::lock_guard<std::mutex> lock(mut);
	dataQueue.push(std::move(funct));
	dataCondition.notify_one();
}


std::function<void()> ThreadQueue::pop()
{
	std::unique_lock<std::mutex> lock(mut);
	dataCondition.wait(lock, [this] {return !dataQueue.empty(); });

	std::function<void()> value = std::move(dataQueue.front());
	dataQueue.pop();
	return value;
}


size_t ThreadQueue::size() const
{
	std::lock_guard<std::mutex> lock(mut);
	return dataQueue.size();
}
