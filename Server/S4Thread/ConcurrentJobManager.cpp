#include "stdafx.h"
#include "ConcurrentJobManager.h"
#include "ThreadLocal.h"
#include "../S4Util/Log.h"

namespace S4Thread
{
	ConcurrentJobManager::ConcurrentJobManager(std::size_t size)
	{
		mWork = std::make_shared<boost::asio::io_service::work>(mIOService);

		for (std::size_t i = 0; i < size; ++i)
		{
			mGroup.create_thread(boost::bind(&boost::asio::io_service::run, &mIOService));
		}

		for (std::size_t i = 0; i < size; ++i)
		{
			PostJob([i]() {
				if (i < MAX_DB_THREAD)
				{
					LThreadType = THREAD_DB_WORKER;
				}
				else
				{
					LThreadType = THREAD_IO_WORKER;
				}

				LThreadId = static_cast<int>(i);

				// TASK - 락 오더 체커 추가해야 함
				// LLockOrderChecker = new LockOrderChecker(LWorkerThreadId);

				S4Util::GThreadCallHistory[LThreadId] = S4Util::LThreadCallHistory = new S4Util::ThreadCallHistory(LThreadId);
				S4Util::GThreadCallElapsedRecord[LThreadId] = S4Util::LThreadCallElapsedRecord = new S4Util::ThreadCallElapsedRecord(LThreadId);

				Sleep(1000);
			});
		}
			
	}

	ConcurrentJobManager::~ConcurrentJobManager()
	{
		mWork.reset();
		mGroup.join_all();
	}
}
