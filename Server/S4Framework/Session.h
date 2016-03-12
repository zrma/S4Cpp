#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace S4Framework
{
	enum DisconnectReason
	{
		DR_NONE,
		DR_ACTIVE,
		DR_ONCONNECT_ERROR,
		DR_IO_REQUEST_ERROR,
		DR_COMPLETION_ERROR,
		DR_SENDFLUSH_ERROR
	};

	class Session
	{
	public:
		Session(int sessionID, boost::asio::io_service& dispatcher);
		virtual ~Session();

		void	Reset();

		int		GetSessionID() { return mSessionID; }
		
		typedef boost::asio::ip::tcp::socket Socket;
		
		Socket&	GetSocket() { return mSocket; }

		void PostRecv();
		void PostSend(const char* pData, const std::size_t nSize);

		void FlushSend();

		void AddRefCount();
		void SubRefCount();

		bool IsConnected() const { return !!mConnected; }
		void Disconnect(DisconnectReason dr);
		virtual void OnDisconnect(DisconnectReason dr) = 0;
		virtual void OnRelease() = 0;

	protected:
		void RecvComplete(const boost::system::error_code& error, size_t bytes_transferred);
		void SendComplete(const boost::system::error_code& error, size_t bytes_transferred);

		int				mSendPendingCount;
		int				mBufferOffset;
		
		volatile long	mRefCount = 0;
		volatile long	mConnected = 0;

		int				mSessionID = -1;
		Socket			mSocket;

		boost::asio::streambuf		mRecvDataBuffer;
		boost::asio::streambuf		mSendDataBuffer;

		typedef boost::asio::io_service Dispatcher;
		Dispatcher&		mDispatcher;

		typedef boost::asio::strand SyncWrapper;
		SyncWrapper		mSendSyncWrapper;
	};

	typedef std::deque<Session*> SessionPtrList;
	extern thread_local std::shared_ptr<SessionPtrList> LSendRequestSessionList;
	extern thread_local std::shared_ptr<SessionPtrList> LSendRequestFailedSessionList;
}