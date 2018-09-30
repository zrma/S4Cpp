#include "stdafx.h"
#include "ClientSessionManager.h"
#include "ClientSession.h"
#include "ThreadLocal.h"
#include "Exception.h"
#include "Log.h"

#include <boost/lexical_cast.hpp>
#include <boost/asio/use_future.hpp>

namespace S4Framework
{
	std::unique_ptr<ClientSessionManager> GClientSessionManager;

	ClientSessionManager::~ClientSessionManager()
	{
		for (auto& toBeDelete : mClientSessionList)
		{
			if (toBeDelete->GetSocket().is_open())
			{
				toBeDelete->GetSocket().close();
			}

			delete toBeDelete;
		}
		mClientSessionList.clear();
		mClientSessionQueue.clear();
	}

	void ClientSessionManager::PrepareClientSession(std::size_t maxConnection)
	{
		CRASH_ASSERT(LThreadType == THREAD_MAIN);

		mMaxConnection = maxConnection;

		for (auto& toBeDelete : mClientSessionList)
		{
			if (toBeDelete == nullptr)
			{
				continue;
			}

			if (toBeDelete->GetSocket().is_open())
			{
				toBeDelete->GetSocket().close();
			}

			delete toBeDelete;
		}
		mClientSessionList.clear();
		mClientSessionQueue.clear();

		mClientSessionList.resize(mMaxConnection);

		auto size = mClientSessionList.size();
		for (decltype(size) i = 0; i < size; ++i)
		{
			auto* client = new ClientSession(i, mAcceptor.get_io_service());
			mClientSessionList[i] = client;
			mClientSessionQueue.insert(static_cast<int>(i));
		}
	}

	void ClientSessionManager::AcceptClientSession()
	{
		const auto f = [=]
		{
			if (mClientSessionQueue.empty())
			{
				mIsAccepting = false;
				return;
			}

			mIsAccepting = true;
			auto size = mClientSessionList.size();
			const decltype(size) sessionId = *mClientSessionQueue.begin();
			mClientSessionQueue.erase(static_cast<int>(sessionId));

			if (size > sessionId && mClientSessionList[sessionId])
			{
				auto newClient = mClientSessionList[sessionId];

				newClient->Reset();
				newClient->AddRefCount(); ///< ref count +1 for issuing 

				/*mAcceptor.async_accept(newClient->GetSocket(),
					boost::bind(&ClientSessionManager::AcceptComplete, this, newClient, boost::asio::placeholders::error));*/

				mAcceptor.async_accept(newClient->GetSocket(),
					boost::bind(&ClientSessionManager::AcceptComplete, this, newClient, boost::asio::placeholders::error));

				++mCurrentIssueCount;
			}
		};
		auto task = mWrapper.wrap(f);
		mDispatcher.post(task);
	}

	void ClientSessionManager::AcceptComplete(ClientSession* client, const boost::system::error_code& error)
	{
		if (!error)
		{
			// std::string ip = boost::lexical_cast<std::string>(client->GetSocket().remote_endpoint());
			// BOOST_LOG_TRIVIAL(info) << "클라이언트 접속 성공. " << client->GetSessionId() << ":" << ip;
			// std::cout << "클라이언트 접속 성공. " << client->GetSessionId() << ":" << ip << std::endl;

			client->AcceptComplete();
			client->PostReceive();
			AcceptClientSession();
		}
		else
		{
			// BOOST_LOG_TRIVIAL(error) << "error No: " << error.value() << " error Message: " << error.message();
			std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
		}
	}

	void ClientSessionManager::ReturnClientSession(const std::size_t sessionId)
	{
		const auto f = [=]
		{
			decltype(sessionId) size = mClientSessionList.size();
			if (size > sessionId && mClientSessionList[sessionId])
			{
				ClientSession* client = mClientSessionList[sessionId];
				CRASH_ASSERT(client->mConnected == 0 && client->mRefCount == 0);

				// std::string ip = boost::lexical_cast<std::string>(client->GetSocket().remote_endpoint());
				// BOOST_LOG_TRIVIAL(info) << "클라이언트 접속 종료. " << client->GetSessionId() << ":" << ip;
				// std::cout << "클라이언트 접속 종료. " << client->GetSessionId() << ":" << ip << std::endl;

				client->Reset();
				mClientSessionQueue.insert(static_cast<int>(sessionId));

				++mCurrentReturnCount;

				if (!mIsAccepting)
				{
					AcceptClientSession();
				}
			}
		};
		auto task = mWrapper.wrap(f);
		mDispatcher.post(task);
	}

	void ClientSessionManager::PrintSessionState()
	{
		const auto f = [=]
		{
			std::cout << "session state : (" << mClientSessionQueue.size() << "/" << mClientSessionList.size() << ")" << std::endl;
			PrintMemoryInfo();
		};
		auto task = mWrapper.wrap(f);
		mDispatcher.post(task);
	}
}