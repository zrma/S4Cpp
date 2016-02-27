// S4Game.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "../../S4Thread/ThreadLocal.h"
#include "../../S4Thread/ConcurrentJobManager.h"
#include "../../S4Network/NetworkManager.h"

#include "../../S4Util/Exception.h"
#include "../../S4Util/Log.h"

#include <mutex>

#pragma comment(lib, "S4Thread.lib")
#pragma comment(lib, "S4Network.lib")

#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "S4Util.lib")

const int PORT_NUM = 35555;

std::shared_ptr<S4Network::NetworkManager> GNetworkManager;
std::shared_ptr<S4Thread::ConcurrentJobManager> GLogicPool;

int main()
{
	/// for dump on crash
	SetUnhandledExceptionFilter(S4Util::ExceptionFilter);

	S4Thread::LThreadType = S4Thread::THREAD_TYPE::THREAD_MAIN;
	
	GLogicPool = std::make_unique<S4Thread::ConcurrentJobManager>();
	GNetworkManager = std::make_unique<S4Network::NetworkManager>((PORT_NUM));
	GNetworkManager->Run();
	
	S4Util::CRASH_ASSERT(false);

	BOOST_LOG_TRIVIAL(info) << "네트워크 접속 종료";
	getchar();

	return 0;
}
