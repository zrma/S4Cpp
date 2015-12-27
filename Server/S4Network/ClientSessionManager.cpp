#include "stdafx.h"
#include "ClientSessionManager.h"

namespace S4Network
{
	std::shared_ptr<ClientSessionManager> GClientSessionManager;

	void ClientSessionManager::PrepareClientSession()
	{

	}

	bool ClientSessionManager::AcceptClientSession()
	{
		return false;
	}

	void ClientSessionManager::ReturnClientSession(ClientSessionPtr client)
	{

	}
}
