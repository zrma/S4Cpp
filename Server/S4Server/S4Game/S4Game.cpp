// S4Game.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

	// BOOST_LOG_TRIVIAL(info) << "네트워크 접속 종료";

	for (std::size_t i = 0; i < 10; ++i)
	{
		GJobManager.PostJob([]()
		{
			BOOST_LOG_TRIVIAL(info) << std::this_thread::get_id() << "스레드 진입!!!";
			BOOST_LOG_TRIVIAL(info) << std::this_thread::get_id() << "스레드 탈출!!!";
		});
	}

	boost::asio::strand job(GJobManager.GetHandler());

	for (std::size_t i = 0; i < 10; ++i)
	{
		GJobManager.PostJob(job.wrap([]()
		{
			BOOST_LOG_TRIVIAL(info) << std::this_thread::get_id() << "스레드 진입!!!";
			Sleep(3000);
			BOOST_LOG_TRIVIAL(info) << std::this_thread::get_id() << "스레드 탈출!!!";
		}));
	}

	getchar();
	
    return 0;
}