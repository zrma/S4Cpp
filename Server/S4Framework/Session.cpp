#include "stdafx.h"
#include "Session.h"

#include <boost/asio/streambuf.hpp>

#define MAX_BUF_SIZE	8092

namespace S4Framework
{
	thread_local std::deque<Session*>* LSendRequestSessionList = nullptr;

	Session::Session(int sessionID, boost::asio::io_service& dispatcher)
		: mSessionID(sessionID)
		, mDispatcher(dispatcher)
		, mTcpSocket(dispatcher)
		, mUdpSocket(dispatcher)
		, mSendSyncWrapper(dispatcher)
	{
	}
	
	Session::~Session()
	{
	}

	void Session::Reset()
	{
		mSessionID	= -1;
		mRefCount	= 0;
		mConnected	= 0;

		mTcpSocket.close();
		mUdpSocket.close();

		size_t size = mRecvDataBuffer.size();
		mRecvDataBuffer.consume(size);
		
		auto f = [=]()
		{
			size_t size = mSendDataBuffer.size();
			mSendDataBuffer.consume(size);
		};
		auto task = mSendSyncWrapper.wrap(f);
		mDispatcher.post(task);
	}

	void Session::PostRecv()
	{
		boost::asio::streambuf::mutable_buffers_type bufs = mRecvDataBuffer.prepare(MAX_BUF_SIZE);

		mTcpSocket.async_read_some(
			bufs,
			boost::bind(&Session::RecvComplete, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
			);
	}

	void Session::PostSend(char* pData, const std::size_t nSize)
	{
		auto f = [=]()
		{
			std::ostream os(&mSendDataBuffer);
			os.write(pData, nSize);
		};
		auto task = mSendSyncWrapper.wrap(f);
		mDispatcher.post(task);
	}

	void Session::FlushSend()
	{
		auto f = [=]()
		{
			boost::asio::async_write(mTcpSocket, mSendDataBuffer.data(),
				boost::bind(&Session::SendComplete, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)
				);
		};
		auto task = mSendSyncWrapper.wrap(f);
		mDispatcher.post(task);
	}

	void Session::RecvComplete(const boost::system::error_code& error, size_t bytes_transferred)
	{

	}

	void Session::SendComplete(const boost::system::error_code& error, size_t bytes_transferred)
	{

	}

}
