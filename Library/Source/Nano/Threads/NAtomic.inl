/*	NAME:
		NAtomic.inl

	DESCRIPTION:
		Atomic variable.

		Types that are trivially copyable are implemented with std::atomic.

		Types that are not trivially copyable are protected with a mutex.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
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
#include "NMixinComparable.h"
#include "NMutex.h"
#include "NScopedLock.h"





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<typename T>
class NAtomic<T, std::enable_if_t<std::is_trivially_copyable_v<T>>>
{
public:
	static constexpr bool is_always_lock_free = std::atomic<T>::is_always_lock_free;

	NAtomic()
		: mValue{}
	{
	}

	NAtomic(T theValue)
		: mValue(theValue)
	{
	}

	T operator=(T theValue)
	{
		mValue = theValue;
		return theValue;
	}

	bool is_lock_free() const
	{
		return mValue.is_lock_free();
	}

	void store(T newValue, std::memory_order memOrder = std::memory_order_seq_cst)
	{
		mValue.store(newValue, memOrder);
	}

	T load(std::memory_order memOrder = std::memory_order_seq_cst) const
	{
		return mValue.load(memOrder);
	}

	 operator T() const
	{
		return load();
	}

	T exchange(T newValue, std::memory_order memOrder = std::memory_order_seq_cst)
	{
		return mValue.exchange(newValue, memOrder);
	}

	bool                                compare_exchange_weak(T&                valueExpected,
															  T                 valueDesired,
															  std::memory_order memSuccess,
															  std::memory_order memFailure)
	{
		return mValue.compare_exchange_weak(valueExpected, valueDesired, memSuccess, memFailure);
	}

	bool                                compare_exchange_weak(T&                valueExpected,
															  T                 valueDesired,
															  std::memory_order memOrder = std::memory_order_seq_cst)
	{
		return mValue.compare_exchange_weak(valueExpected, valueDesired, memOrder);
	}

	bool                                compare_exchange_strong(T&                valueExpected,
																T                 valueDesired,
																std::memory_order memSuccess,
																std::memory_order memFailure)
	{
		return mValue.compare_exchange_strong(valueExpected, valueDesired, memSuccess, memFailure);
	}

	bool                                compare_exchange_strong(T&                valueExpected,
																T                 valueDesired,
																std::memory_order memOrder = std::memory_order_seq_cst)
	{
		return mValue.compare_exchange_strong(valueExpected, valueDesired, memOrder);
	}



private:
	std::atomic<T>                      mValue;
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<typename T>
class NAtomic<T, std::enable_if_t<!std::is_trivially_copyable_v<T>>>
	: public NMixinComparable<NAtomic<T>>
{
public:
	static constexpr bool is_always_lock_free = false;

	NAtomic()
		: mValue{}
	{
	}

	NAtomic(const T& theValue)
		: mValue(theValue)
	{
	}

	T operator=(const T& theValue) noexcept
	{
		NScopedLock acquireLock(mLock);

		mValue = theValue;
		return theValue;
	}

	bool is_lock_free() const
	{
		return false;
	}

	void store(const T& newValue, std::memory_order memOrder = std::memory_order_seq_cst)
	{
		NScopedLock acquireLock(mLock);
		NN_UNUSED(memOrder);

		mValue = newValue;
	}

	T load(std::memory_order memOrder = std::memory_order_seq_cst) const
	{
		NScopedLock acquireLock(mLock);
		NN_UNUSED(memOrder);

		return mValue;
	}

	 operator T() const
	{
		return load();
	}

	T exchange(T newValue, std::memory_order memOrder = std::memory_order_seq_cst)
	{
		NScopedLock acquireLock(mLock);
		NN_UNUSED(memOrder);

		std::swap(mValue, newValue);
		return newValue;
	}

	bool                                compare_exchange_weak(T&                valueExpected,
															  const T&          valueDesired,
															  std::memory_order memSuccess,
															  std::memory_order memFailure)
	{
		return compare_exchange_strong(valueExpected, valueDesired, memSuccess, memFailure);
	}

	bool                                compare_exchange_weak(T&                valueExpected,
															  const T&          valueDesired,
															  std::memory_order memOrder = std::memory_order_seq_cst)
	{
		return compare_exchange_strong(valueExpected, valueDesired, memOrder, memOrder);
	}

	bool                                compare_exchange_strong(T&       valueExpected,
																const T& valueDesired,
																std::memory_order /*memSuccess*/,
																std::memory_order /*memFailure*/)
	{
		NScopedLock acquireLock(mLock);

		if (mValue == valueExpected)
		{
			mValue = valueDesired;
			return true;
		}
		else
		{
			valueExpected = mValue;
			return false;
		}
	}

	bool                                compare_exchange_strong(T&                valueExpected,
																const T&          valueDesired,
																std::memory_order memOrder = std::memory_order_seq_cst)
	{
		return compare_exchange_strong(valueExpected, valueDesired, memOrder, memOrder);
	}


public:
	// NMixinComparable
	bool CompareEqual(const T& otherValue) const
	{
		return mValue == otherValue;
	}

	NComparison CompareOrder(const T& otherValue) const
	{
		return NCompare(mValue, otherValue);
	}


private:
	mutable NMutex                      mLock;
	T                                   mValue;
};
