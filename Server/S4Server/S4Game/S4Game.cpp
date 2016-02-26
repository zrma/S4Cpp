// S4Game.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "../../S4Thread/ThreadLocal.h"
#include "../../S4Thread/ConcurrentJobManager.h"
#include "../../S4Network/NetworkManager.h"

#include <boost/log/trivial.hpp>

#include "../../S4Util/Exception.h"
#include "../../S4Util/Log.h"

#include <mutex>

#pragma comment(lib, "S4Thread.lib")
#pragma comment(lib, "S4Network.lib")

#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "S4Util.lib")

const int PORT_NUM = 35555;

S4Network::NetworkManager GNetworkManager(PORT_NUM);
S4Thread::ConcurrentJobManager GJobManager(MAX_WORKER_THREAD);
std::mutex GCoutLock;

void Func()
{
	TRACE_THIS

	while (true)
	{
		TRACE_PERF

		GCoutLock.lock();
		std::cout << "테스트 출력 - " << GetCurrentThreadId() << std::endl;
		GCoutLock.unlock();
	}
}

int main()
{
	/// for dump on crash
	SetUnhandledExceptionFilter(S4Util::ExceptionFilter);

	S4Thread::LThreadType = S4Thread::THREAD_TYPE::THREAD_MAIN;
	GNetworkManager.Run();
	
	auto f = []() {
		TRACE_THIS

		while (true)
		{
			TRACE_PERF

			GCoutLock.lock();
			std::cout << "테스트 출력 - " << GetCurrentThreadId() << std::endl;
			GCoutLock.unlock();
		}
	};

	for (std::size_t i = 0; i < MAX_WORKER_THREAD; ++i)
	{
		GJobManager.PostJob(f);
	}

	for (std::size_t i = 0 ; i < MAX_WORKER_THREAD ; ++i)
	{
		GJobManager.PostJob(Func);
	}

	Sleep(1000);
	S4Util::CRASH_ASSERT(false);

	BOOST_LOG_TRIVIAL(info) << "네트워크 접속 종료";
	getchar();

	return 0;
}
