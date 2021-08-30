/*	NAME:
		NAtomic.h

	DESCRIPTION:
		Atomic variable.

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
#ifndef NATOMIC_H
#define NATOMIC_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMutex.h"

// System
#include <atomic>





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<typename T, typename Enabled                       = void>
class NAtomic
{
public:
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
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NAtomic.inl"



#endif // NATOMIC_H
