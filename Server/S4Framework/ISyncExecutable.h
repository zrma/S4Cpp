#pragma once

#include "TypeTraits.h"
#include "IConcurrentPool.h"

#include <boost/asio/strand.hpp>
#include <chrono>

namespace S4Framework
{
	class ISyncExecutable : public std::enable_shared_from_this<ISyncExecutable>
	{
	public:
		ISyncExecutable() = delete;
		ISyncExecutable( IConcurrentPool& pool ) : mPool( pool ), mWrapper( pool.GetDispatcher() ) {}
		virtual ~ISyncExecutable() {}

		template <class R, class T, class... Args>
		void DoSync( R( T::*memfunc )( Args... ), Args... args )
		{
			static_assert( true == std::is_convertible<T, ISyncExecutable>::value, "T should be derived from SyncExecutable" );

			auto f = [ = ]( Args... args ) { return ( static_cast<T*>( this )->*memfunc ) ( std::forward<Args>( args )... ); };
			auto task = mWrapper.wrap( f );
			mPool.PostJob( task );
		}

		template <class T>
		std::shared_ptr<T> GetSharedFromThis()
		{
			static_assert( true == std::is_convertible<T, ISyncExecutable>::value, "T should be derived from SyncExecutable" );

			return std::static_pointer_cast<T>( shared_from_this() );
		}

	private:
		typedef boost::asio::strand SyncWrapper;
		SyncWrapper			mWrapper;

		template <class T, class F, class... Args>
		friend void DoSyncAfter( uint32_t after, T instance, F memfunc, Args&&... args );
		IConcurrentPool&	mPool;
	};

	template <class T, class F, class... Args>
	void DoSyncAfter( uint32_t after, T instance, F memfunc, Args&&... args )
	{
		static_assert( true == is_shared_ptr<T>::value, "T should be shared_ptr" );
		static_assert( true == std::is_convertible<T, std::shared_ptr<ISyncExecutable>>::value, "T should be shared_ptr SyncExecutable" );

		auto timer = std::make_shared<boost::asio::steady_timer>( std::static_pointer_cast<ISyncExecutable>( instance )->mPool.GetDispatcher() );
		timer->expires_from_now( std::chrono::milliseconds( after ) );

		auto owner = std::static_pointer_cast<ISyncExecutable>( instance );
		auto task = owner->mWrapper.wrap( boost::bind( memfunc, instance, std::forward<Args>( args )... ) );

		timer->async_wait( [ = ]( const boost::system::error_code& error )
		{
			if( !error )
			{
				timer->cancel();
				owner->mPool.PostJob( task );
			}
			else
			{
				// BOOST_LOG_TRIVIAL(error) << "DoSyncAfter() error [" << error.value() << "] " << error.message();
				std::cout << "DoSyncAfter() error [" << error.value() << "] " << error.message() << std::endl;
			}
		} );
	}
}