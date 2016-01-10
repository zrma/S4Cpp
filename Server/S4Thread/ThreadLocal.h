#pragma once

#include <thread>

namespace S4Thread
{
	enum THREAD_TYPE
	{
		THREAD_MAIN = 0,
		THREAD_IO_WORKER,
		THREAD_DB_WORKER,
		THREAD_WORLD
	};

	class LockOrderChecker;

	extern thread_local int LThreadType;
	extern thread_local int LThreadId;
	extern thread_local int LTickCount;

	extern thread_local LockOrderChecker* LLockOrderChecker;
}