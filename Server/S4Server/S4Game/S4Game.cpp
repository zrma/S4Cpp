// S4Game.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

	BOOST_LOG_TRIVIAL(info) << "��Ʈ��ũ ���� ����";

	getchar();
	
    return 0;
}