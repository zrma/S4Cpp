#pragma once

#include <thread>

namespace S4Network
{
	enum class THREAD_TYPE : int
	{
		THREAD_MAIN,
		THREAD_IO_WORKER,
		THREAD_DB_WORKER,
		THREAD_WORLD
	};

	extern thread_local int LThreadType;
	extern thread_local int LThreadId;
	extern thread_local int LTickCount;
}