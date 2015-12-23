// S4Game.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ThreadLocal.h"
#include "NetworkManager.h"

#include <boost/log/trivial.hpp>

#pragma comment(lib, "Thread.lib")
#pragma comment(lib, "S4Network.lib")

const int PORT_NUM = 35555;

S4Network::NetworkManager GNetworkManager;

int main()
{
	LThreadType = THREAD_TYPE::THREAD_MAIN;

	GNetworkManager.Init(PORT_NUM);
	GNetworkManager.Run();

	BOOST_LOG_TRIVIAL(info) << "네트워크 접속 종료";

	getchar();
	
    return 0;
}