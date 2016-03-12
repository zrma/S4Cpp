#include "stdafx.h"
#include "Log.h"
#include "ThreadLocal.h"
#include <Psapi.h>

namespace S4Framework
{
	void ThreadCallHistory::DumpOut(std::ostream& ost)
	{
		uint64_t count = mCounter < MAX_HISTORY ? mCounter : MAX_HISTORY;

		ost << "===== Recent Call History [Thread:" << mThreadId << "]" << std::endl;

		for (int i = 1; i <= count; ++i)
		{
			ost << "   " << mHistory[(mCounter - i) % MAX_HISTORY] << std::endl;
		}
		ost << "===== End of Recent Call History" << std::endl << std::endl;
	}

	void ThreadCallElapsedRecord::DumpOut(std::ostream& ost)
	{
		uint64_t count = mCounter < MAX_ELAPSED_RECORD ? mCounter : MAX_ELAPSED_RECORD;

		ost << "===== Recent Call Performance [Thread:" << mThreadId << "]" << std::endl;

		for (int i = 1; i <= count; ++i)
		{
			ost << "  FUNC:" << mElapsedFuncSig[(mCounter - i) % MAX_ELAPSED_RECORD]
				<< "ELAPSED: " << mElapsedTime[(mCounter - i) % MAX_ELAPSED_RECORD] << std::endl;
		}
		ost << "===== End of Recent Call Performance" << std::endl << std::endl;

	}

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

	ScopeElapsedCheck::~ScopeElapsedCheck()
	{
		if (LThreadType != THREAD_MAIN)
		{
			LThreadCallElapsedRecord->Append(mFuncSig, GetTickCount64() - mStartTick);
		}
	}

	void PrintMemoryInfo()
	{
		HANDLE hProcess;
		PROCESS_MEMORY_COUNTERS pmc;
		DWORD myProcessId = GetCurrentProcessId();

		std::cout << "\nProcess ID : " << myProcessId << std::endl;

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, myProcessId);
		if (NULL == hProcess)
		{
			return;
		}

		if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
		{	
			std::cout << "\tPageFaultCount : ";
			std::cout << pmc.PageFaultCount << std::endl;

			std::cout << "\tPeakWorkingSetSize : ";
			std::cout << pmc.PeakWorkingSetSize << std::endl;

			std::cout << "\tWorkingSetSize : ";
			std::cout << pmc.WorkingSetSize << std::endl;

			std::cout << "\tQuotaPeakPagedPoolUsage : ";
			std::cout << pmc.QuotaPeakPagedPoolUsage << std::endl;

			std::cout << "\tQuotaPagedPoolUsage : ";
			std::cout << pmc.QuotaPagedPoolUsage << std::endl;

			std::cout << "\tQuotaPeakNonPagedPoolUsage : ";
			std::cout << pmc.QuotaPeakNonPagedPoolUsage << std::endl;

			std::cout << "\tQuotaNonPagedPoolUsage : ";
			std::cout << pmc.QuotaNonPagedPoolUsage << std::endl;

			std::cout << "\tPagefileUsage : ";
			std::cout << pmc.PagefileUsage << std::endl;

			std::cout << "\tPeakPagefileUsage : ";
			std::cout << pmc.PeakPagefileUsage << std::endl;
		}

		CloseHandle(hProcess);
	}
}
