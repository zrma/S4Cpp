#pragma once

namespace S4Framework
{
	LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS* exceptionInfo);

	inline void CRASH_ASSERT(bool isOk)
	{
		if (isOk)
		{
			return;
		}

		int* crashVal = nullptr;
		*crashVal = 0xDEADBEEF;
	}
}