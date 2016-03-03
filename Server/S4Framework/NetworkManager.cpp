#include "stdafx.h"

#include "NetworkManager.h"
#include "Log.h"
#include "ConcurrentJobManager.h"

namespace S4Framework
{
	NetworkManager::NetworkManager(int port, std::size_t size)
		: IConcurrentPool(size), mAcceptor(mDispatcher, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
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
		BOOST_LOG_TRIVIAL(info) << "클라이언트 접속 대기";

		// TASK 클라이언트 접속 시작

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
