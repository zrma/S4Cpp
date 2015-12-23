#include "stdafx.h"
#include "ThreadLocal.h"

thread_local int LThreadType = -1;
thread_local int LThreadId = -1;
thread_local int LTickCount = 0;