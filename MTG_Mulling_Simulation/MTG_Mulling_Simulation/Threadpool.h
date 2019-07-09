#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>

typedef std::function<void()> ThreadWork;

class Threadpool
{
public:
	Threadpool();
	~Threadpool();

	void Init(const unsigned int aThreadCount);
	void Shutdown();

	void QueueWork(const ThreadWork& aWorkFunc);

	void AssistWork();

private:

	std::condition_variable myTaskConditional;
	std::mutex myWorkQueueMutex;

	std::vector<std::thread> myThreads;
	std::queue<ThreadWork> myWorkQueue;

	bool myShouldStop;
	std::atomic_int myActiveThreadCount;
};

inline Threadpool::Threadpool()
{
	myShouldStop = false;
	myActiveThreadCount = 0;
}

inline Threadpool::~Threadpool()
{

}

inline void Threadpool::Init(const unsigned int aThreadCount)
{
	const unsigned int workerCount = aThreadCount < (std::thread::hardware_concurrency() - 2) ? aThreadCount : (std::thread::hardware_concurrency() - 2);

	for (unsigned int i = 0; i < workerCount; ++i)
	{
		myThreads.emplace_back([this]
		{
			
			ThreadWork dispatchable;
			{
				std::unique_lock workLock(this->myWorkQueueMutex);
				this->myTaskConditional.wait(workLock, [this] { return (this->myShouldStop) || (!this->myWorkQueue.empty()); });

				if (this->myShouldStop && this->myWorkQueue.empty())
				{
					return;
				}

				myActiveThreadCount++;

				dispatchable = std::move(this->myWorkQueue.front());
				this->myWorkQueue.pop();
			}
			
			dispatchable();
			myActiveThreadCount--;
		});
	}
}

inline void Threadpool::Shutdown()
{
	{
		std::unique_lock lock(myWorkQueueMutex);
		myShouldStop = true;
	}

	myTaskConditional.notify_all();
	for (std::thread& t : myThreads)
	{
		t.join();
	}
}

inline void Threadpool::QueueWork(const ThreadWork & aWorkFunc)
{
	std::unique_lock lock(myWorkQueueMutex);
	myWorkQueue.emplace(aWorkFunc);
	myTaskConditional.notify_one();
}

inline void Threadpool::AssistWork()
{
	while (!myWorkQueue.empty())
	{
		ThreadWork dispatchable;

		{
			std::unique_lock workLock(this->myWorkQueueMutex);
			this->myTaskConditional.wait(workLock, [this] { return (this->myShouldStop) || (!this->myWorkQueue.empty()); });

			if (this->myShouldStop && this->myWorkQueue.empty())
			{
				return;
			}

			dispatchable = std::move(this->myWorkQueue.front());
			this->myWorkQueue.pop();
		}
		
		dispatchable();
	}
}
