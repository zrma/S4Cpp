// S4Game.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "../../S4Framework/ThreadLocal.h"
#include "../../S4Framework/ConcurrentJobManager.h"
#include "../../S4Framework/NetworkManager.h"

#include "../../S4Framework/Exception.h"
#include "../../S4Framework/Log.h"
#include "../../S4Framework/ISyncExecutable.h"

#pragma comment(lib, "S4Framework.lib")
#pragma comment(lib, "Dbghelp.lib")

const int PORT_NUM = 35555;

std::shared_ptr<S4Framework::NetworkManager> GNetworkManager;
std::shared_ptr<S4Framework::ConcurrentJobManager> GLogicPool;

class Test : public S4Framework::ISyncExecutable
{
public:
	Test(S4Framework::IConcurrentPool& pool) : ISyncExecutable(pool)
	{

	}
	virtual ~Test() {}

	void Reset()
	{
		mCount = 0;
		mHeartBeat = 0;
	}

	void Start(int heartbeat)
	{
		mHeartBeat = heartbeat;

		OnTick();
	}

	void OnTick()
	{
		++mCount;
		if (mHeartBeat > 0)
		{
			if ( rand() % 1000 > 998 )
			{
				BOOST_LOG_TRIVIAL(info) << "Thread ID : " << GetCurrentThreadId() << " / 카운트 : " << mCount;

				DoSync(&Test::Reset);

				int heartBeat = rand() % 500 + 500;
				S4Framework::DoSyncAfter(10, GetSharedFromThis<Test>(), &Test::Start, heartBeat);

				return;
			}

			S4Framework::DoSyncAfter(mHeartBeat, GetSharedFromThis<Test>(), &Test::OnTick);
		}
	}

private:
	int mCount = 0;
	int mHeartBeat = 0;
};

int main()
{
	/// for dump on crash
	SetUnhandledExceptionFilter(S4Framework::ExceptionFilter);

	S4Framework::LThreadType = S4Framework::THREAD_TYPE::THREAD_MAIN;
	
	GLogicPool = std::make_unique<S4Framework::ConcurrentJobManager>();
	GLogicPool->Init();

	for (std::size_t i = 0; i < 3000; ++i)
	{
		auto t1 = std::make_shared<Test>(*GLogicPool);
		t1->DoSync(&Test::Reset);

		int heartBeat = rand() % 500 + 500;
		S4Framework::DoSyncAfter(10, t1, &Test::Start, heartBeat);
	}

	GNetworkManager = std::make_unique<S4Framework::NetworkManager>(PORT_NUM);
	GNetworkManager->Init();
	GNetworkManager->Run();
	
	BOOST_LOG_TRIVIAL(info) << "네트워크 접속 종료";
	getchar();

	return 0;
}
