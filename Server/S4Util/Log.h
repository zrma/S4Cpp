#pragma once

#include <iostream>

namespace S4Thread
{
	extern thread_local int LThreadId;
}

namespace S4Util
{
	struct LogEvent
	{
		int mThreadId = -1;
		int	mAdditionalInfo = 0;
		const char* mMessage = nullptr;
	};

#define MAX_LOG_SIZE  65536   ///< Must be a power of 2

	extern LogEvent gLogEvents[MAX_LOG_SIZE];
	extern __int64 gCurrentLogIndex;

	inline void EventLog(const char* msg, int info)
	{
		__int64 index = _InterlockedIncrement64(&gCurrentLogIndex) - 1;
		LogEvent& event = gLogEvents[index & (MAX_LOG_SIZE - 1)];
		event.mThreadId = S4Thread::LThreadId;
		event.mMessage = msg;
		event.mAdditionalInfo = info;
	}

	void EventLogDumpOut(std::ostream& ost = std::cout);
}
