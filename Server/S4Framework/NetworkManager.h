#pragma once

#include "IConcurrentPool.h"

constexpr auto MAX_CONNECTION = 10000;

namespace S4Framework
{
	class NetworkManager : public IConcurrentPool
	{
	public:
		explicit NetworkManager( int port, std::size_t size = MAX_IO_THREAD );
		~NetworkManager();

		void	StartAccept( std::size_t size = MAX_CONNECTION );
	private:
		virtual void InitThread() override;
		virtual void Run() override;

		static void	DoSendJob();

		int		mPort = 0;
		bool	mIsContinue = true;
	};
};