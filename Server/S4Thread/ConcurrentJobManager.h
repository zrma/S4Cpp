#pragma once

#include "IConcurrentPool.h"
#include "ThreadLocal.h"

namespace S4Thread
{
	class ConcurrentJobManager : IConcurrentPool
	{
	public:
		ConcurrentJobManager(std::size_t size = MAX_LOGIC_THREAD);
		~ConcurrentJobManager();
	};

}
