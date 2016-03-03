// S4Game.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "../../S4Framework/ThreadLocal.h"
#include "../../S4Framework/ConcurrentJobManager.h"
#include "../../S4Framework/NetworkManager.h"

#include "../../S4Framework/Exception.h"
#include "../../S4Framework/Log.h"
#include "../../S4Framework/SyncExecutable.h"

#pragma comment(lib, "S4Framework.lib")
#pragma comment(lib, "Dbghelp.lib")

const int PORT_NUM = 35555;

std::shared_ptr<S4Framework::NetworkManager> GNetworkManager;
std::shared_ptr<S4Framework::ConcurrentJobManager> GLogicPool;

class Test : public S4Framework::SyncExecutable
{
public:
	Test(S4Framework::IConcurrentPool& pool) : SyncExecutable(pool)
	{

	}
	virtual ~Test() {}

	void Start(int heartbeat)
	{
		mHeartBeat = heartbeat;

		OnTick();
	}

	void OnTick()
	{
		if (mHeartBeat > 0)
		{
			BOOST_LOG_TRIVIAL(info) << "카운트 : " << ++mCount;
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

	auto test = std::make_shared<Test>(*GLogicPool);
	S4Framework::DoSyncAfter(10, test, &Test::Start, 1000);

	GNetworkManager = std::make_unique<S4Framework::NetworkManager>((PORT_NUM));
	GNetworkManager->Run();
	
	BOOST_LOG_TRIVIAL(info) << "네트워크 접속 종료";
	getchar();

	return 0;
}
