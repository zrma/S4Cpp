#include "stdafx.h"

#include "NetworkManager.h"
#include "Log.h"
#include "ConcurrentJobManager.h"
#include "Session.h"
#include "ClientSessionManager.h"

namespace S4Framework
{
	NetworkManager::NetworkManager(int port, std::size_t size)
		: IConcurrentPool(size), mPort(port)
	{
	}

	NetworkManager::~NetworkManager()
	{
	}
	
	void NetworkManager::InitThread()
	{
		BOOST_LOG_TRIVIAL(info) << "세션 큐 생성 중";
		LSendRequestSessionList = std::make_shared<SessionListPtr>();

		auto timer = std::make_shared<boost::asio::steady_timer>(mDispatcher);
	}

	void NetworkManager::Run()
	{
		do
		{
			DoSendJob();
		} while (mDispatcher.run_one());
	}

	void NetworkManager::StartAccept(std::size_t size)
	{
		GClientSessionManager = std::make_unique<ClientSessionManager>(mPort, size, mDispatcher);
		GClientSessionManager->PrepareClientSession();

		BOOST_LOG_TRIVIAL(info) << "클라이언트 접속 대기";

		while (GClientSessionManager->AcceptClientSession())
		{
			Sleep(100);
		}
	}

	void NetworkManager::DoSendJob()
	{
		while (!LSendRequestSessionList->empty())
		{
			auto& session = LSendRequestSessionList->front();
			session->FlushSend();
			LSendRequestSessionList->pop_front();
		}
	}

}
