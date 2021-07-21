/*	NAME:
		NThreadGroup.h

	DESCRIPTION:
		Thread pool group.

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
#ifndef NTHREADTASK_HDR
#define NTHREADTASK_HDR
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NFunction.h"
#include "NSemaphore.h"

// System
#include <atomic>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NThreadPool;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NThreadGroup
{
public:
										NThreadGroup();


	// Add a function to the group
	//
	// The function is executed by the specified thread pool,
	// or the main thread pool if no pool is provided.
	void                                Add(const NFunction& theFunction, NThreadPool* thePool = nullptr);


	// Add functions to the group
	//
	// The function is invoked for each item in the container.
	//
	// The container must not be modified while the tasks are
	// executing.
	//
	// The function is executed by the specified thread pool,
	// or the main thread pool if no pool is provided.
	template<typename Container, typename Function>
	void                                AddEach(const Container& theContainer,
												const Function&  theFunction,
												NThreadPool*     thePool = nullptr);


	// Wait for the group
	//
	// Waits for the tasks in this group to complete.
	void                                WaitForCompletion();


private:
	std::atomic_size_t                  mCount;
	NSemaphore                          mSemaphore;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NThreadGroup.inl"



#endif // NThreadTask_HDR
