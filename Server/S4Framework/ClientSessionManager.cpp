#include "stdafx.h"
#include "ClientSessionManager.h"
#include "ClientSession.h"
#include "ThreadLocal.h"
#include "Exception.h"

namespace S4Framework
{
	std::unique_ptr<ClientSessionManager> GClientSessionManager;

	ClientSessionManager::~ClientSessionManager()
	{
		for (auto& toBeDelete : mFreeSessionList )
		{
			delete toBeDelete;
		}
		mFreeSessionList.clear();
	}

	void ClientSessionManager::PrepareClientSession()
	{
		CRASH_ASSERT(LThreadType == THREAD_MAIN);

		for (int i = 0; i < mMaxConnection; ++i)
		{
			ClientSession* client = new ClientSession(i, mAcceptor.get_io_service());
			mFreeSessionList.push_back(client);
		}
	}

	bool ClientSessionManager::AcceptClientSession()
	{
		auto f = [=]
		{
			while (mCurrentIssueCount - mCurrentReturnCount < mMaxConnection)
			{
				ClientSession* newClient = mFreeSessionList.back();
				mFreeSessionList.pop_back();

				++mCurrentIssueCount;

				newClient->AddRefCount(); ///< refcount +1 for issuing 
				newClient->PostAccept(mAcceptor);
			}
		};
		auto task = mWrapper.wrap(f);
		mDispatcher.post(task);
		
		return true;
	}

	void ClientSessionManager::ReturnClientSession(ClientSession* client)
	{
		auto f = [=]
		{
			CRASH_ASSERT(client->mConnected == 0 && client->mRefCount == 0);

			client->Reset();

			mFreeSessionList.push_back(client);

			++mCurrentReturnCount;
		};
		auto task = mWrapper.wrap(f);
		mDispatcher.post(task);
	}
}
