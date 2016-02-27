#pragma once

#pragma warning(push)
#pragma warning(disable: 4005)
#include <boost/asio.hpp>
#pragma warning(pop)

#include <boost/thread.hpp>

#include "../S4Thread/ThreadLocal.h"
#include "../S4Thread/IConcurrentPool.h"

namespace S4Network
{
	class NetworkManager : S4Thread::IConcurrentPool
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