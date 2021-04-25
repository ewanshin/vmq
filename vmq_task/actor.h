#ifndef __ACTOR_H__
#define __ACTOR_H__

#define POP_TASK_COUNT 1


typedef unsigned long long delay_queue_key;
typedef std::queue< std::function<void()> >	task_queue_type;
class actor
{
public:
	actor();
	virtual ~actor();

	template<class F, class... Args>
	auto invoke( F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>;
	inline void set_parent(std::shared_ptr< actor > parent) 
	{ 
		parent_actor_ptr = parent; 
	}
	int execute_task(int pop_count = POP_TASK_COUNT );

public:

	task_queue_type				task_queue_;
	// 타이머를 지원하는 task manager를 만드려면 어떻게 하는게 맞을까?
//	std::unordered_map <time_t, task_queue_type >	delay_queue_;	// 각 시간별로의 task queue
	std::mutex										queue_mutex_;

	std::shared_ptr< actor >					parent_actor_ptr;
};

typedef std::shared_ptr< actor > actor_ptr;


template<class F, class... Args>
auto actor::invoke(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;

	auto task = std::make_shared< std::packaged_task<return_type()> >
		(
			// ToDo 일단 queue에서 빼봤다가 시간값이 안맞으면 다시 queue에 집어넣는다?? 역전될수 있잖아!!! (불가!)
			// Queue를 시간우선으로 설계한다??
			// 복수의 queue, 0 시간인 녀석, 해당 시간인 녀석으로 이중화 한다?? -> 이게 가장 현실적이긴 한데...
			// 데이터 구조가 복잡해지는 이슈가 있겠지.. queue를 wrapping ㅛ

			//템플릿 함수는 우측값 참조를 추론한다. f.args가 좌측값이어도 컴파일 에러가 안난다.
			//forward는 템플릿 함수에서 좌측값은 촤측값, 우측값은 우측값으로 바꾼다. ( 원래의 형태 )
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);

	std::future<return_type> res = task->get_future();

	task_queue_.emplace([task]() { (*task)(); });
//	delay_queue_.at(0).emplace([task]() { (*task)(); });
//	delay_queue_.at(0).emplace([task]() { (*task)(); });

	//결과에 get해서 wait을 사용할수 있다.
	return res;
}

#endif  //#define __ACTOR_H__
