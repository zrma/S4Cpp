#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace S4Thread
{
	class ConcurrentJobManager
	{
	public:
		ConcurrentJobManager(boost::asio::io_service& io_service, std::size_t size);
		~ConcurrentJobManager();

	private:
		boost::asio::io_service& m_io_service;
		boost::thread_group m_Group;

		std::shared_ptr<boost::asio::io_service::work> m_Work;
	};

	template <class F>
	void PostJob(F f)
	{
		m_io_service.post(f);
	}
}
