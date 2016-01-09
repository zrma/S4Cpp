#pragma once

#include <boost/pool/singleton_pool.hpp>

namespace S4Util
{
	/// 요놈을 상속 받아야만 xnew/xdelete 사용할 수 있게...
	struct PooledAllocatable {};
	
	template <class T, class... Args>
	T* xnew(Args... arg)
	{
		static_assert(true == std::is_convertible<T, PooledAllocatable>::value, "only allowed when PooledAllocatable");

		void* alloc = boost::singleton_pool<T, sizeof(T)>::malloc();
		new (alloc)T(arg...);
		return reinterpret_cast<T*>(alloc);
	}

	template <class T>
	void xdelete(T* object)
	{
		static_assert(true == std::is_convertible<T, PooledAllocatable>::value, "only allowed when PooledAllocatable");

		object->~T();
		boost::singleton_pool<T, sizeof(T)>::free(object);
	}

	template <class T>
	void xdestroy()
	{
		static_assert(true == std::is_convertible<T, PooledAllocatable>::value, "only allowed when PooledAllocatable");

		boost::singleton_pool<T, sizeof(T)>::purge_memory();
	}
}
