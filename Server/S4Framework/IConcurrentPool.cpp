#include "stdafx.h"

#include "Log.h"
#include "ThreadLocal.h"
#include "IConcurrentPool.h"

namespace S4Framework
{
	IConcurrentPool::IConcurrentPool(std::size_t size) : mPoolSize(size)
	{
		mWork = std::make_shared<boost::asio::io_service::work>(mIOService);

		for (std::size_t i = 0; i < mPoolSize; ++i)
		{
			mGroup.create_thread(boost::bind(&IConcurrentPool::Init, this, &mIOService));
		}
	}

	IConcurrentPool::~IConcurrentPool()
	{
		mWork.reset();
		mGroup.join_all();
	}

	void IConcurrentPool::Init(boost::asio::io_service* ioService)
	{
		LThreadId = GetCurrentThreadId();

		LThreadCallHistory = new ThreadCallHistory(LThreadId);
		InterlockedPushEntrySList(&GThreadCallHistory, (PSLIST_ENTRY)LThreadCallHistory);

		LThreadCallElapsedRecord = new ThreadCallElapsedRecord(LThreadId);
		InterlockedPushEntrySList(&GThreadCallElapsedRecord, (PSLIST_ENTRY)LThreadCallElapsedRecord);

		ioService->run();
	}
}