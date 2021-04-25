#ifndef __task_manager_H__
#define __task_manager_H__

#define VMQ_PLAYER_COUNT				(USHRT_MAX)
#define VMQ_FIELD_COUNT					(USHRT_MAX)
#define VMQ_SERVICE_COUNT				(USHRT_MAX)

typedef std::function<void(player_ptr, int execute_code )>	player_postee;
typedef std::function<void(task_ptr)>				worker_postee;

class task_manager : public actor, public  std::enable_shared_from_this<actor>
{
	
public:
	task_manager();
	virtual ~task_manager();
	void start();
	void stop();
	void set_player_factory(player_factory* factory) { factory_ = factory; }
	
	player_key_type new_player_key(int n) { return n; }

	void create_worker(int worker_size );
	player_ptr create_player(player_key_type player_key, player_postee&& f_postee);

	// ToDo: worker_postee와 player_postee의 차이는 무엇인가?
	void send_worker_postee(int worker_key, worker_postee&& postee);	// 결국 이건 내부 호출용이지 외부에서 호출할때는 player_postee가 되어야 할 것 같은데?
	void send_player_postee(player_key_type key, player_postee&& postee);

private:
	void do_work();

	player_factory*											factory_;
	volatile bool											terminated_;
	std::shared_ptr< std::thread >							thread_ptr_;
	std::vector< task_ptr >									workers_;
	std::unordered_map< player_key_type, player_ptr >		actor_map_;
	
};

#endif	//#define __task_manager_H__
