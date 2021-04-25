#ifndef __task_H__
#define __task_H__

class task_manager;

class task : public actor
{

public:
	task(task_manager* task_mgr) :task_manager_(task_mgr), terminated_(false), worker_count_per_sleep_(10), pop_postee_per_actor_{ 100 } {}
	virtual ~task();

	void start();
	void stop();

	void add_player( player_key_type key, player_ptr actor );

private:
	void do_work();

	volatile bool										terminated_;
	task_manager*								task_manager_;
	std::shared_ptr< std::thread >						thread_ptr_;

	std::unordered_map< player_key_type, actor_ptr >	actor_map_;
	
	int													worker_count_per_sleep_;
	int													pop_postee_per_actor_;			// 액터당 한번에 뽑는 Postee 갯수
};
typedef std::shared_ptr<  task > task_ptr;

#endif	//#define __task_H__
