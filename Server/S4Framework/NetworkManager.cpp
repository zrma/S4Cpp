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
		GClientSessionManager.release();
	}
	
	void NetworkManager::InitThread()
	{
		// BOOST_LOG_TRIVIAL(info) << "���� ť ���� ��";
		std::cout << "���� ť ���� ��" << std::endl;
		LSendRequestSessionList = std::make_shared<SessionPtrList>();
		LSendRequestFailedSessionList = std::make_shared<SessionPtrList>();
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

		auto prevTime = GetTickCount64();
		GClientSessionManager->AcceptClientSession();

		const auto tick = 5 * 60 * 1000; // 5��

		while (mIsContinue)
		{
			Sleep(100);

			if (prevTime + tick < GetTickCount64())
			{
				// 5�� �ֱ�� ���� ����͸�
				GClientSessionManager->PrintSessionState();
				prevTime = GetTickCount64();
				// mIsContinue = false;
			}
		}
	}

	void NetworkManager::DoSendJob()
	{
		while (!LSendRequestSessionList->empty())
		{
			auto& session = LSendRequestSessionList->front();
			LSendRequestSessionList->pop_front();

			session->FlushSend();
		}
		
		//////////////////////////////////////////////////////////////////////////
		// Swap!
		LSendRequestSessionList->swap(*LSendRequestFailedSessionList);
	}

}
