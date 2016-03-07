#include "stdafx.h"
#include "ClientSessionManager.h"
#include "ClientSession.h"
#include "ThreadLocal.h"
#include "Exception.h"
#include "Log.h"

#include <boost/lexical_cast.hpp>

namespace S4Framework
{
	std::unique_ptr<ClientSessionManager> GClientSessionManager;

	ClientSessionManager::~ClientSessionManager()
	{
		for (auto& toBeDelete : mClientSessionList )
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
		
		for (int i = 0; i < mClientSessionList.size(); ++i)
		{
			ClientSession* client = new ClientSession(i, mAcceptor.get_io_service());
			mClientSessionList[i] = client;
			mClientSessionQueue.push_back(i);
		}
	}

	void ClientSessionManager::AcceptClientSession()
	{
		auto f = [=]
		{
			if (mClientSessionQueue.empty())
			{
				mIsAccepting = false;
				return;
			}

			mIsAccepting = true;
			int sessionID = mClientSessionQueue.front();
			mClientSessionQueue.pop_front();

			if (mClientSessionList.size() > sessionID && mClientSessionList[sessionID])
			{
				ClientSession* newClient = mClientSessionList[sessionID];

				newClient->Reset();
				newClient->AddRefCount(); ///< refcount +1 for issuing 

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
			std::string ip = boost::lexical_cast<std::string>(client->GetSocket().remote_endpoint());
			BOOST_LOG_TRIVIAL(info) << "클라이언트 접속 성공. " << client->GetSessionID() << ":" << ip;

			client->PostRecv();
			AcceptClientSession();
		}
		else
		{
			std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
		}
	}

	void ClientSessionManager::ReturnClientSession(const int sessionID)
	{
		auto f = [=]
		{
			if (mClientSessionList.size() > sessionID && mClientSessionList[sessionID])
			{
				ClientSession* client = mClientSessionList[sessionID];
				CRASH_ASSERT(client->mConnected == 0 && client->mRefCount == 0);

				std::string ip = boost::lexical_cast<std::string>(client->GetSocket().remote_endpoint());
				BOOST_LOG_TRIVIAL(info) << "클라이언트 접속 종료. " << client->GetSessionID() << ":" << ip;

				client->Reset();
				mClientSessionQueue.push_back(sessionID);

				++mCurrentReturnCount;

				if (mIsAccepting == false)
				{
					AcceptClientSession();
				}
			}
		};
		auto task = mWrapper.wrap(f);
		mDispatcher.post(task);
	}
}
