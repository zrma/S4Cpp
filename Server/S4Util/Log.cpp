#include "stdafx.h"
#include "Log.h"

namespace S4Util
{
	LogEvent gLogEvents[MAX_LOG_SIZE];
	__int64 gCurrentLogIndex = 0;

	void EventLogDumpOut(std::ostream& ost)
	{
		__int64 count = gCurrentLogIndex < MAX_LOG_SIZE ? gCurrentLogIndex : MAX_LOG_SIZE;

		ost << "===== Recent Sequential Event Log =====" << std::endl;

		for (int i = 1; i <= count; ++i)
		{
			const LogEvent& log = gLogEvents[(gCurrentLogIndex - i) % MAX_LOG_SIZE];
			ost << "TID[" << log.mThreadId << "] MSG[ " << log.mMessage << " ] INFO [" << log.mAdditionalInfo << "]" << std::endl;
		}

		ost << "===== End of Event Log =====" << std::endl;
	}
}
