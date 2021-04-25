#include <queue>
#include <future>
#include <unordered_map>
#include "queue/mpsc-queue.hpp"

#include "actor.h"
#include "player.h"


player::player() :key_(player_key_type())
{
	complete_count_ = 0;
}


player::~player()
{
}


void player::send_shared_postee(shared_postee && postee)
{
	if (parent_actor_ptr != nullptr)
	{
		parent_actor_ptr->invoke(postee);
	}
}


