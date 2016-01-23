#include "stdafx.h"
#include "Session.h"

namespace S4Network
{
	Session::Session(boost::asio::io_service& io_service)
		: mTcpSocket( io_service ), mUdpSocket( io_service )
	{
	}
	
	Session::~Session()
	{
	}
}
