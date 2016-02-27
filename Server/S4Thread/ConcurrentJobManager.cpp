#include "stdafx.h"
#include "ConcurrentJobManager.h"
#include "../S4Util/Log.h"

namespace S4Thread
{
	ConcurrentJobManager::ConcurrentJobManager(std::size_t size) : IConcurrentPool(size)
	{
	}

	ConcurrentJobManager::~ConcurrentJobManager()
	{
	}
}
