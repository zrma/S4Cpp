// S4Game.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "../../S4Framework/ThreadLocal.h"
#include "../../S4Framework/ConcurrentJobManager.h"
#include "../../S4Framework/NetworkManager.h"

#include "../../S4Framework/Exception.h"
#include "../../S4Framework/Log.h"

#pragma comment(lib, "S4Framework.lib")
#pragma comment(lib, "Dbghelp.lib")

const int PORT_NUM = 35555;

std::shared_ptr<S4Framework::NetworkManager> GNetworkManager;
std::shared_ptr<S4Framework::ConcurrentJobManager> GLogicPool;

int main()
{
	/// for dump on crash
	SetUnhandledExceptionFilter(S4Framework::ExceptionFilter);

	S4Framework::LThreadType = S4Framework::THREAD_TYPE::THREAD_MAIN;
	
	GLogicPool = std::make_unique<S4Framework::ConcurrentJobManager>();
	GNetworkManager = std::make_unique<S4Framework::NetworkManager>((PORT_NUM));
	GNetworkManager->Run();
	
	BOOST_LOG_TRIVIAL(info) << "네트워크 접속 종료";
	getchar();

	return 0;
}
