#include "stdafx.h"
#include "ConcurrentJobManager.h"

namespace S4Thread
{
	ConcurrentJobManager::ConcurrentJobManager(std::size_t size)
	{
		mWork = std::make_shared<boost::asio::io_service::work>(mIOService);

		for (std::size_t i = 0; i < size; ++i)
		{
			mGroup.create_thread(boost::bind(&boost::asio::io_service::run, &mIOService));
		}
	}

	ConcurrentJobManager::~ConcurrentJobManager()
	{
		mWork.reset();
		mGroup.join_all();
	}
}
