#include "stdafx.h"
#include "ConcurrentJobManager.h"

S4Thread::ConcurrentJobManager::ConcurrentJobManager(std::size_t size)
{
	m_Work = std::make_shared<boost::asio::io_service::work>(m_io_service);

	for (std::size_t i = 0; i < size; ++i)
	{
		m_Group.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
	}
}


S4Thread::ConcurrentJobManager::~ConcurrentJobManager()
{
	m_Work.reset();
	m_Group.join_all();
}
