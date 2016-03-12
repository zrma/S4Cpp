#pragma once

#include <iostream>
#include <boost/log/trivial.hpp>

namespace S4Framework
{
	extern thread_local int LThreadId;

	class ThreadCallHistory : SLIST_ENTRY
	{
	public:
		ThreadCallHistory(int tid) : mThreadId(tid)
		{
			memset(mHistory, 0, sizeof(mHistory));
		}

		inline void Append(const char* funsig)
		{
			mHistory[mCounter++ % MAX_HISTORY] = funsig;
		}

		void DumpOut(std::ostream& ost = std::cout);

	private:
		enum
		{
			MAX_HISTORY = 1024
		};

		uint64_t	mCounter = 0;
		int			mThreadId = -1;
		const char*	mHistory[MAX_HISTORY];
	};
	
	class ThreadCallElapsedRecord : SLIST_ENTRY
	{
	public:
		ThreadCallElapsedRecord(int tid) : mThreadId(tid)
		{
			memset(mElapsedFuncSig, 0, sizeof(mElapsedFuncSig));
			memset(mElapsedTime, 0, sizeof(mElapsedTime));
		}

		inline void Append(const char* funcsig, int64_t elapsed)
		{
			mElapsedFuncSig[mCounter % MAX_ELAPSED_RECORD] = funcsig;
			mElapsedTime[mCounter % MAX_ELAPSED_RECORD] = elapsed;
			++mCounter;
		}

		void DumpOut(std::ostream& ost = std::cout);

	private:
		enum
		{
			MAX_ELAPSED_RECORD = 512
		};

		uint64_t	mCounter = 0;
		int			mThreadId = -1;
		const char*	mElapsedFuncSig[MAX_ELAPSED_RECORD];
		int64_t		mElapsedTime[MAX_ELAPSED_RECORD];
	};

	class ScopeElapsedCheck
	{
	public:
		ScopeElapsedCheck(const char* funcsig) : mFuncSig(funcsig)
		{
			/* FYI
			* 10~16 ms 해상도로 체크하려면 GetTickCount 사용
			* 1 us 해상도로 체크하려면  QueryPerformanceCounter 사용
			*/
			mStartTick = GetTickCount64();
		}

		~ScopeElapsedCheck();
		
	private:

		const char*	mFuncSig;
		int64_t	mStartTick = 0;
	};

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
		event.mThreadId = LThreadId;
		event.mMessage = msg;
		event.mAdditionalInfo = info;
	}

	void EventLogDumpOut(std::ostream& ost = std::cout);
	void PrintMemoryInfo();
}

#define TRACE_THIS	\
	using namespace S4Framework; \
	__if_exists (this)	\
	{	\
		LRecentThisPointer = (void*)this;	\
	}	\
	if (LThreadType != THREAD_MAIN)	\
	{	\
		LThreadCallHistory->Append(__FUNCSIG__);	\
	}	

#define TRACE_PERF	\
	S4Framework::ScopeElapsedCheck __scope_elapsed_check__(__FUNCSIG__);