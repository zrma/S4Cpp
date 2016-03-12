#include "stdafx.h"
#include "Log.h"
#include "ThreadLocal.h"

namespace S4Framework
{
	thread_local int LThreadType = -1;
	thread_local int LThreadId = -1;
	thread_local int LTickCount = 0;
	thread_local void* LRecentThisPointer = nullptr;

	thread_local std::shared_ptr<ThreadCallHistory> LThreadCallHistory;
	thread_local std::shared_ptr<ThreadCallElapsedRecord> LThreadCallElapsedRecord;

	SLIST_HEADER GThreadCallHistory;
	SLIST_HEADER GThreadCallElapsedRecord;
}
