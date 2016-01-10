#include "stdafx.h"
#include "ThreadLocal.h"
#include "LockOrderChecker.h"

namespace S4Thread
{
	thread_local int LThreadType = -1;
	thread_local int LThreadId = -1;
	thread_local int LTickCount = 0;

	thread_local LockOrderChecker* LLockOrderChecker;
}
