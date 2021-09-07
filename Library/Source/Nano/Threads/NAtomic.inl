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
//		Internal Macros
//-----------------------------------------------------------------------------
// NAtomic implementations
//
// std::atomic can only hold types that are trivially copyable.
//
// More complex types can be made atomic by protecting access with a mutex.
#define NAtomic_ATOMIC                                      NAtomic<T, std::enable_if_t<std::is_trivially_copyable_v<T>>>
#define NAtomic_MUTEX                                       NAtomic<T, std::enable_if_t<!std::is_trivially_copyable_v<T>>>





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<typename T>
class NAtomic_ATOMIC
{
public:
	static constexpr bool is_always_lock_free = std::atomic<T>::is_always_lock_free;

										NAtomic();
										NAtomic(T theValue);

	T                                   operator=(T theValue);
	NAtomic<T>&                         operator=(const NAtomic<T>& theValue) = delete;


	// Is the implementation lock-free?
	bool                                is_lock_free() const;


	// Store / load a value
	void                                store(T newValue, std::memory_order memOrder = std::memory_order_seq_cst);
	T                                   load(std::memory_order memOrder = std::memory_order_seq_cst) const;


	// Cast to T
										operator T() const;


	// Atomically replace the value
	T                                   exchange(T newValue, std::memory_order memOrder = std::memory_order_seq_cst);


	// Compare-exchange the value
	bool                                compare_exchange_weak(T&                valueExpected,
															  T                 valueDesired,
															  std::memory_order memSuccess,
															  std::memory_order memFailure);

	bool                                compare_exchange_weak(T&                valueExpected,
															  T                 valueDesired,
															  std::memory_order memOrder = std::memory_order_seq_cst);

	bool                                compare_exchange_strong(T&                valueExpected,
																T                 valueDesired,
																std::memory_order memSuccess,
																std::memory_order memFailure);

	bool                                compare_exchange_strong(T&                valueExpected,
																T                 valueDesired,
																std::memory_order memOrder = std::memory_order_seq_cst);


private:
	std::atomic<T>                      mValue;
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<typename T>
class NAtomic_MUTEX : public NMixinComparable<NAtomic<T>>
{
public:
	static constexpr bool is_always_lock_free = false;

										NAtomic();
										NAtomic(const T& theValue);

	T                                   operator=(const         T&  theValue);
	NAtomic<T>&                         operator=(const NAtomic<T>& theValue) = delete;


	// Is the implementation lock-free?
	bool                                is_lock_free() const;


	// Store / load a value
	void                                store(const T& newValue, std::memory_order memOrder = std::memory_order_seq_cst);
	T                                   load(std::memory_order memOrder = std::memory_order_seq_cst) const;


	// Cast to T
										operator T() const;


	// Atomically replace the value
	T                                   exchange(T newValue, std::memory_order memOrder = std::memory_order_seq_cst);


	// Compare-exchange the value
	bool                                compare_exchange_weak(T&                valueExpected,
															  const T&          valueDesired,
															  std::memory_order memSuccess,
															  std::memory_order memFailure);

	bool                                compare_exchange_weak(T&                valueExpected,
															  const T&          valueDesired,
															  std::memory_order memOrder = std::memory_order_seq_cst);

	bool                                compare_exchange_strong(T&                valueExpected,
																const T&          valueDesired,
																std::memory_order memSuccess,
																std::memory_order memFailure);

	bool                                compare_exchange_strong(T&                valueExpected,
																const T&          valueDesired,
																std::memory_order memOrder = std::memory_order_seq_cst);


public:
	// NMixinComparable
	bool                                CompareEqual(const T& otherValue) const;
	NComparison                         CompareOrder(const T& otherValue) const;


private:
	mutable NMutex                      mLock;
	T                                   mValue;
};





#pragma mark NAtomic_ATOMIC
//=============================================================================
//		NAtomic::NAtomic : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
NAtomic_ATOMIC::NAtomic()
	: mValue{}
{
}





//=============================================================================
//		NAtomic::NAtomic : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
NAtomic_ATOMIC::NAtomic(T theValue)
	: mValue(theValue)
{
}





//=============================================================================
//		NAtomic::operator= : Assignment operator.
//-----------------------------------------------------------------------------
template<typename T>
T NAtomic_ATOMIC::operator=(T theValue)
{
	mValue = theValue;
	return theValue;
}





//=============================================================================
//		NAtomic::is_lock_free : Is access lock-free?
//-----------------------------------------------------------------------------
template<typename T>
bool NAtomic_ATOMIC::is_lock_free() const
{
	return mValue.is_lock_free();
}





//=============================================================================
//		NAtomic::store : Store a value.
//-----------------------------------------------------------------------------
template<typename T>
void NAtomic_ATOMIC::store(T newValue, std::memory_order memOrder)
{
	mValue.store(newValue, memOrder);
}





//=============================================================================
//		NAtomic::load : Load the value.
//-----------------------------------------------------------------------------
template<typename T>
T NAtomic_ATOMIC::load(std::memory_order memOrder) const
{
	return mValue.load(memOrder);
}





//=============================================================================
//		NAtomic::operator T : Cast operator.
//-----------------------------------------------------------------------------
template<typename T>
NAtomic_ATOMIC::operator T() const
{
	return load();
}





//=============================================================================
//		NAtomic::exchange : Exchange the current value.
//-----------------------------------------------------------------------------
template<typename T>
T NAtomic_ATOMIC::exchange(T newValue, std::memory_order memOrder)
{
	return mValue.exchange(newValue, memOrder);
}





//=============================================================================
//		NAtomic::compare_exchange_weak : Compare and exchange.
//-----------------------------------------------------------------------------
template<typename T>
bool NAtomic_ATOMIC::compare_exchange_weak(T&                valueExpected,
										   T                 valueDesired,
										   std::memory_order memSuccess,
										   std::memory_order memFailure)
{
	return mValue.compare_exchange_weak(valueExpected, valueDesired, memSuccess, memFailure);
}





//=============================================================================
//		NAtomic::compare_exchange_weak : Compare and exchange.
//-----------------------------------------------------------------------------
template<typename T>
bool NAtomic_ATOMIC::compare_exchange_weak(T&                valueExpected,
										   T                 valueDesired,
										   std::memory_order memOrder)
{
	return mValue.compare_exchange_weak(valueExpected, valueDesired, memOrder);
}





//=============================================================================
//		NAtomic::compare_exchange_strong : Compare and exchange.
//-----------------------------------------------------------------------------
template<typename T>
bool NAtomic_ATOMIC::compare_exchange_strong(T&                valueExpected,
											 T                 valueDesired,
											 std::memory_order memSuccess,
											 std::memory_order memFailure)
{
	return mValue.compare_exchange_strong(valueExpected, valueDesired, memSuccess, memFailure);
}





//=============================================================================
//		NAtomic::compare_exchange_strong : Compare and exchange.
//-----------------------------------------------------------------------------
template<typename T>
bool NAtomic_ATOMIC::compare_exchange_strong(T&                valueExpected,
											 T                 valueDesired,
											 std::memory_order memOrder)
{
	return mValue.compare_exchange_strong(valueExpected, valueDesired, memOrder);
}





#pragma mark NAtomic_MUTEX
//=============================================================================
//		NAtomic::NAtomic : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
NAtomic_MUTEX::NAtomic()
	: mValue{}
{
}





//=============================================================================
//		NAtomic::NAtomic : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
NAtomic_MUTEX::NAtomic(const T& theValue)
	: mValue(theValue)
{
}





//=============================================================================
//		NAtomic::operator= : Assignment operator.
//-----------------------------------------------------------------------------
template<typename T>
T NAtomic_MUTEX::operator=(const T& theValue)
{
	NScopedLock acquireLock(mLock);

	mValue = theValue;
	return theValue;
}





//=============================================================================
//		NAtomic::is_lock_free : Is access lock-free?
//-----------------------------------------------------------------------------
template<typename T>
bool NAtomic_MUTEX::is_lock_free() const
{
	return false;
}





//=============================================================================
//		NAtomic::store : Store a value.
//-----------------------------------------------------------------------------
template<typename T>
void NAtomic_MUTEX::store(const T& newValue, std::memory_order memOrder)
{
	NScopedLock acquireLock(mLock);
	NN_UNUSED(memOrder);

	mValue = newValue;
}





//=============================================================================
//		NAtomic::load : Load the value.
//-----------------------------------------------------------------------------
template<typename T>
T NAtomic_MUTEX::load(std::memory_order memOrder) const
{
	NScopedLock acquireLock(mLock);
	NN_UNUSED(memOrder);

	return mValue;
}





//=============================================================================
//		NAtomic::operator T : Cast operator.
//-----------------------------------------------------------------------------
template<typename T>
NAtomic_MUTEX::operator T() const
{
	return load();
}





//=============================================================================
//		NAtomic::exchange : Exchange the current value.
//-----------------------------------------------------------------------------
template<typename T>
T NAtomic_MUTEX::exchange(T newValue, std::memory_order memOrder)
{
	NScopedLock acquireLock(mLock);
	NN_UNUSED(memOrder);

	std::swap(mValue, newValue);
	return newValue;
}





//=============================================================================
//		NAtomic::compare_exchange_weak : Compare and exchange.
//-----------------------------------------------------------------------------
template<typename T>
bool NAtomic_MUTEX::compare_exchange_weak(T&                valueExpected,
										  const T&          valueDesired,
										  std::memory_order memSuccess,
										  std::memory_order memFailure)
{
	return compare_exchange_strong(valueExpected, valueDesired, memSuccess, memFailure);
}





//=============================================================================
//		NAtomic::compare_exchange_weak : Compare and exchange.
//-----------------------------------------------------------------------------
template<typename T>
bool NAtomic_MUTEX::compare_exchange_weak(T&                valueExpected,
										  const T&          valueDesired,
										  std::memory_order memOrder)
{
	return compare_exchange_strong(valueExpected, valueDesired, memOrder, memOrder);
}





//=============================================================================
//		NAtomic::compare_exchange_strong : Compare and exchange.
//-----------------------------------------------------------------------------
template<typename T>
bool NAtomic_MUTEX::compare_exchange_strong(T&       valueExpected,
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





//=============================================================================
//		NAtomic::compare_exchange_strong : Compare and exchange.
//-----------------------------------------------------------------------------
template<typename T>
bool NAtomic_MUTEX::compare_exchange_strong(T&                valueExpected,
											const T&          valueDesired,
											std::memory_order memOrder)
{
	return compare_exchange_strong(valueExpected, valueDesired, memOrder, memOrder);
}





//=============================================================================
//		NAtomic::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
template<typename T>
bool NAtomic_MUTEX::CompareEqual(const T& otherValue) const
{
	return mValue == otherValue;
}





//=============================================================================
//		NAtomic::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
template<typename T>
NComparison NAtomic_MUTEX::CompareOrder(const T& otherValue) const
{
	return NCompare(mValue, otherValue);
}
