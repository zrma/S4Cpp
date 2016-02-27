#include "stdafx.h"
#include "ConcurrentJobManager.h"
#include "Log.h"

namespace S4Framework
{
	ConcurrentJobManager::ConcurrentJobManager(std::size_t size) : IConcurrentPool(size)
	{
	}

	ConcurrentJobManager::~ConcurrentJobManager()
	{
	}
}
