#pragma once

#include <boost/asio.hpp>

namespace S4Framework
{
	class ClientSession;
	class ClientSessionManager
	{
	public:
		ClientSessionManager(int port, boost::asio::io_service& dispatcher)
			: mDispatcher(dispatcher)
			, mAcceptor(dispatcher, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
			, mWrapper(dispatcher) {}
		~ClientSessionManager();

		void PrepareClientSession(std::size_t maxConnection);
		void AcceptClientSession();
		void ReturnClientSession(const int sessionID);

	private:
		void AcceptComplete(ClientSession* client, const boost::system::error_code& error);

		typedef boost::asio::strand SyncWrapper;
		SyncWrapper mWrapper;

		typedef std::vector<ClientSession*> ClientList;
		ClientList mClientSessionList;
		std::deque<int> mClientSessionQueue;

		uint64_t mCurrentIssueCount = 0;
		uint64_t mCurrentReturnCount = 0;

		std::size_t mMaxConnection = 0;

		boost::asio::io_service&		mDispatcher;
		boost::asio::ip::tcp::acceptor	mAcceptor;
		bool	mIsAccepting = true;
	};

	extern std::unique_ptr<ClientSessionManager> GClientSessionManager;
}
