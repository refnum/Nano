/*	NAME:
		NDeque.h

	DESCRIPTION:
		Deque object.

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
#ifndef NDEQUE_H
#define NDEQUE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinContainer.h"
#include "NStdUtility.h"
#include "NanoMacros.h"

// System
#include <atomic>
#include <memory>
#include <optional>
#include <type_traits>
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
template<typename T>
class NDequeRingBuffer;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<typename T>
class NN_EMPTY_BASE NDeque : public NMixinContainer<NDeque<T>>
{
public:
										NDeque();
									   ~NDeque();

										NDeque(   const NDeque& otherDeque) = delete;
	NDeque&                             operator=(const NDeque& otherDeque) = delete;

										NDeque(   NDeque&& otherDeque) = delete;
	NDeque&                             operator=(NDeque&& otherDeque) = delete;


	// Get the size
	size_t                              GetSize() const;


	// Purge the queue
	//
	// Releases any obsolete allocations made by the queue.
	//
	// Only a single thread may purge the queue. In addition, any threads
	// that may have removed items from the queue in the past must have
	// completed any processing of those items.
	void                                Purge();


	// Emplace to the front of the deque
	//
	// Only a single thread may add items to the front of the queue.
	template<typename... Args>
	void                                EmplaceFront(Args&&... theArgs);


	// Pop from the front of the queue
	//
	// Only a single thread may remove items from the front of the queue.
	bool                                PopFront();


	// Pop from the back of the queue
	//
	// Returns std::nullopt if an item could not be removed.
	//
	// This may occur if the queue is empty, or if another thread
	// removed an item at the same time.
	//
	// Multiple threads may remove items from the back of the queue.
	std::optional<T>                    PopBack();


private:
	static constexpr bool mIsAllocFree = nstd::is_alloc_free_v<T>;

	using NDequeRingBufferT = NDequeRingBuffer<std::conditional_t<mIsAllocFree, T, T*>>;

	std::atomic_size_t                  mTop;
	std::atomic_size_t                  mBottom;
	std::atomic<NDequeRingBufferT*>     mBuffer;
	std::vector<std::unique_ptr<NDequeRingBufferT>> mPurgeable;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NDeque.inl"



#endif // NDEQUE_H
