#pragma once

namespace S4Network
{
	class ClientSession;
	typedef std::shared_ptr<ClientSession> ClientSessionPtr;

	class ClientSessionManager
	{
	public:
		ClientSessionManager() {}
		~ClientSessionManager() {}

		void PrepareClientSession();
		bool AcceptClientSession();
				
		void ReturnClientSession(ClientSessionPtr client);

	private:
		typedef std::list<ClientSessionPtr> ClientList;
		ClientList mFreeSessionList;

		// FastSpinlock mLock;

		uint64_t mCurrentIssueCount;
		uint64_t mCurrentReturnCount;
	};

	extern std::shared_ptr<ClientSessionManager> GClientSessionManager;
}
