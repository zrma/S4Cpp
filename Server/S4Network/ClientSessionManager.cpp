#include "stdafx.h"
#include "ClientSessionManager.h"

namespace S4Network
{
	std::shared_ptr<ClientSessionManager> GClientSessionManager;

	void ClientSessionManager::PrepareClientSession()
	{
		// TASK 클라이언트 세션 준비
	}

	bool ClientSessionManager::AcceptClientSession()
	{
		// TASK 클라이언트 접속 시작
		return false;
	}

	void ClientSessionManager::ReturnClientSession(ClientSessionPtr client)
	{
		// TASK 세션 반환 구현
	}
}
