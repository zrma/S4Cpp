#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace S4Thread
{
	class ConcurrentJobManager
	{
	public:
		ConcurrentJobManager(std::size_t size);
		~ConcurrentJobManager();

		template <class F>
		void PostJob(F f)
		{
			m_io_service.post(f);
		}

		boost::asio::io_service& GetHandler() { return m_io_service; }

	private:
		boost::asio::io_service m_io_service;
		boost::thread_group m_Group;

		std::shared_ptr<boost::asio::io_service::work> m_Work;
	};

}
