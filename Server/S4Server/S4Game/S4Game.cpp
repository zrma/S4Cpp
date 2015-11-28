// S4Game.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "NetworkManager.h"

#pragma comment(lib, "S4Network.lib")

const int PORT_NUM = 35555;

S4Network::NetworkManager GNetworkManager;

int main()
{
	GNetworkManager.Init(PORT_NUM);
	
    return 0;
}