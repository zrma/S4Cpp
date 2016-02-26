#include "stdafx.h"
#include "../S4Util/Log.h"
#include "ThreadLocal.h"

namespace S4Thread
{
	thread_local int LThreadType = -1;
	thread_local int LThreadId = -1;
	thread_local void* LRecentThisPointer = nullptr;

	thread_local int LTickCount = 0;
}

namespace S4Util
{
	thread_local ThreadCallHistory* LThreadCallHistory = nullptr;
	thread_local ThreadCallElapsedRecord* LThreadCallElapsedRecord = nullptr;

	ThreadCallHistory* GThreadCallHistory[MAX_WORKER_THREAD] = { 0, };
	ThreadCallElapsedRecord* GThreadCallElapsedRecord[MAX_WORKER_THREAD] = { 0, };
}
