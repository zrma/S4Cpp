#pragma once

#include <thread>

enum THREAD_TYPE
{
	THREAD_MAIN = 0,
	THREAD_IO_WORKER,
	THREAD_DB_WORKER,
	THREAD_WORLD
};

extern thread_local int LThreadType;
extern thread_local int LThreadId;
extern thread_local int LTickCount;