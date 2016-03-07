#pragma once

#include "Session.h"

namespace S4Framework
{
	class ClientSession : public Session
	{
	public:
		ClientSession(int sessionID, boost::asio::io_service& dispatcher) : Session(sessionID, dispatcher) {}
		ClientSession() = delete;
		virtual ~ClientSession();

		void PostAccept(boost::asio::ip::tcp::acceptor& acceptor);

		virtual void OnDisconnect() override;
		virtual void OnRelease() override;

	private:
		void AcceptComplete(const boost::system::error_code& error);

		friend class ClientSessionManager;
	};
}
