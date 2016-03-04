#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace S4Framework
{
	class Session
	{
	public:
		Session(int sessionID, boost::asio::io_service& dispatcher);
		~Session();

		void	Reset();

		int		GetSessionID() { return mSessionID; }
		
		typedef boost::asio::ip::tcp::socket TcpSocket;
		typedef boost::asio::ip::udp::socket UdpSocket;

		TcpSocket&	GetTcpSocket() { return mTcpSocket; }
		UdpSocket&	GetUdpSocekt() { return mUdpSocket; }

		void PostRecv();
		void PostSend(char* pData, const std::size_t nSize);

		void FlushSend();

	protected:
		void RecvComplete(const boost::system::error_code& error, size_t bytes_transferred);
		void SendComplete(const boost::system::error_code& error, size_t bytes_transferred);

		int				mSendPendingCount;
		int				mBufferOffset;
		
		volatile long	mRefCount = 0;
		volatile long	mConnected = 0;

		int				mSessionID = -1;

		TcpSocket		mTcpSocket;
		UdpSocket		mUdpSocket;

		boost::asio::streambuf		mRecvDataBuffer;
		boost::asio::streambuf		mSendDataBuffer;

		typedef boost::asio::io_service Dispatcher;
		Dispatcher&		mDispatcher;

		typedef boost::asio::strand SyncWrapper;
		SyncWrapper		mSendSyncWrapper;
	};

	extern thread_local std::deque<Session*>* LSendRequestSessionList;
}