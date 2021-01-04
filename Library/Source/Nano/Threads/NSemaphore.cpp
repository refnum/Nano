/*	NAME:
		NSemaphore.cpp

	DESCRIPTION:
		Semaphore object.

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
#include "NSemaphore.h"

// Nano
#include "NDebug.h"





//=============================================================================
//		NSemaphore::NSemaphore : Constructor.
//-----------------------------------------------------------------------------
NSemaphore::NSemaphore(size_t theValue)
	: mSemaphore(Create(theValue))
{
}





//=============================================================================
//		NSemaphore::~NSemaphore : Destructor.
//-----------------------------------------------------------------------------
NSemaphore::~NSemaphore()
{


	// Clean up
	if (mSemaphore != kNSemaphoreNone)
	{
		Destroy(mSemaphore);
	}
}





//=============================================================================
//		NSemaphore::NSemaphore : Constructor.
//-----------------------------------------------------------------------------
NSemaphore::NSemaphore(NSemaphore&& theSemaphore)
	: mSemaphore(kNSemaphoreNone)
{


	// Initialise ourselves
	std::swap(mSemaphore, theSemaphore.mSemaphore);
}





//=============================================================================
//		NSemaphore::operator = : Move operator.
//-----------------------------------------------------------------------------
NSemaphore& NSemaphore::operator=(NSemaphore&& theSemaphore)
{


	// Move the semaphore
	std::swap(mSemaphore, theSemaphore.mSemaphore);

	return *this;
}





//=============================================================================
//		NSemaphore::Wait : Wait for the semaphore.
//-----------------------------------------------------------------------------
bool NSemaphore::Wait(NInterval waitFor)
{


	// Validate our parameters
	NN_REQUIRE(waitFor >= 0.0 || waitFor == kNTimeForever);



	// Wait for the semaphore
	return Wait(mSemaphore, waitFor);
}





//=============================================================================
//		NSemaphore::Signal : Signal the semaphore.
//-----------------------------------------------------------------------------
void NSemaphore::Signal(size_t numSignals)
{


	// Signal the semaphore
	for (size_t n = 0; n < numSignals; n++)
	{
		Signal(mSemaphore);
	}
}
