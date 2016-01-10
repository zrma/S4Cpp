#include "stdafx.h"
#include "FastSpinlock.h"
#include "LockOrderChecker.h"
#include "ThreadLocal.h"

#pragma comment(lib, "winmm.lib")

namespace S4Thread
{
	FastSpinlock::FastSpinlock(const int lockOrder) : mLockFlag(0), mLockOrder(lockOrder)
	{
	}


	FastSpinlock::~FastSpinlock()
	{
	}


	void FastSpinlock::EnterLock()
	{
		/// �� ���� �Ű� �Ƚᵵ �Ǵ� ���� �׳� �н�
		if (mLockOrder != LO_DONT_CARE)
			LLockOrderChecker->Push(this);

		for (int nloops = 0; ; nloops++)
		{
			if (InterlockedExchange(&mLockFlag, 1) == 0)
				return;

			UINT uTimerRes = 1;
			timeBeginPeriod(uTimerRes);
			Sleep((DWORD)min(10, nloops));
			timeEndPeriod(uTimerRes);
		}

	}

	void FastSpinlock::LeaveLock()
	{
		InterlockedExchange(&mLockFlag, 0);

		/// �� ���� �Ű� �Ƚᵵ �Ǵ� ���� �׳� �н�
		if (mLockOrder != LO_DONT_CARE)
			LLockOrderChecker->Pop(this);
	}
}
