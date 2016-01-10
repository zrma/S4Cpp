#include "stdafx.h"
#include "TimeStamp.h"
#include <boost/log/trivial.hpp>

namespace S4Util
{
	TimeStamp::TimeStamp(const std::string label)
		:m_Label(label), m_Start(std::chrono::system_clock::now())
	{
	}

	TimeStamp::~TimeStamp()
	{
		auto m_ElapsedTime = (std::chrono::system_clock::now() - m_Start).count();
		BOOST_LOG_TRIVIAL(info) << "\n--------------------------------------\n"
			<< m_Label << " Perf TimeStamp \n  "
			<< "Elapsed Time : " << m_ElapsedTime << "ns \n--------------------------------------\n";
	}
}
