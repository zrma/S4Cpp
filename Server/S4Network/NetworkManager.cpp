#include "stdafx.h"
#include "NetworkManager.h"

#include <boost/log/trivial.hpp>

namespace S4Network
{
	NetworkManager::NetworkManager()
	{
	}

	NetworkManager::~NetworkManager()
	{
	}

	void NetworkManager::Init(int port)
	{
		mIOService = std::make_shared<boost::asio::io_service>();
		mAcceptor = std::make_shared<boost::asio::ip::tcp::acceptor>
			((*mIOService), boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
	}

	void NetworkManager::Run()
	{
		StartAccept();

		mIOService->run();
	}

	void NetworkManager::StartAccept()
	{
		BOOST_LOG_TRIVIAL(info) << "클라이언트 접속 대기";

		/*
		m_pSession = new Session(m_acceptor.get_io_service());

		m_acceptor.async_accept(m_pSession->Socket(),
			boost::bind(&TCP_Server::handle_accept,
				this,
				m_pSession,
				boost::asio::placeholders::error)
			);
		*/
	}
}
