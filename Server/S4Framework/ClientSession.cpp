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
		BOOST_LOG_TRIVIAL(info) << "클라이언트 접속 해제!";
	}

	void ClientSession::OnRelease()
	{
		GClientSessionManager->ReturnClientSession(mSessionID);
	}
}