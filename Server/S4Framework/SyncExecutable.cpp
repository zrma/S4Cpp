#include "stdafx.h"
#include "SyncExecutable.h"
#include "IConcurrentPool.h"

#include <boost/asio/steady_timer.hpp>

namespace S4Framework
{
	SyncExecutable::SyncExecutable(IConcurrentPool& pool)
		: mPool(pool), mWrapper(pool.GetDispatcher())
	{

	}

	template <class R, class T, class... Args>
	void SyncExecutable::DoSync(R(T::*memfunc)(Args...), Args... args)
	{
		static_assert(true == std::is_convertible<T, SyncExecutable>::value, "T should be derived from SyncExecutable");

		mPool.PostJob(mWrapper.wrap(static_cast<T*>(this)->*memfunc, this, std::forward<Args>(args)...));
	}

	template <class T, class F, class... Args>
	void DoSyncAfter(uint32_t after, T instance, F memfunc, Args&&... args)
	{
		static_assert(true == is_shared_ptr<T>::value, "T should be shared_ptr");
		static_assert(true == std::is_convertible<T, std::shared_ptr<SyncExecutable>>::value, "T should be shared_ptr SyncExecutable");

		boost::asio::steady_timer timer(std::static_pointer_cast<SyncExecutable>(instance)->mPool);
		timer.expires_from_now(boost::chrono::milliseconds(after));

		// TASK - strand 동기화 작업 해야함
		timer.async_wait(boost::bind(memfunc, instance, std::forward<Args>(args)...));

		// LTimer->PushTimerJob(std::static_pointer_cast<SyncExecutable>(instance),
		//	std::bind(memfunc, instance, std::forward<Args>(args)...), after);
	}
}