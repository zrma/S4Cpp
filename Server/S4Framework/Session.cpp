#include "stdafx.h"

#include <boost/asio/streambuf.hpp>

#include "Session.h"
#include "Log.h"
#include "Exception.h"

#define MAX_BUF_SIZE	8092

namespace S4Framework
{
	thread_local std::shared_ptr<SessionPtrList> LSendRequestSessionList;
	thread_local std::shared_ptr<SessionPtrList> LSendRequestFailedSessionList;

	/**
	 * \brief Session
	 * \param sessionID SessionID
	 * \param dispatcher Boost io_service
	 */
	Session::Session(int sessionID, boost::asio::io_service& dispatcher)
		: mSendPendingCount(0), mBufferOffset(0), mSessionID(sessionID)
		, mSocket(dispatcher)
		, mDispatcher(dispatcher)
		, mSendSyncWrapper(dispatcher)
	{}

	Session::~Session()
	{
		mRefCount = 0;
		mConnected = 0;

		mSendPendingCount = 0;

		if( mSocket.is_open() )
		{
			mSocket.close();
		}
	}

	void Session::Reset()
	{
		mRefCount = 0;
		mConnected = 0;

		mSendPendingCount = 0;

		if( mSocket.is_open() )
		{
			mSocket.close();
		}
	}

	void Session::PostRecv()
	{
		if( !IsConnected() )
		{
			return;
		}

		// TASK - 임시
		AddRefCount();

		boost::asio::streambuf::mutable_buffers_type bufs = mRecvDataBuffer.prepare( MAX_BUF_SIZE );

		mSocket.async_read_some(
			bufs,
			boost::bind( &Session::RecvComplete, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred )
		);
	}

	void Session::PostSend( const char* pData, const std::size_t nSize )
	{
		if( !IsConnected() )
		{
			return;
		}

		auto f = [ = ]()
		{
			std::ostream os( &mSendDataBuffer );
			os.write( pData, nSize );

			LSendRequestSessionList->push_back( this );
		};
		auto task = mSendSyncWrapper.wrap( f );
		mDispatcher.post( task );
	}

	void Session::FlushSend()
	{
		if( !IsConnected() )
		{
			Disconnect( DR_SENDFLUSH_ERROR );
			return;
		}

		auto f = [ = ]()
		{
			// 보낼 데이터가 없을 경우
			if( mSendDataBuffer.size() == 0 )
			{
				// 보냈던 데이터도 없는 경우
				if( 0 == mSendPendingCount )
				{
					return;
				}

				// 재시도 대상만 별도 수집
				LSendRequestFailedSessionList->push_back( this );
				return;
			}

			// 이전의 send가 완료 안 된 경우
			if( mSendPendingCount > 0 )
			{
				// 재시도 대상만 별도 수집
				LSendRequestFailedSessionList->push_back( this );
				return;
			}

			// TASK - 임시
			AddRefCount();

			++mSendPendingCount;

			boost::asio::async_write( mSocket, mSendDataBuffer.data(),
				boost::bind( &Session::SendComplete, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred )
			);
		};
		auto task = mSendSyncWrapper.wrap( f );
		mDispatcher.post( task );
	}

	void Session::AddRefCount()
	{
		CRASH_ASSERT( InterlockedIncrement( &mRefCount ) > 0 );
	}

	void Session::SubRefCount()
	{
		long ret = InterlockedDecrement( &mRefCount );
		CRASH_ASSERT( ret >= 0 );

		if( ret == 0 )
		{
			OnRelease();
		}
	}

	void Session::Disconnect( DisconnectReason dr )
	{
		if( 0 == InterlockedExchange( &mConnected, 0 ) )
		{
			return;
		}

		auto f = [ = ]()
		{
			auto size = mRecvDataBuffer.size();
			mRecvDataBuffer.consume( size );

			size = mSendDataBuffer.size();
			mSendDataBuffer.consume( size );

			OnDisconnect( dr );

			/// release refcount when added at issuing a session
			SubRefCount();
		};

		auto task = mSendSyncWrapper.wrap( f );
		mDispatcher.post( task );
	}

	void Session::RecvComplete( const boost::system::error_code& error, size_t bytes_transferred )
	{
		// TASK - 임시
		SubRefCount();

		if( error )
		{
			if( error == boost::asio::error::eof )
			{
				// BOOST_LOG_TRIVIAL(info) << "클라이언트 연결 종료";

				Disconnect( DR_ONCONNECT_ERROR );
			}
			else
			{
				// BOOST_LOG_TRIVIAL(error) << "Session RecvComplete error [" << error.value() << "] " << error.message();
				std::cerr << "Session RecvComplete error [" << error.value() << "] " << error.message() << std::endl;

				Disconnect( DR_COMPLETION_ERROR );
			}
		}
		else
		{
			if( bytes_transferred > 0 )
			{
				// BOOST_LOG_TRIVIAL(info) << "RefCount : " << mRefCount;
				mRecvDataBuffer.commit( bytes_transferred );
			}

			// TASK - echo test

			const char* bufPtr = boost::asio::buffer_cast<const char*>( mRecvDataBuffer.data() );
			size_t len = mRecvDataBuffer.size();
			PostSend( bufPtr, len );

			mRecvDataBuffer.consume( len );

			// boost::asio::streambuf::const_buffers_type bufs = mRecvDataBuffer.data();
			//	std::string data(boost::asio::buffers_begin(bufs), boost::asio::buffers_end(bufs));

			// mRecvDataBuffer.consume(size);
			// trace(boost::str(boost::format("Still on buffer %d bytes") % _read_buf.size()));

			PostRecv();
		}
	}

	void Session::SendComplete( const boost::system::error_code& error, size_t bytes_transferred )
	{
		// TASK - 임시
		SubRefCount();

		if( error )
		{
			if( error == boost::asio::error::eof )
			{
				// BOOST_LOG_TRIVIAL(info) << "클라이언트 연결 종료";

				Disconnect( DR_ONCONNECT_ERROR );
			}
			else
			{
				// BOOST_LOG_TRIVIAL(error) << "Session SendComplete error [" << error.value() << "] " << error.message();
				std::cerr << "Session SendComplete error [" << error.value() << "] " << error.message() << std::endl;

				Disconnect( DR_COMPLETION_ERROR );
			}
		}
		else
		{
			if( bytes_transferred > 0 )
			{
				auto f = [ = ]()
				{
					// BOOST_LOG_TRIVIAL(info) << "Recv Complete RefCount : " << mRefCount;
					mSendDataBuffer.consume( bytes_transferred );

					--mSendPendingCount;
				};
				auto task = mSendSyncWrapper.wrap( f );
				mDispatcher.post( task );
			}
		}
	}
}