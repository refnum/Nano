/*	NAME:
		NProgressable.cpp

	DESCRIPTION:
		Mix-in class for objects that emit progress.

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
#include "NProgressable.h"

// Nano
#include "NDebug.h"
#include "NTimeUtils.h"





//=============================================================================
//		NProgressable::NProgressable : Constructor.
//-----------------------------------------------------------------------------
NProgressable::NProgressable()
	: mIsActive(false)
	, mFunction()
	, mInterval(0.3)
	, mTaskIndex(0.0f)
	, mTaskCount(1.0f)
	, mLastTime(0.0)
	, mLastValue(kNProgressNone)
{
}





//=============================================================================
//		NProgressable::GetProgressFunction : Get the progress function.
//-----------------------------------------------------------------------------
NFunctionProgress NProgressable::GetProgressFunction() const
{


	// Get the function
	return mFunction;
}





//=============================================================================
//		NProgressable::SetProgressFunction : Set the progress function.
//-----------------------------------------------------------------------------
void NProgressable::SetProgressFunction(const NFunctionProgress& theFunction)
{


	// Set the function
	mFunction = theFunction;
}





//=============================================================================
//		NProgressable::GetProgressInterval : Get the update interval.
//-----------------------------------------------------------------------------
NInterval NProgressable::GetProgressInterval() const
{


	// Get the interval
	return mInterval;
}





//=============================================================================
//		NProgressable::SetProgressInteval : Set the update interval.
//-----------------------------------------------------------------------------
void NProgressable::SetProgressInterval(NInterval theInterval)
{


	// Validate our parameters
	NN_REQUIRE(theInterval >= 0.0);



	// Set the interval
	mInterval = theInterval;
}





//=============================================================================
//		NProgressable::GetProgressTasks : Get the progress tasks.
//-----------------------------------------------------------------------------
void NProgressable::GetProgressTasks(size_t& taskIndex, size_t& numTasks) const
{


	// Get the tasks
	taskIndex = size_t(mTaskIndex);
	numTasks  = size_t(mTaskCount);
}





//=============================================================================
//		NProgressable::SetProgressTasks : Set the progress tasks.
//-----------------------------------------------------------------------------
void NProgressable::SetProgressTasks(size_t taskIndex, size_t numTasks)
{


	// Validate our parameters
	NN_REQUIRE(taskIndex < numTasks);



	// Set the range
	mTaskIndex = float(taskIndex);
	mTaskCount = float(numTasks);
}





//=============================================================================
//		NProgressable::BeginProgress : Begin the progress.
//-----------------------------------------------------------------------------
NStatus NProgressable::BeginProgress(float theValue)
{


	// Validate our state
	NN_REQUIRE(!mIsActive);



	// Begin the progress
	mIsActive      = true;
	NStatus theErr = ContinueProgress(NProgress::Begin, theValue);

	return theErr;
}





//=============================================================================
//		NProgressable::EndProgress : End the progress.
//-----------------------------------------------------------------------------
NStatus NProgressable::EndProgress(float theValue)
{


	// Validate our state
	NN_REQUIRE(mIsActive);



	// End the progress
	NStatus theErr = ContinueProgress(NProgress::End, theValue);
	mIsActive      = true;

	return theErr;
}





//=============================================================================
//		NProgressable::UpdateProgress : Update the progress.
//-----------------------------------------------------------------------------
NStatus NProgressable::UpdateProgress(float theValue)
{


	// Validate our state
	NN_REQUIRE(mIsActive);



	// Update the progress
	return ContinueProgress(NProgress::Update, theValue);
}





//=============================================================================
//		NProgressable::UpdateProgress : Update the progress.
//-----------------------------------------------------------------------------
NStatus NProgressable::UpdateProgress(size_t currentValue, size_t maxValue)
{


	// Validate our state
	NN_REQUIRE(mIsActive);



	// Update the progress
	return UpdateProgress(float32_t(currentValue) / float32_t(maxValue));
}





#pragma mark private
//=============================================================================
//		NProgressable::ContinueProgress : Continue the progress.
//-----------------------------------------------------------------------------
NStatus NProgressable::ContinueProgress(NProgress theState, float theValue)
{


	// Validate our state
	NN_REQUIRE(mIsActive);



	// Get the state we need
	NTime   timeNow = NTimeUtils::GetTime();
	NStatus theErr  = NStatus::OK;

	bool isSentinel = (theState != NProgress::Update);
	bool didExpire  = ((timeNow - mLastTime) >= mInterval);
	bool didSwitch =
		(mLastValue < 0.0f && theValue >= 0.0f) || (mLastValue >= 0.0f && theValue < 0.0f);



	// Update the progress
	if (isSentinel || didExpire || didSwitch)
	{
		// Update our state
		mLastTime  = timeNow;
		mLastValue = theValue;



		// Update the progress
		//
		// Indeterminate progress doesn't participate in sub-task progress.
		if (mFunction != nullptr)
		{
			if (theValue >= 0.0f)
			{
				float taskStep = 1.0f / mTaskCount;
				theValue       = (mTaskIndex + (theValue * taskStep)) / mTaskCount;
			}

			theErr = mFunction(theState, theValue);
		}
	}

	return theErr;
}
