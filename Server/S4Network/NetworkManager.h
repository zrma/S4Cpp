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
		NetworkManager();
		~NetworkManager();

		void	Init(int Port);

	private:
		std::shared_ptr<boost::asio::ip::tcp::acceptor>		mAcceptor;
		std::shared_ptr<boost::asio::io_service>			mIOService;

		bool	mIsAccepting = false;
		int		mSeqNumber = 0;
	};
};