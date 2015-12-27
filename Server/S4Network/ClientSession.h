#pragma once

#include "../S4Util/MemoryPool.h"
#include "Session.h"

namespace S4Network
{
	class ClientSession : public Session, public S4Util::PooledAllocatable
	{
	public:
		ClientSession();
		virtual ~ClientSession();
	};
}
