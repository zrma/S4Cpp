// S4Game.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "ThreadLocal.h"
#include "ConcurrentJobManager.h"
#include "NetworkManager.h"

#include <boost/log/trivial.hpp>

#pragma comment(lib, "S4Thread.lib")
#pragma comment(lib, "S4Network.lib")

const int PORT_NUM = 35555;

S4Network::NetworkManager GNetworkManager;
S4Thread::ConcurrentJobManager GJobManager(8);

int main()
{
	S4Thread::LThreadType = S4Thread::THREAD_TYPE::THREAD_MAIN;

	GNetworkManager.Init(PORT_NUM);
	GNetworkManager.Run();

	// BOOST_LOG_TRIVIAL(info) << "��Ʈ��ũ ���� ����";

	for (std::size_t i = 0; i < 10; ++i)
	{
		GJobManager.PostJob([]()
		{
			BOOST_LOG_TRIVIAL(info) << std::this_thread::get_id() << "������ ����!!!";
			BOOST_LOG_TRIVIAL(info) << std::this_thread::get_id() << "������ Ż��!!!";
		});
	}

	boost::asio::strand job(GJobManager.GetHandler());

	for (std::size_t i = 0; i < 10; ++i)
	{
		GJobManager.PostJob(job.wrap([]()
		{
			BOOST_LOG_TRIVIAL(info) << std::this_thread::get_id() << "������ ����!!!";
			Sleep(3000);
			BOOST_LOG_TRIVIAL(info) << std::this_thread::get_id() << "������ Ż��!!!";
		}));
	}

	getchar();
	
    return 0;
}