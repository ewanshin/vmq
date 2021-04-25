#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <future>

#include "queue/mpsc-queue.hpp"

#include "actor.h"
#include "player.h"
#include "task.h"
#include "task_manager.h"


task_manager::task_manager() :terminated_(false)
{

}


task_manager::~task_manager()
{
	stop();

	if (thread_ptr_.get() != nullptr)
	{
		thread_ptr_->join();
	}
}

void task_manager::start()
{
	thread_ptr_ = std::make_shared< std::thread >(&task_manager::do_work, this);
}

void task_manager::stop()
{
	terminated_ = true;
}


void task_manager::do_work()
{
	int execute_count = 0;

	while (terminated_ == false)
	{
		execute_count = execute_task();

		if (execute_count == 0)
		{
			// 이거 항상 들어올것 같은데?? execute_count가 항상 0인거 아냐??
//            std::cout << "execute count:" << execute_count << std::endl;

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		} else{
            std::cout << "execute count:" << execute_count << std::endl;
        }
	}
}

void task_manager::create_worker( int worker_size )
{

	for (int i = 0; i < worker_size; ++i)
	{
		auto th = std::make_shared< task>( this );
		th->start();

		workers_.push_back(th);
	}
}

player_ptr task_manager::create_player(player_key_type player_key, player_postee && f_postee)
{
	auto wsize = workers_.size();

	// worker의 사이즈만큼 나눠서 분배
	int worker_key = player_key % wsize;

	auto player = player_ptr(factory_->create());	// 팩토리를 통해 플레이어 생성
	auto this_ptr = shared_from_this();

	player->set_parent(this_ptr);
	player->set_key(player_key);

	auto iter = actor_map_.find(player_key);
	if (iter == actor_map_.end())
	{
		actor_map_.insert({ player_key, player });
	}

	//여기서 생성해서 넣는다.
	//  worker_key의 워커에 player가 f_postee 를 호출한 게 된다
	unsigned int delay_time = 0;
	send_worker_postee(worker_key, [player_key, player, f_postee, delay_time](task_ptr worker)
	{
		worker->add_player(player_key, player);

		if (f_postee != nullptr)
		{
			f_postee(player, 0);
		}
	});

	return player;
}

void task_manager::send_worker_postee(int worker_key, worker_postee && postee)
{
	auto worker = workers_[worker_key];
	worker->invoke(postee, worker);
}

void task_manager::send_player_postee(player_key_type key, player_postee && postee)
{
	//넣는건 직접 넣어버린다.
	auto iter = actor_map_.find(key);
	if (iter == actor_map_.end())
	{
		if (postee != nullptr)
		{
			postee(nullptr, -1);
		}
	}
	else
	{
		auto& actor = iter->second;
		actor->invoke( postee, actor, 0);
	}
}
