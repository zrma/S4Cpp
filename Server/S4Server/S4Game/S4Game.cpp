// S4Game.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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