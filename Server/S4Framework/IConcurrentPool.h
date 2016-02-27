#pragma once

#pragma warning(push)
#pragma warning(disable: 4005)
#include <boost/asio.hpp>
#pragma warning(pop)

#include <boost/thread.hpp>

#define MAX_IO_THREAD		4
#define MAX_DB_THREAD		4
#define MAX_LOGIC_THREAD	8
#define MAX_WORKER_THREAD	(MAX_IO_THREAD + MAX_DB_THREAD + MAX_LOGIC_THREAD)

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
