#include "stdafx.h"
#include "NetworkManager.h"
#include "ThreadLocal.h"


S4Network::NetworkManager::NetworkManager()
{
}


S4Network::NetworkManager::~NetworkManager()
{
}

void S4Network::NetworkManager::Init( int port )
{
	S4Network::Init();

	mIOService = std::make_shared<boost::asio::io_service>();
	mAcceptor = std::make_shared<boost::asio::ip::tcp::acceptor>
		( (*mIOService), boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port ) );
}
