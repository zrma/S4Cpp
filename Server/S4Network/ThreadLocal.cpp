#include "stdafx.h"
#include "ThreadLocal.h"

boost::thread_specific_ptr<int> S4Network::LThreadType;
boost::thread_specific_ptr<int> S4Network::LThreadId;
boost::thread_specific_ptr<int64_t> S4Network::LTickCount;

void S4Network::Init()
{
	LThreadType.reset(new int(-1));
	LThreadId.reset(new int(-1));
	LTickCount.reset(new int64_t(0));
}