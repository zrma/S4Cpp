#include "stdafx.h"

#include "NetworkManager.h"
#include "../S4Util/Log.h"

namespace S4Network
{
	NetworkManager::NetworkManager(int port, std::size_t size)
		: IConcurrentPool(size), mAcceptor(mIOService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	{
	}

	NetworkManager::~NetworkManager()
	{
	}
	
	void NetworkManager::Run()
	{
		StartAccept();
	}

	void NetworkManager::StartAccept()
	{
		BOOST_LOG_TRIVIAL(info) << "Ŭ���̾�Ʈ ���� ���";

		// TASK Ŭ���̾�Ʈ ���� ����

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
