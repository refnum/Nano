/*	NAME:
		NThreadID.inl

	DESCRIPTION:
		Thread ID.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
// System
#if NN_TARGET_WINDOWS
	#include <Windows.h>
#else
	#include <pthread.h>
#endif // NN_TARGET_WINDOWS





//=============================================================================
//		NThreadID::NThreadID : Constructor.
//-----------------------------------------------------------------------------
inline NThreadID::NThreadID()
	: mValue(0)
{
}





//=============================================================================
//		NThreadID::IsValid : Is the thread ID valid?
//-----------------------------------------------------------------------------
inline bool NThreadID::IsValid() const
{


	// Check the ID
	return mValue != 0;
}





//=============================================================================
//		NThreadID::GetValue : Get the thread ID value.
//-----------------------------------------------------------------------------
inline uintptr_t NThreadID::GetValue() const
{


	// Get the value
	return mValue;
}





//=============================================================================
//		NThreadID::Get : Get the current thread's ID.
//-----------------------------------------------------------------------------
inline NThreadID NThreadID::Get()
{


	// Get the ID
	//
	// A std::thread::id is deliberately opaque and must be pushed through
	// std::hash to obtain an integer representation.
	//
	// This takes about a dozen instructions, which we can avoid by using
	// the native thread identifier directly.
	NThreadID theID;

#if NN_TARGET_WINDOWS
	static_assert(sizeof(uintptr_t) >= sizeof(DWORD));
	theID.mValue = uintptr_t(GetCurrentThreadId());

#else
	static_assert(sizeof(uintptr_t) >= sizeof(pthread_t));
	theID.mValue = uintptr_t(pthread_self());
#endif

	return theID;
}





#pragma mark NMixinComparable
//=============================================================================
//		NThreadID::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
inline bool NThreadID::CompareEqual(const NThreadID& theID) const
{


	// Compare the ID
	return mValue == theID.mValue;
}





//=============================================================================
//		NThreadID::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
inline NComparison NThreadID::CompareOrder(const NThreadID& theID) const
{


	// Compare the ID
	return NCompare(mValue, theID.mValue);
}
