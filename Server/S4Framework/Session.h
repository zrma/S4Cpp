#pragma once

#include <boost/asio.hpp>
// #include "CircularBuffer.h"

namespace S4Framework
{
	class Session
	{
	public:
		Session(boost::asio::io_service& io_service);
		~Session();
		
	protected:
		boost::asio::ip::tcp::socket mTcpSocket;
		boost::asio::ip::udp::socket mUdpSocket;

		// TASK 버퍼 구현
		// TASK 버퍼 멀티스레드 락 추가
		// FastSpinlock	mSendBufferLock;
		int				mSendPendingCount;

		volatile long	mRefCount = 0;
		volatile long	mConnected = 0;
	};
}