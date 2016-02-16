#pragma once

#include <thread>

#define MAX_IO_THREAD		4
#define MAX_DB_THREAD		4
#define MAX_WORKER_THREAD	(MAX_IO_THREAD + MAX_DB_THREAD)

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
	
	extern thread_local void* LRecentThisPointer;
	extern thread_local int LTickCount;

	extern thread_local LockOrderChecker* LLockOrderChecker;
}

namespace S4Util
{
	class ThreadCallHistory;
	class ThreadCallElapsedRecord;

	extern thread_local ThreadCallHistory* LThreadCallHistory;
	extern thread_local ThreadCallElapsedRecord* LThreadCallElapsedRecord;

	extern ThreadCallHistory* GThreadCallHistory[MAX_WORKER_THREAD];
	extern ThreadCallElapsedRecord* GThreadCallElapsedRecord[MAX_WORKER_THREAD];
}