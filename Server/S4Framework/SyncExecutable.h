#pragma once

#include "TypeTraits.h"
#include "IConcurrentPool.h"

#include <boost/asio/strand.hpp>
#include <chrono>

namespace S4Framework
{
	class SyncExecutable : public std::enable_shared_from_this<SyncExecutable>
	{
	public:
		SyncExecutable() = delete;
		SyncExecutable(IConcurrentPool& pool) : mPool(pool), mWrapper(pool.GetDispatcher())	{}
		virtual ~SyncExecutable() {}

		template <class R, class T, class... Args>
		void DoSync(R(T::*memfunc)(Args...), Args... args)
		{
			static_assert(true == std::is_convertible<T, SyncExecutable>::value, "T should be derived from SyncExecutable");

			mPool.PostJob(mWrapper.wrap(static_cast<T*>(this)->*memfunc, this, std::forward<Args>(args)...));
		}

		template <class T>
		std::shared_ptr<T> GetSharedFromThis()
		{
			static_assert(true == std::is_convertible<T, SyncExecutable>::value, "T should be derived from SyncExecutable");

			return std::static_pointer_cast<T>(shared_from_this());
		}
		
	private:
		typedef boost::asio::strand SyncWrapper;
		SyncWrapper			mWrapper;

		template <class T, class F, class... Args>
		friend void DoSyncAfter(uint32_t after, T instance, F memfunc, Args&&... args);
		IConcurrentPool&	mPool;
	};
	
	template <class T, class F, class... Args>
	void DoSyncAfter(uint32_t after, T instance, F memfunc, Args&&... args)
	{
		static_assert(true == is_shared_ptr<T>::value, "T should be shared_ptr");
		static_assert(true == std::is_convertible<T, std::shared_ptr<SyncExecutable>>::value, "T should be shared_ptr SyncExecutable");

		// TASK - 스마트포인터로 감싸줘야 함
		IConcurrentPool::Timer& timer = std::static_pointer_cast<SyncExecutable>(instance)->mPool.GetTimer();
		timer.expires_from_now(std::chrono::milliseconds(after));

		// TASK - strand 동기화 작업 해야함
		auto f = boost::bind(memfunc, instance, std::forward<Args>(args)...);

		timer.async_wait([&instance, f](const boost::system::error_code& error)
		{
			if (!error)
			{
				std::static_pointer_cast<SyncExecutable>(instance)->mPool.PostJob(f);
			}
			else
			{
				BOOST_LOG_TRIVIAL(error) << "DoSyncAfter() error [" << error.value() << "] " << error.message();
			}
		});

		// LTimer->PushTimerJob(std::static_pointer_cast<SyncExecutable>(instance),
		//	std::bind(memfunc, instance, std::forward<Args>(args)...), after);
	}
}