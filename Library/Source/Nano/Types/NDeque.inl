/*	NAME:
		NDeque.inl

	DESCRIPTION:
		Deque object.

		Implements a lock-free deque as described by:

			Dynamic Circular Work-Stealing Deque
			https://www.dre.vanderbilt.edu/~schmidt/PDF/work-stealing-dequeue.pdf

			Correct and Efficient Work-Stealing for Weak Memory Models
			https://fzn.fr/readings/ppopp13.pdf

		Distinction between allocation / allocation-free types based on:

			riften::deque
			https://github.com/ConorWilliams/ConcurrentDeque.

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
#include "NMathUtils.h"





//=============================================================================
//		Internal class declaration.
//-----------------------------------------------------------------------------
template<typename T>
class NDequeRingBuffer
{
public:
										NDequeRingBuffer(size_t theCapacity);


	// Get the capacity
	size_t                              GetCapacity() const;


	// Load / store a value
	T                                   Load( size_t theIndex) const;
	void                                Store(size_t theIndex, T theValue);


	// Allocate a new buffer
	NDequeRingBuffer<T>*                Allocate(size_t indexBottom, size_t indexTop) const;


private:
	size_t                              mCapacity;
	size_t                              mModuloMask;
	std::unique_ptr<std::atomic<T>[]>   mBuffer;
};





#pragma mark NDequeRingBuffer
//=============================================================================
//		NDequeRingBuffer::NDequeRingBuffer : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
NDequeRingBuffer<T>::NDequeRingBuffer(size_t theCapacity)
	: mCapacity(  theCapacity)
	, mModuloMask(theCapacity - 1)
	, mBuffer(std::make_unique<std::atomic<T>[]>(theCapacity))
{
	// Validate our state
										NN_REQUIRE(NMathUtils::IsPowerOf2(theCapacity));
}





//=============================================================================
//		NDequeRingBuffer::GetCapacity : Get the capacity.
//-----------------------------------------------------------------------------
template<typename T>
size_t NDequeRingBuffer<T>::GetCapacity() const
{


	// Get the capacity
	return mCapacity;
}





//=============================================================================
//		NDequeRingBuffer::Load : Load an item from the buffer.
//-----------------------------------------------------------------------------
template<typename T>
T NDequeRingBuffer<T>::Load(size_t theIndex) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex >= 0);



	// Store the value
	return mBuffer[theIndex & mModuloMask].load(std::memory_order_relaxed);
}





//=============================================================================
//		NDequeRingBuffer::Store : Store an item into the buffer.
//-----------------------------------------------------------------------------
template<typename T>
void NDequeRingBuffer<T>::Store(size_t theIndex, T theValue)
{


	// Validate our parameters
	NN_REQUIRE(theIndex >= 0);



	// Store the value
	mBuffer[theIndex & mModuloMask].store(theValue, std::memory_order_relaxed);
}





//=============================================================================
//		NDequeRingBuffer::Allocate : Allocate a new buffer.
//-----------------------------------------------------------------------------
template<typename T>
NDequeRingBuffer<T>* NDequeRingBuffer<T>::Allocate(size_t indexBottom, size_t indexTop) const
{


	// Validate our parameters
	NN_REQUIRE(indexTop <= indexBottom);



	// Allocate the buffer
	//
	// Returns a new, larger, buffer containing the required
	// elements from the existing buffer.
	NDequeRingBuffer<T>* newBuffer = new NDequeRingBuffer<T>(mCapacity * 2);

	for (auto n = indexTop; n < indexBottom; n++)
	{
		newBuffer->Store(n, Load(n));
	}

	return newBuffer;
}





#pragma mark NDeque
//=============================================================================
//		NDeque::NDeque : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
NDeque<T>::NDeque()
	: mTop{1}
	, mBottom{1}
	, mBuffer(new NDequeRingBufferT(64))
	, mPurgeable{}
{
}





//=============================================================================
//		NDeque::~NDeque : Destructor.
//-----------------------------------------------------------------------------
template<typename T>
NDeque<T>::~NDeque()
{


	// Clean up
	if constexpr (!mIsAllocFree)
	{
		while (PopFront())
		{
		}
	}

	delete mBuffer.load();
}





//=============================================================================
//		NDeque::GetSize : Get the size.
//-----------------------------------------------------------------------------
template<typename T>
size_t NDeque<T>::GetSize() const
{


	// Get the size
	size_t theBottom = mBottom.load(std::memory_order_relaxed);
	size_t theTop    = mTop.load(std::memory_order_relaxed);

	if (theBottom >= theTop)
	{
		return size_t(theBottom - theTop);
	}

	return 0;
}





//=============================================================================
//		NDeque::Purge : Purge the queue.
//-----------------------------------------------------------------------------
template<typename T>
void NDeque<T>::Purge()
{


	// Purge the buffer
	mPurgeable.clear();
}





//=============================================================================
//		NDeque::EmplaceFront : Emplace to the front of the queue.
//-----------------------------------------------------------------------------
template<typename T>
template<typename... Args>
void NDeque<T>::EmplaceFront(Args&&... theArgs)
{


	// Get the state we need
	size_t theBottom = mBottom.load(std::memory_order_relaxed);
	size_t theTop    = mTop.load(std::memory_order_acquire);
	size_t theSize   = theBottom - theTop;



	// Get the buffer
	//
	// Once the buffer becomes full we create a new, larger, buffer
	// and move the existing buffer onto the purgeable list.
	NDequeRingBufferT* theBuffer = mBuffer.load(std::memory_order_relaxed);

	if (theSize >= theBuffer->GetCapacity() - 1)
	{
		auto newBuffer = theBuffer->Allocate(theBottom, theTop);
		mPurgeable.emplace_back(std::exchange(theBuffer, newBuffer));

		mBuffer.store(theBuffer, std::memory_order_relaxed);
	}



	// Emplace the object
	if constexpr (mIsAllocFree)
	{
		theBuffer->Store(theBottom, T{std::forward<Args>(theArgs)...});
	}
	else
	{
		theBuffer->Store(theBottom, new T{std::forward<Args>(theArgs)...});
	}

	std::atomic_thread_fence(std::memory_order_release);
	mBottom.store(theBottom + 1, std::memory_order_relaxed);
}





//=============================================================================
//		NDeque::PopFront : Pop from the front of the queue.
//-----------------------------------------------------------------------------
template<typename T>
bool NDeque<T>::PopFront()
{


	// Get the state we need
	size_t             theBottom = mBottom.load(std::memory_order_relaxed) - 1;
	NDequeRingBufferT* theBuffer = mBuffer.load(std::memory_order_relaxed);
	mBottom.store(theBottom, std::memory_order_relaxed);

	std::atomic_thread_fence(std::memory_order_seq_cst);
	size_t theTop = mTop.load(std::memory_order_relaxed);



	// Non-empty queue
	//
	// Remove the item at the bottom of the queue.
	if (theTop <= theBottom)
	{
		// Single item in queue
		//
		// If there is a single item in the queue we attempt to increment top.
		//
		// If we succeed then the queue is now empty so we can restore the
		// original state and continue to remove the item.
		//
		// If we fail to increment top then we know the last item has just been
		// removed by PopBack so we restore the original state and return.
		if (theTop == theBottom)
		{
			if (!mTop.compare_exchange_strong(theTop,
											  theTop + 1,
											  std::memory_order_seq_cst,
											  std::memory_order_relaxed))
			{
				mBottom.store(theBottom + 1, std::memory_order_relaxed);
				return false;
			}

			mBottom.store(theBottom + 1, std::memory_order_relaxed);
		}



		// Remove the item
		auto theItem = theBuffer->Load(theBottom);

		if constexpr (!mIsAllocFree)
		{
			delete theItem;
		}

		return true;
	}


	// Empty queue
	//
	// An empty queue is returned to the original state.
	else
	{
		mBottom.store(theBottom + 1, std::memory_order_relaxed);
		return false;
	}
}





//=============================================================================
//		NDeque::PopBack : Pop from the back of the queue.
//-----------------------------------------------------------------------------
template<typename T>
std::optional<T> NDeque<T>::PopBack()
{


	// Get the state we need
	size_t theTop = mTop.load(std::memory_order_acquire);

	std::atomic_thread_fence(std::memory_order_seq_cst);
	size_t theBottom = mBottom.load(std::memory_order_acquire);



	// Non-empty queue
	//
	// Remove the item at the top of the queue.
	std::optional<T> theResult;

	if (theTop < theBottom)
	{
		// Fetch the item
		//
		// We must load the result value before incrementing top as the top
		// may change again as soon as we increment it.
		//
		// If we fail to increment top then we know another thread has just
		// popped the top so we return an empty result.
		NDequeRingBufferT* theBuffer = mBuffer.load(std::memory_order_consume);
		auto               theItem   = theBuffer->Load(theTop);

		if (mTop.compare_exchange_strong(theTop,
										 theTop + 1,
										 std::memory_order_seq_cst,
										 std::memory_order_relaxed))
		{
			// Move the item to the result
			if constexpr (mIsAllocFree)
			{
				theResult = theItem;
			}
			else
			{
				theResult = std::move(*theItem);
				delete theItem;
			}
		}
	}

	return theResult;
}
