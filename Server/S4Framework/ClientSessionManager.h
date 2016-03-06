#pragma once

#include <boost/asio.hpp>

namespace S4Framework
{
	class ClientSession;
	class ClientSessionManager
	{
	public:
		ClientSessionManager(int port, std::size_t size, boost::asio::io_service& dispatcher)
			: mMaxConnection(size)
			, mDispatcher(dispatcher)
			, mAcceptor(dispatcher, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
			, mWrapper(dispatcher) {}
		~ClientSessionManager();

		void PrepareClientSession();
		bool AcceptClientSession();

		void ReturnClientSession(ClientSession* client);

	private:
		typedef boost::asio::strand SyncWrapper;
		SyncWrapper mWrapper;
		
		typedef std::list<ClientSession*> ClientList;
		ClientList mFreeSessionList;

		uint64_t mCurrentIssueCount = 0;
		uint64_t mCurrentReturnCount = 0;

		std::size_t mMaxConnection = 0;

		boost::asio::io_service&		mDispatcher;
		boost::asio::ip::tcp::acceptor	mAcceptor;
		bool	mIsAcceptable = true;
	};

	extern std::unique_ptr<ClientSessionManager> GClientSessionManager;
}
