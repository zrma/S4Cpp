#pragma once

#include <boost/asio.hpp>

namespace S4Framework
{
	class ClientSession;
	class ClientSessionManager
	{
	public:
		ClientSessionManager(int port, boost::asio::io_service& dispatcher)
			: mWrapper(dispatcher)
			, mDispatcher(dispatcher)
			, mAcceptor(dispatcher, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
		{}
		ClientSessionManager() = delete;
		ClientSessionManager(const ClientSessionManager&) = delete;
		ClientSessionManager& operator=(const ClientSessionManager&) = delete;
		~ClientSessionManager();

		void PrepareClientSession( std::size_t maxConnection );
		void AcceptClientSession();
		void ReturnClientSession( const std::size_t sessionId);
		void PrintSessionState();

	private:
		void AcceptComplete( ClientSession* client, const boost::system::error_code& error );

		typedef boost::asio::strand SyncWrapper;
		SyncWrapper mWrapper;

		typedef std::vector<ClientSession*> ClientList;
		ClientList mClientSessionList;
		std::unordered_set<int> mClientSessionQueue;

		uint64_t mCurrentIssueCount = 0;
		uint64_t mCurrentReturnCount = 0;

		std::size_t mMaxConnection = 0;

		boost::asio::io_service&		mDispatcher;
		boost::asio::ip::tcp::acceptor	mAcceptor;
		bool	mIsAccepting = true;
	};

	extern std::unique_ptr<ClientSessionManager> GClientSessionManager;
}