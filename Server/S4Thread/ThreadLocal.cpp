#include "stdafx.h"
#include "ThreadLocal.h"

thread_local int S4Thread::LThreadType = -1;
thread_local int S4Thread::LThreadId = -1;
thread_local int S4Thread::LTickCount = 0;
