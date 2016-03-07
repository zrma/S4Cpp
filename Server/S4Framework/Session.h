#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace S4Framework
{
	class Session
	{
	public:
		Session(int sessionID, boost::asio::io_service& dispatcher);
		virtual ~Session();

		void	Reset();

		int		GetSessionID() { return mSessionID; }
		
		typedef boost::asio::ip::tcp::socket TcpSocket;
		typedef boost::asio::ip::udp::socket UdpSocket;

		TcpSocket&	GetTcpSocket() { return mTcpSocket; }
		UdpSocket&	GetUdpSocekt() { return mUdpSocket; }

		void PostRecv();
		void PostSend(const char* pData, const std::size_t nSize);

		void FlushSend();

		void AddRefCount();
		void SubRefCount();

		void DisconnectRequest(/*DisconnectReason dr*/);
		virtual void OnDisconnect(/*DisconnectReason dr*/) = 0;
		virtual void OnRelease() = 0;

	protected:
		void RecvComplete(const boost::system::error_code& error, size_t bytes_transferred);
		void SendComplete(const boost::system::error_code& error, size_t bytes_transferred);
		void DisconnectComplete(const boost::system::error_code& error, size_t bytes_transferred);

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

	typedef std::deque<Session*> SessionListPtr;
	extern thread_local std::shared_ptr<SessionListPtr> LSendRequestSessionList;
}