/*	NAME:
		NProgressable.h

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
#ifndef NPROGRESSABLE_H
#define NPROGRESSABLE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NTime.h"
#include "NanoConstants.h"

// System
#include <functional>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Progress values
inline constexpr float kNProgressNone                       = 0.0f;
inline constexpr float kNProgressDone                       = 1.0f;
inline constexpr float kNProgressUnknown                    = -1.0f;


// Progress state
enum class NProgress
{
	Begin,
	Update,
	End
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Progress function
//
// A progress operation reports the begin and end states once per operation,
// and the update state zero or more times.
//
// Progress values range from 0.0 to 1.0 inclusive, or -1.0 if the value is
// currently indeterminate.
using NFunctionProgress                                     = std::function<NStatus(NProgress theState, float theValue)>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NProgressable
{
public:
										NProgressable();


	// Get/set the progress function
	NFunctionProgress                   GetProgressFunction() const;
	void                                SetProgressFunction(  const NFunctionProgress& theFunction);


	// Get/set the update interval
	//
	// Progress updates will occur at most once per interval.
	//
	// The begin/end update, or updates that change to/from indeterminate
	// progress, will always be dispatched.
	NInterval                           GetProgressInterval() const;
	void                                SetProgressInterval(NInterval theInterval);


	// Get/set the current task
	//
	// A progress operation defaults to a single task.
	//
	// An operation that is composed of multiple tasks can set the current
	// and total number of tasks contained within the operation.
	//
	// Each sub-task should report progress through the Begin/End/Update API
	// as normal, however the progress function will receive progress for the
	// overall operation as if it was a single task.
	void                                GetProgressTasks(size_t& taskIndex, size_t& numTasks) const;
	void                                SetProgressTasks(size_t  taskIndex, size_t  numTasks);


	// Report progress
	NStatus                             BeginProgress( float theValue = kNProgressNone);
	NStatus                             EndProgress(   float theValue = kNProgressDone);
	NStatus                             UpdateProgress(float theValue);
	NStatus                             UpdateProgress(size_t currentValue, size_t maxValue);


private:
	NStatus                             ContinueProgress(NProgress theState, float theValue);


private:
	bool                                mIsActive;
	NFunctionProgress                   mFunction;
	NInterval                           mInterval;
	float                               mTaskIndex;
	float                               mTaskCount;

	NTime                               mLastTime;
	float                               mLastValue;
};



#endif // NPROGRESSABLE_H
