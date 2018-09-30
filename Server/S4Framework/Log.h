#pragma once

#include <iostream>
#include <boost/log/trivial.hpp>

namespace S4Framework
{
	extern thread_local int LThreadId;

	class ThreadCallHistory : SLIST_ENTRY
	{
	public:
		explicit ThreadCallHistory(const int threadId) : mThreadId(threadId), mHistory{nullptr}
		{}

		inline void Append(const char* funcSig)
		{
			mHistory[mCounter++ % MAX_HISTORY] = funcSig;
		}

		void DumpOut(std::ostream& ost = std::cout) const;

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
		explicit ThreadCallElapsedRecord(const int tid) : mThreadId(tid)
		{}

		inline void Append(const char* funSig, int64_t elapsed)
		{
			mElapsedFuncSig[mCounter % MAX_ELAPSED_RECORD] = funSig;
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
		const char*	mElapsedFuncSig[MAX_ELAPSED_RECORD]{nullptr};
		int64_t		mElapsedTime[MAX_ELAPSED_RECORD]{0};
	};

	class ScopeElapsedCheck
	{
	public:
		explicit ScopeElapsedCheck(const char* funcSig) : mFuncSig(funcSig)
		{
			/* FYI
			* 10~16 ms 해상도로 체크하려면 GetTickCount 사용
			* 1 us 해상도로 체크하려면  QueryPerformanceCounter 사용
			*/
			mStartTick = GetTickCount64();
		}
		ScopeElapsedCheck() = delete;

		~ScopeElapsedCheck();
		
	private:

		const char*	mFuncSig;
		int64_t	mStartTick = 0;
	};

	struct LogEvent
	{
		int ThreadId = -1;
		int	AdditionalInfo = 0;
		const char* Message = nullptr;
	};

	constexpr auto MAX_LOG_SIZE = 65536;   ///< Must be a power of 2

	extern LogEvent GLogEvents[MAX_LOG_SIZE];
	extern __int64 GCurrentLogIndex;

	inline void EventLog(const char* msg, int info)
	{
		const auto index = _InterlockedIncrement64(&GCurrentLogIndex) - 1;
		auto& event = GLogEvents[index & (MAX_LOG_SIZE - 1)];
		event.ThreadId = LThreadId;
		event.Message = msg;
		event.AdditionalInfo = info;
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