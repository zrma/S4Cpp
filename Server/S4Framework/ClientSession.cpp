#include "stdafx.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "Log.h"

namespace S4Framework
{
	ClientSession::~ClientSession()
	{
	}
	
	void ClientSession::OnDisconnect()
	{
		BOOST_LOG_TRIVIAL(info) << "Ŭ���̾�Ʈ ���� ����!";
	}

	void ClientSession::OnRelease()
	{
		GClientSessionManager->ReturnClientSession(mSessionID);
	}
}