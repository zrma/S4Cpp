#pragma once

#include "TypeTraits.h"
#include <boost/asio/strand.hpp>

namespace S4Framework
{
	class IConcurrentPool;
	class SyncExecutable : public std::enable_shared_from_this<SyncExecutable>
	{
	public:
		SyncExecutable() = delete;
		SyncExecutable(IConcurrentPool& pool);
		virtual ~SyncExecutable() {}

		template <class R, class T, class... Args>
		void DoSync(R(T::*memfunc)(Args...), Args... args);
		
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
	void DoSyncAfter(uint32_t after, T instance, F memfunc, Args&&... args);
}