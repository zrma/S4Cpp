#pragma once

#include "Session.h"

namespace S4Network
{
	class ClientSession : public Session
	{
	public:
		ClientSession() = delete;
		virtual ~ClientSession();
	};
}
