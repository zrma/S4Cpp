#pragma once

#include "IConcurrentPool.h"

namespace S4Framework
{	
	class NetworkManager : public IConcurrentPool
	{
	public:
		NetworkManager(int port, std::size_t size = MAX_IO_THREAD);
		~NetworkManager();

		void	Run();
		
	private:
		virtual void InitThread();

		void	StartAccept();

		boost::asio::ip::tcp::acceptor	mAcceptor;

		bool	mIsAccepting = false;
		int		mSeqNumber = 0;
	};
};