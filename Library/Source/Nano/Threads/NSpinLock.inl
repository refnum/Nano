/*	NAME:
		NSpinLock.inl

	DESCRIPTION:
		Spin lock.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoMacros.h"

#if !NN_PLATFORM_DARWIN
	#include "NThread.h"
	#include "NTimeUtils.h"
#endif // !NN_PLATFORM_DARWIN

// System
#if NN_PLATFORM_DARWIN
	#include <os/lock.h>
#else
	#include <atomic>
#endif // NN_PLATFORM_DARWIN





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr size_t kNSpinLockLoopsYield                = 1000;





//=============================================================================
//		Darwin
//-----------------------------------------------------------------------------
//		Darwin must never use a looping spin-lock.
//
//		On Darwin platforms threads in a higher QoS class will never decay
//		to those in a lower class, and the scheduler will always prioritise
//		runnable threads in a given QoS class before those of a lower class.
//
//		As the thread spinning for the lock is always runnable, a high-QoS
//		thread may spin forever if a low-QoS thread currently has the lock.
//
//			https://lists.swift.org/pipermail/swift-dev/Week-of-Mon-20151214/000372.html
//			https://blog.postmates.com/why-spinlocks-are-bad-on-ios-b69fc5221058
//
//		The os_unfair_lock API is effectively a spin-lock in terms of weight,
//		but it allows hand-off between threads of different QoS classes if
//		the thread would need to spin.
//-----------------------------------------------------------------------------
#if NN_PLATFORM_DARWIN

class NSpinLock : public NSpinLock_<os_unfair_lock>
{
public:
	NSpinLock()
	{
		// Initialise ourselves
		mLock = OS_UNFAIR_LOCK_INIT;
	}

	bool Lock(NInterval waitFor = kNTimeForever)
	{
		// Acquire the lock
		//
		// We can't spin, so always ignore our timeout.
		NN_UNUSED(waitFor);
		os_unfair_lock_lock(&mLock);

		return true;
	}

	void Unlock()
	{
		// Validate our state
	#if NN_DEBUG
		os_unfair_lock_assert_owner(&mLock);
	#endif    // NN_DEBUG


		// Release the lock
		os_unfair_lock_unlock(&mLock);
	}
};





//=============================================================================
//		Generic
//-----------------------------------------------------------------------------
//		Other platforms use a looping spin-lock.
//
//		We pause each time through the loop to let the processor know we are
//		spinning, to avoid starving other threads on this core.
//
//		If we've been spinning for long enough we also explicitly yield to
//		allow another thread to run in our place.
//
//		Spinning can lead to priority inversion, however some platforms such
//		as Windows will randomise thread priorities to avoid this situation.
//-----------------------------------------------------------------------------
#else

class NSpinLock : public NSpinLock_<std::atomic_flag>
{
public:
	NSpinLock()
	{
		// Initialise ourselves
		mLock.clear();
	}

	bool Lock(NInterval waitFor = kNTimeForever)
	{
		// Get the state we need
		if (waitFor == kNTimeForever)
		{
			waitFor = kNTimeDistantFuture;
		}

		NInterval stopTime = NTimeUtils::GetTime() + waitFor;


		// Acquire the lock
		do
		{
			for (size_t n = 0; n < kNSpinLockLoopsYield; n++)
			{
				if (mLock.test_and_set(std::memory_order_acquire))
				{
					return true;
				}

				NThread::Pause();
			}

			NThread::Switch();
		} while (NTimeUtils::GetTime() < stopTime);

		return false;
	}

	void Unlock()
	{
		// Release the lock
		mLock.clear(std::memory_order_release);
	}
};


#endif
