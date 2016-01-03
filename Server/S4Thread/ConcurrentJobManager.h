#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace S4Thread
{
	class ConcurrentJobManager
	{
	public:
		ConcurrentJobManager(std::size_t size);
		~ConcurrentJobManager();

		template <class F>
		void PostJob(F f)
		{
			mIOService.post(f);
		}

		boost::asio::io_service& GetHandler() { return mIOService; }

	private:
		boost::asio::io_service mIOService;
		boost::thread_group mGroup;

		std::shared_ptr<boost::asio::io_service::work> mWork;
	};

}
