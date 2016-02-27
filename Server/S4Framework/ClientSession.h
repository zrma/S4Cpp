#pragma once

#include "Session.h"

namespace S4Framework
{
	class ClientSession : public Session
	{
	public:
		ClientSession() = delete;
		virtual ~ClientSession();
	};
}
