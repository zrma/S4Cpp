#include "stdafx.h"

#include "Log.h"
#include "ThreadLocal.h"
#include "IConcurrentPool.h"

namespace S4Framework
{
	IConcurrentPool::IConcurrentPool(std::size_t size) : mPoolSize(size)
	{

	}

	IConcurrentPool::~IConcurrentPool()
	{
		mWork.reset();
		mGroup.join_all();
	}

	void IConcurrentPool::Init()
	{
		mWork = std::make_shared<boost::asio::io_service::work>(mDispatcher);

		auto f = [=]()
		{
			LThreadId = GetCurrentThreadId();

			LThreadCallHistory = new ThreadCallHistory(LThreadId);
			InterlockedPushEntrySList(&GThreadCallHistory, (PSLIST_ENTRY)LThreadCallHistory);

			LThreadCallElapsedRecord = new ThreadCallElapsedRecord(LThreadId);
			InterlockedPushEntrySList(&GThreadCallElapsedRecord, (PSLIST_ENTRY)LThreadCallElapsedRecord);

			InitThread();
			mDispatcher.run();
		};

		for (std::size_t i = 0; i < mPoolSize; ++i)
		{
			mGroup.create_thread(f);
		}
	}
}