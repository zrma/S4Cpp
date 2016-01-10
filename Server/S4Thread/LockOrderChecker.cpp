#include "stdafx.h"
#include "../S4Util/Exception.h"
#include "ThreadLocal.h"
#include "FastSpinlock.h"
#include "LockOrderChecker.h"

namespace S4Thread
{
	LockOrderChecker::LockOrderChecker(int tid) : mWorkerThreadId(tid), mStackTopPos(0)
	{
		memset(mLockStack, 0, sizeof(mLockStack));
	}

	void LockOrderChecker::Push(FastSpinlock* lock)
	{
		S4Util::CRASH_ASSERT(mStackTopPos < MAX_LOCK_DEPTH);

		if (mStackTopPos > 0)
		{
			/// ���� ���� �ɷ� �ִ� ���¿� �����Ѱ��� �ݵ�� ���� ���� �켱������ ���ƾ� �Ѵ�.
			S4Util::CRASH_ASSERT(mLockStack[mStackTopPos - 1]->mLockOrder < lock->mLockOrder);
		}

		mLockStack[mStackTopPos++] = lock;
	}

	void LockOrderChecker::Pop(FastSpinlock* lock)
	{

		/// �ּ��� ���� ���� �ִ� ���¿��� �� ���̰�
		S4Util::CRASH_ASSERT(mStackTopPos > 0);

		/// �翬�� �ֱٿ� push�ߴ� �༮�̶� ���ƾ߰���..
		S4Util::CRASH_ASSERT(mLockStack[mStackTopPos - 1] == lock);

		mLockStack[--mStackTopPos] = nullptr;

	}
}