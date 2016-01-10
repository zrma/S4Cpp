// S4Game.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "../../S4Thread/ThreadLocal.h"
#include "../../S4Thread/LockOrderChecker.h"
#include "../../S4Thread/ConcurrentJobManager.h"
#include "../../S4Network/NetworkManager.h"

#include <boost/log/trivial.hpp>

// #include "../../S4Util/Exception.h"
#include "../../S4Util/ObjectPool.h"
#include "../../S4Util/TimeStamp.h"

#include <deque>

#pragma comment(lib, "S4Thread.lib")
#pragma comment(lib, "S4Network.lib")

#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "S4Util.lib")

const int PORT_NUM = 35555;

S4Network::NetworkManager GNetworkManager(PORT_NUM);
S4Thread::ConcurrentJobManager GJobManager(8);

void Test();

int main()
{
	/// for dump on crash
	SetUnhandledExceptionFilter(S4Util::ExceptionFilter);

	S4Thread::LThreadType = S4Thread::THREAD_TYPE::THREAD_MAIN;
	// GNetworkManager.Run();
	
	Test();

	// BOOST_LOG_TRIVIAL(info) << "네트워크 접속 종료";
	getchar();

    return 0;
}

class T
{
public:
	int a;
	int b;

	void Func()
	{
		a = b;
	}
};

class T2 : public S4Util::ObjectPool<T2>
{
public:
	int a;
	int b;

	void Func()
	{
		a = b;
	}
};

void Test()
{
	S4Thread::LLockOrderChecker = new S4Thread::LockOrderChecker(S4Thread::LThreadType);

	{
		S4Util::TimeStamp st("Raw new/delete");

		std::deque<T*> vec;
		
		for (std::size_t i = 0; i < 100000; ++i )
		{
			T* t = new T();
			t->Func();

			vec.push_back(t);

			t = new T();
			t->Func();

			vec.push_front(t);
		}

		for (std::size_t i = 0; i < 100000; ++i)
		{
			T* t = vec.front();
			delete t;

			vec.pop_front();
		}
	}

	{
		S4Util::TimeStamp st("Object Pool new/delete");

		std::deque<T2*> vec;

		for (std::size_t i = 0; i < 100000; ++i)
		{
			T2* t = new T2();
			t->Func();

			vec.push_back(t);
			
			t = new T2();
			t->Func();

			vec.push_front(t);
		}

		for (std::size_t i = 0; i < 100000; ++i)
		{
			T2* t = vec.front();
			delete t;

			vec.pop_front();
		}
	}
}