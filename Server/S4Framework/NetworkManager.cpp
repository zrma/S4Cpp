#include "stdafx.h"

#include "NetworkManager.h"
#include "Log.h"
#include "ConcurrentJobManager.h"
#include "Session.h"

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

	void NetworkManager::InitThread()
	{
		BOOST_LOG_TRIVIAL(info) << "���� ť ���� ��";
		LSendRequestSessionList = new std::deque<Session*>;
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

		while (true)
		{
			Sleep(1000);
		}
	}
}
