#pragma once

#pragma warning(push)
#pragma warning(disable: 4005)
#include <boost/asio.hpp>
#pragma warning(pop)

namespace S4Network
{
	class NetworkManager
	{
	public:
		NetworkManager(int port);
		~NetworkManager();

		void	Init();
		void	Run();

	private:
		void	StartAccept();

		boost::asio::io_service			mIOService;
		boost::asio::ip::tcp::acceptor	mAcceptor;
		
		bool	mIsAccepting = false;
		int		mSeqNumber = 0;
	};
};