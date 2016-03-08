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
		// BOOST_LOG_TRIVIAL(info) << "���� ť ���� ��";
		std::cout << "���� ť ���� ��" << std::endl;
		LSendRequestSessionList = std::make_shared<SessionListPtr>();
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
		GClientSessionManager = std::make_unique<ClientSessionManager>(mPort, mDispatcher);
		GClientSessionManager->PrepareClientSession(size);

		// BOOST_LOG_TRIVIAL(info) << "Ŭ���̾�Ʈ ���� ���";
		std::cout << "Ŭ���̾�Ʈ ���� ���" << std::endl;

		auto startTime = GetTickCount64();
		GClientSessionManager->AcceptClientSession();

		while (mIsContinue)
		{
			Sleep(100);

			/*if (startTime + 30000 < GetTickCount64())
			{
				mIsContinue = false;
			}*/
		}
	}

	void NetworkManager::DoSendJob()
	{
		for (const auto& session : *LSendRequestSessionList)
		{
			session->FlushSend();
		}

		LSendRequestSessionList->clear();
	}

}
