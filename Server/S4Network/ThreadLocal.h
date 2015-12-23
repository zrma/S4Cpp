#pragma once

#include <boost/thread/tss.hpp>

namespace S4Network
{
	enum class THREAD_TYPE : int
	{
		THREAD_MAIN,
		THREAD_IO_WORKER,
		THREAD_DB_WORKER,
		THREAD_WORLD
	};

	extern boost::thread_specific_ptr<int> LThreadType;
	extern boost::thread_specific_ptr<int> LThreadId;
	extern boost::thread_specific_ptr<int64_t> LTickCount;

	extern void Init();
}