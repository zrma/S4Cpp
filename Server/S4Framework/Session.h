#pragma once

#include <boost/asio.hpp>
#define MAX_BUF_SIZE	8092

namespace S4Framework
{
	class Session
	{
	public:
		Session(int sessionID, boost::asio::io_service& io_service);
		~Session();

		void	Reset();

		int		GetSessionID() { return mSessionID; }
		
		typedef boost::asio::ip::tcp::socket TcpSocket;
		typedef boost::asio::ip::udp::socket UdpSocket;

		TcpSocket&	GetTcpSocket() { return mTcpSocket; }
		UdpSocket&	GetUdpSocekt() { return mUdpSocket; }

		void PostRecv();
		void PostSend(const bool bImmediately, const std::size_t nSize, char* pData);

	protected:
		void RecvComplete();
		void SendComplete();

		int				mSendPendingCount;
		int				mBufferOffset;
		
		volatile long	mRefCount = 0;
		volatile long	mConnected = 0;

		int				mSessionID = -1;

		TcpSocket		mTcpSocket;
		UdpSocket		mUdpSocket;

		std::array<char, MAX_BUF_SIZE>		mRecvDataBuffer;
		std::deque<std::shared_ptr<char>>	mSendDataQueue;

		boost::asio::strand					mSendSyncWrapper;
	};
}