// S4Game.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "../../S4Thread/ThreadLocal.h"
#include "../../S4Thread/ConcurrentJobManager.h"
#include "../../S4Network/NetworkManager.h"

#include <boost/log/trivial.hpp>
#include <mutex>

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

	BOOST_LOG_TRIVIAL(info) << "네트워크 접속 종료";
	
	/*
	std::size_t total = 3000000000;
	std::size_t count = 1000;
	std::size_t token = total / count;

	std::mutex mtx_lock;
	std::size_t totalSum = 0;
	std::size_t totalCount = 0;
	std::size_t type = 0;

	BOOST_LOG_TRIVIAL(info) << "Single Thread Start!";
	
	for (std::size_t i = 1; i <= total; ++i)
	{
		totalSum += i;
	}

	BOOST_LOG_TRIVIAL(info) << "Result : " << totalSum;

	totalSum = 0;

	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	auto job = [&](std::size_t idx, std::size_t start, std::size_t end) -> std::size_t
	{
		std::size_t sum = 0;
		for (std::size_t i = start; i <= end; ++i)
		{
			sum += i;
		}

		BOOST_LOG_TRIVIAL(info) << idx << " : " << start << " to " << end << " = "<< sum;

		std::lock_guard<std::mutex> guard(mtx_lock);
		totalSum += sum;

		if (++totalCount == count)
		{
			BOOST_LOG_TRIVIAL(info) << "Total : " << totalSum;
			totalSum = 0;
			totalCount = 0;

			if (type++ == 0 )
			{
				BOOST_LOG_TRIVIAL(info) << "Multi Thread Start!";
				SetEvent(hEvent);
			}
		}

		return sum;
	};

	BOOST_LOG_TRIVIAL(info) << "Strand Start!";
	
	boost::asio::strand task(GJobManager.GetHandler());

	for (std::size_t i = 0; i < count; ++i)
	{
		GJobManager.PostJob( task.wrap(std::bind(job, i, i * token + 1, (i + 1) * token)) );
	}
	
	WaitForSingleObject(hEvent, 30000);

	for (std::size_t i = 0; i < count; ++i)
	{
		GJobManager.PostJob(std::bind(job, i, i * token + 1, (i + 1) * token));
	}
	*/

	int div = 0;
	int test = 4 / div;
		
	getchar();
	
    return 0;
}