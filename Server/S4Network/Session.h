#pragma once

#include <boost/asio.hpp>
// #include "CircularBuffer.h"

namespace S4Network
{
	class Session
	{
	public:
		Session(boost::asio::io_service& io_service);
		~Session();



	protected:
		boost::asio::ip::tcp::socket mTcpSocket;
		boost::asio::ip::udp::socket mUdpSocket;

		// FastSpinlock	mSendBufferLock;
		int				mSendPendingCount;

		volatile long	mRefCount = 0;
		volatile long	mConnected = 0;
	};
}

