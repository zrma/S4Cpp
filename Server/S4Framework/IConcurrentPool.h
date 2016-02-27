#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace S4Framework
{
	class IConcurrentPool
	{
	private:
		IConcurrentPool() = delete;
	public:
		IConcurrentPool(std::size_t size);
		virtual ~IConcurrentPool() = 0;

		template <class F>
		void PostJob(F f)
		{
			mIOService.post(f);
		}

		typedef boost::asio::strand SyncWarpper;
		virtual SyncWarpper GetSyncWarpper() { return SyncWarpper(mIOService); }

		virtual void Init(boost::asio::io_service* ioService);

	protected:
		boost::asio::io_service mIOService;
		boost::thread_group mGroup;

		std::shared_ptr<boost::asio::io_service::work> mWork;
		std::size_t	mPoolSize;
	};
}
