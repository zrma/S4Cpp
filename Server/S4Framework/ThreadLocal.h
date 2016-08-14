#pragma once

#include <thread>

namespace S4Framework
{
	enum THREAD_TYPE
	{
		THREAD_MAIN = 0,
		THREAD_IO_WORKER,
		THREAD_DB_WORKER,
		THREAD_WORLD
	};

	extern thread_local int LThreadType;
	extern thread_local int LThreadId;
	extern thread_local int LTickCount;
	extern thread_local void* LRecentThisPointer;

	class ThreadCallHistory;
	class ThreadCallElapsedRecord;

	extern thread_local std::shared_ptr<ThreadCallHistory> LThreadCallHistory;
	extern thread_local std::shared_ptr<ThreadCallElapsedRecord> LThreadCallElapsedRecord;

	extern SLIST_HEADER GThreadCallHistory;
	extern SLIST_HEADER GThreadCallElapsedRecord;
}