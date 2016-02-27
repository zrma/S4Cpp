#include "stdafx.h"

#include "..\S4Util\Log.h"
#include "ThreadLocal.h"
#include "IConcurrentPool.h"

namespace S4Thread
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

		S4Util::LThreadCallHistory = new S4Util::ThreadCallHistory(LThreadId);
		InterlockedPushEntrySList(&S4Util::GThreadCallHistory, (PSLIST_ENTRY)S4Util::LThreadCallHistory);

		S4Util::LThreadCallElapsedRecord = new S4Util::ThreadCallElapsedRecord(LThreadId);
		InterlockedPushEntrySList(&S4Util::GThreadCallElapsedRecord, (PSLIST_ENTRY)S4Util::LThreadCallElapsedRecord);

		ioService->run();
	}
}