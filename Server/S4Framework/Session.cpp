#include "stdafx.h"
#include "Session.h"

namespace S4Framework
{
	Session::Session(int sessionID, boost::asio::io_service& dispatcher)
		: mSessionID( sessionID )
		, mTcpSocket( dispatcher )
		, mUdpSocket( dispatcher )
		, mSendSyncWrapper( dispatcher )
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

		mRecvDataBuffer.fill(0);
		mSendDataQueue.clear();
	}

	void Session::PostRecv()
	{

	}

	void Session::PostSend(const bool bImmediately, const std::size_t nSize, char* pData)
	{

	}

	void Session::RecvComplete()
	{

	}

	void Session::SendComplete()
	{

	}

}
