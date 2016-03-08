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

		void AcceptComplete();

		virtual void OnDisconnect(DisconnectReason dr) override;
		virtual void OnRelease() override;

	private:
		friend class ClientSessionManager;
	};
}
