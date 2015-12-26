#include "stdafx.h"
#include "ThreadLocal.h"

namespace S4Thread
{
	thread_local int LThreadType = -1;
	thread_local int LThreadId = -1;
	thread_local int LTickCount = 0;
}
