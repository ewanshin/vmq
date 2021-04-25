// vmq_task.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include <queue>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <future>
#include <string>

#include "queue/mpsc-queue.hpp"

#include "actor.h"
#include "player.h"
#include "task.h"
#include "task_manager.h"

const int WORKER_COUNT = 10;			// 100
const int TEST_PLAYER_MAX = 5000;		// 5000
const int TEST_WORKER_PER_PLAYER = 100;	// 100

struct statistics
{
	int complte_count;
};

int main()
{

	std::shared_ptr< task_manager > mgr = std::make_shared<task_manager>();	// task_manager을 shared_ptr로 만들었다
	player_factory factory;

	mgr->set_player_factory(&factory);
	mgr->create_worker(WORKER_COUNT);
	mgr->start();

	while (true)
	{
		std::string str;
		std::cout << ">>";
		std::getline(std::cin, str);

		if (str == "q")
		{
			break;
		}
		else if (str == "a")
		{

			//mgr의 쓰레드에서 안전하게 처리하게 한다.
			mgr->invoke([&mgr] {

				std::cout << "test player create" << std::endl;
				// 테스트용 플레이어 생성
				for (int i = 0; i < TEST_PLAYER_MAX; i++)
				{
					auto player_key = mgr->new_player_key( i);
					mgr->create_player(player_key, nullptr);
				}
			});
		}
		else if (str == "w")
		{
			std::cout << "postee working..." << std::endl;
			std::shared_ptr<statistics > complete_count_ptr = std::make_shared< statistics >();
			complete_count_ptr->complte_count = 0;

			auto thread_id = std::this_thread::get_id();
			std::cout << "mgr working thread id:" << thread_id << std::endl;

			mgr->invoke([&mgr, complete_count_ptr] 
			{

				auto thread_id = std::this_thread::get_id();
				std::cout << "mgr invoke thread_id:" << thread_id << std::endl;

				for (int i = 0; i < TEST_PLAYER_MAX; i++)
				{
					for (int w = 0; w < TEST_WORKER_PER_PLAYER; w++)
					{
						unsigned int delay_time = 1000;
						mgr->send_player_postee( i, [i,w, complete_count_ptr](player_ptr actor, int execute_code)
						{
							if (execute_code != 0)
							{
							}
							else
							{
								std::cout << "[ThreadID:" << std::this_thread::get_id()  << "][Player:" << i << "][WorkID:" << w << "] : run" << std::endl;
								actor->complete_count_++;
								if (actor->complete_count_ == TEST_WORKER_PER_PLAYER)
								{
									//mgr 쓰레드에서 호출된다. ( 어차피 하나의 쓰레드이기때문에 합계 데이터에 걍 완료 체크해둔다.
									actor->send_shared_postee([complete_count_ptr]
									{
										complete_count_ptr->complte_count++;

										if (complete_count_ptr->complte_count == TEST_PLAYER_MAX)
										{
											std::cout << "total_worker end..." << std::endl;
										}

									});
								}
							}

						});
					
					}
				}

			});


		}

	}



    return 0;
}

