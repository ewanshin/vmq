#include <queue>
#include <iostream>
#include <future>
#include <unordered_map>
#include "queue/mpsc-queue.hpp"


#include "actor.h"


actor::actor()
{

}


actor::~actor()
{

}

int actor::execute_task( int pop_count )
{
/*	std::chrono::time_point<std::chrono::system_clock> now;
	time_t now_time = std::chrono::system_clock::to_time_t(now);
	now = std::chrono::system_clock::now();

	int execute_count = 0;
	for (int i = 0; i < pop_count; i++)
	{
		std::function<void()> task;

		{
			std::unique_lock<std::mutex> lock(this->queue_mutex_);

			for (auto elem : delay_queue_)
			{
				if (elem.first > now_time)
				{
					// 나머진 다음에 체크
					std::cout << "check others next time" << std::endl;
					break;
				}

				// 작업이 없으면 패스
				if (true == elem.second.empty())
				{
					std::cout << "nothing to check" << std::endl;
					continue;
				}

				task = std::move(elem.second.front());
				elem.second.pop();

			}

			//if (delay_queue_.at(0).empty())
			//	break;

			//task = std::move(this->delay_queue_.at(0).front());
			//delay_queue_.at(0).pop();
		}

		if (task != nullptr)
		{
			try
			{
				task();
                execute_count++;
			}
			catch (const std::bad_function_call&)
			{
				std::cout << "bad function call" << std::endl;
			}
			catch (...)
			{
				std::cout << "unknown exception" << std::endl;
			}

		}
	}

	return execute_count;*/
	int execute_count = 0;
	for (int i = 0; i < pop_count; i++)
	{
		std::function<void()> task;

#if QUEUE_TYPE == LOCK_QUEUE
		{
			std::unique_lock<std::mutex> lock(this->queue_mutex_);

			if (task_queue_.empty())
				break;

			task = std::move(this->task_queue_.front());
			task_queue_.pop();
		}
#else
		if (task_queue_.dequeue(task) == false)
			break;

#endif

		if (task != nullptr)
		{
			try
			{
				task();
			}
			catch (const std::bad_function_call&)
			{
				std::cout << "bad function call" << std::endl;
			}
			catch (...)
			{
				std::cout << "unknown exception" << std::endl;
			}

		}
	}

	return execute_count;
}
