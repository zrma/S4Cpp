#include "stdafx.h"

#include "Log.h"
#include "ThreadLocal.h"
#include "IConcurrentPool.h"

namespace S4Framework
{
	IConcurrentPool::IConcurrentPool( std::size_t size ) : mPoolSize( size )
	{

	}

	IConcurrentPool::~IConcurrentPool()
	{
		mWork.reset();
		mDispatcher.stop();
		mGroup.join_all();
	}

	void IConcurrentPool::Init()
	{
		mWork = std::make_shared<boost::asio::io_service::work>( mDispatcher );

		const auto f = [ = ]()
		{
			LThreadId = GetCurrentThreadId();

			LThreadCallHistory = std::make_shared<ThreadCallHistory>( LThreadId );
			InterlockedPushEntrySList( &GThreadCallHistory, reinterpret_cast<PSLIST_ENTRY>(LThreadCallHistory.get()) );

			LThreadCallElapsedRecord = std::make_shared<ThreadCallElapsedRecord>( LThreadId );
			InterlockedPushEntrySList( &GThreadCallElapsedRecord, reinterpret_cast<PSLIST_ENTRY>(LThreadCallElapsedRecord.get()) );

			InitThread();
			Run();
		};

		for( std::size_t i = 0; i < mPoolSize; ++i )
		{
			mGroup.create_thread( f );
		}
	}

	void IConcurrentPool::Run()
	{
		mDispatcher.run();
	}
}