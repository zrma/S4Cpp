#include "stdafx.h"
#include "ClientSessionManager.h"

namespace S4Framework
{
	std::shared_ptr<ClientSessionManager> GClientSessionManager;

	void ClientSessionManager::PrepareClientSession()
	{
		// TASK Ŭ���̾�Ʈ ���� �غ�
	}

	bool ClientSessionManager::AcceptClientSession()
	{
		// TASK Ŭ���̾�Ʈ ���� ����
		return false;
	}

	void ClientSessionManager::ReturnClientSession(ClientSessionPtr client)
	{
		// TASK ���� ��ȯ ����
	}
}
