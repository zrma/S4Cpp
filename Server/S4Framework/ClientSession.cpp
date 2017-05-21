#include "stdafx.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "Exception.h"
#include "Log.h"

namespace S4Framework
{
	ClientSession::~ClientSession()
	{}

	void ClientSession::AcceptComplete()
	{
		if( 1 == InterlockedExchange( &mConnected, 1 ) )
		{
			/// already exists?
			CRASH_ASSERT( false );
			return;
		}
	}

	void ClientSession::OnDisconnect( DisconnectReason dr )
	{
		// std::cout << "Ŭ���̾�Ʈ ���� ����! (" << dr << ")" << std::endl;
	}

	void ClientSession::OnRelease()
	{
		GClientSessionManager->ReturnClientSession( mSessionID );
	}
}