// vmq_task.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
//#include "spdlog/spdlog.h"
#include "logger/logger.h"
#include "logger/lplog_import.h"

constexpr int kTestPlayerMax = 5000;
constexpr int kTestWorkerPerPlayer = 100;

struct Statistics
{
	int complete_count;
};

int main()
{
	//using namespace std::placeholders;  // for _1, _2, _3...

	auto mgr = std::make_shared<player_task_manager>();
	player_factory factory;

	logger logger_;
	logger_.start("vmq.log", log_level::level_info);
	logger_.info("Start");

	mgr->set_player_factory(&factory);
	mgr->create_worker(100);
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
			logger_.info("create player");
			// mgr의 쓰레드에서 안전하게 처리하게 한다.
			mgr->invoke([mgr] {
				for (int i = 0; i < kTestPlayerMax; i++)
				{
					auto player_key = mgr->new_player_key(i);
					mgr->create_player(player_key, nullptr);
				}
			});
		}
		else if (str == "w")
		{
			logger_.info("start work");	
			auto complete_count_ptr = std::make_shared<Statistics>();
			complete_count_ptr->complete_count = 0;

			mgr->invoke([mgr, complete_count_ptr, &logger_] {
				for (int i = 0; i < kTestPlayerMax; i++)
				{
					for (int w = 0; w < kTestWorkerPerPlayer; w++)
					{
						mgr->send_player_postee(i, [i, w, complete_count_ptr, &logger_](player_ptr actor, int execute_code) {
							if (execute_code != 0)
							{
								// 에러 처리 로직 추가 가능
							}
							else
							{
								logger_.debug("[", i, ", ", w, "] : run");
								actor->complete_count_++;
								if (actor->complete_count_ == kTestWorkerPerPlayer)
								{
									// mgr 쓰레드에서 호출된다. (어차피 하나의 쓰레드이기 때문에 합계 데이터에 완료 체크해둔다.)
									actor->send_shared_postee([complete_count_ptr, &logger_] {
										complete_count_ptr->complete_count++;
										if (complete_count_ptr->complete_count == kTestPlayerMax)
										{
											logger_.info("total_worker end...");
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
