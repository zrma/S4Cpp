#pragma once

#pragma warning(push)
#pragma warning(disable: 4005)
#include <boost/asio.hpp>
#pragma warning(pop)

#include <boost/thread.hpp>

#include "ThreadLocal.h"
#include "IConcurrentPool.h"

namespace S4Framework
{
	class NetworkManager : IConcurrentPool
	{
	public:
		NetworkManager(int port, std::size_t size = MAX_IO_THREAD);
		~NetworkManager();

		void	Run();
		
	private:
		void	StartAccept();

		boost::asio::ip::tcp::acceptor	mAcceptor;

		bool	mIsAccepting = false;
		int		mSeqNumber = 0;
	};
};