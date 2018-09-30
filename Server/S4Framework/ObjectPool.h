#pragma once

#include <mutex>

#include "Exception.h"

using LockGuard = std::lock_guard<std::mutex>;

namespace S4Framework
{
	template <class Object, const int AllocCount = 100>
	class ObjectPool
	{
	public:
		ObjectPool()
		{
			static_assert(std::is_polymorphic<Object>::value == false, "NO VIRTUAL Object!");
		}

		static void* operator new(size_t objSize)
		{
			LockGuard criticalSection(mutex);

			if (!mFreeList)
			{
				mFreeList = new uint8_t[sizeof(Object)*AllocCount];

				auto pNext = mFreeList;
				auto ppCurrent = reinterpret_cast<uint8_t**>(mFreeList);

				for (auto i = 0; i < AllocCount - 1; ++i)
				{
					/// OBJECT의 크기가 반드시 포인터 크기보다 커야 한다
					pNext += sizeof(Object);
					*ppCurrent = pNext;
					ppCurrent = reinterpret_cast<uint8_t**>(pNext);
				}

				*ppCurrent = nullptr; ///< 마지막은 0으로 표시
				mTotalAllocCount += AllocCount;
			}

			const auto pAvailable = mFreeList;
			mFreeList = *reinterpret_cast<uint8_t**>(pAvailable);
			++mCurrentUseCount;

			return pAvailable;
		}

		static void	operator delete(void* obj)
		{
			LockGuard criticalSection(mutex);

			CRASH_ASSERT(mCurrentUseCount > 0);

			--mCurrentUseCount;

			*reinterpret_cast<uint8_t**>(obj) = mFreeList;
			mFreeList = static_cast<uint8_t*>(obj);
		}


	private:
		static uint8_t*	mFreeList;
		static int		mTotalAllocCount; ///< for tracing
		static int		mCurrentUseCount; ///< for tracing

		static std::mutex mutex;
	};


	template <class Object, const int AllocCount>
	uint8_t* ObjectPool<Object, AllocCount>::mFreeList = nullptr;

	template <class Object, const int AllocCount>
	int ObjectPool<Object, AllocCount>::mTotalAllocCount = 0;

	template <class Object, const int AllocCount>
	int ObjectPool<Object, AllocCount>::mCurrentUseCount = 0;
}