#pragma once

#include <string>
#include <chrono>

namespace S4Util
{
	class TimeStamp
	{
	public:
		TimeStamp(const std::string label);
		~TimeStamp();

	private:
		std::string m_Label;
		std::chrono::system_clock::time_point m_Start;
	};
}
