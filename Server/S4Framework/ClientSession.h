#pragma once

#include "Session.h"

namespace S4Framework
{
	class ClientSession : public Session
	{
	public:
		ClientSession( std::size_t sessionID, boost::asio::io_service& dispatcher ) : Session( sessionID, dispatcher ) {}
		ClientSession() = delete;
		ClientSession(const ClientSession&) = delete;
		ClientSession& operator=(const ClientSession&) = delete;
		virtual ~ClientSession() = default;

		void AcceptComplete();

		virtual void OnDisconnect( DisconnectReason dr ) override;
		virtual void OnRelease() override;

	private:
		friend class ClientSessionManager;
	};
}