#include "stdafx.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "Log.h"

namespace S4Framework
{
	ClientSession::~ClientSession()
	{
	}

	void ClientSession::PostAccept(boost::asio::ip::tcp::acceptor& acceptor)
	{
		acceptor.async_accept(mTcpSocket, boost::bind(&ClientSession::AcceptComplete, this, boost::asio::placeholders::error));
	}

	void ClientSession::AcceptComplete(const boost::system::error_code& error)
	{
		BOOST_LOG_TRIVIAL(info) << "Ŭ���̾�Ʈ ����!";
		PostRecv();
	}

	void ClientSession::OnRelease()
	{
		BOOST_LOG_TRIVIAL(info) << "Ŭ���̾�Ʈ ���� ����!";
		GClientSessionManager->ReturnClientSession(this);
	}
}