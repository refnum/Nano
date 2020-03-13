/*	NAME:
		NClock.h

	DESCRIPTION:
		Clock object.

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
#ifndef NCLOCK_H
#define NCLOCK_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoTypes.h"





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NClock
{
public:
										NClock();


	// Is the clock running?
	bool                                IsRunning() const;


	// Start the clock
	//
	// Clocks are started on construction.
	//
	// Restarting a clock will reset its start time.
	void                                Start();


	// Stop the clock
	//
	// Returns the elapsed time.
	NInterval                           Stop();


	// Get the elapsed time
	//
	// Returns the time since the clock was started or, if the clock
	// has been stopped, the final elapsed time when it was stopped.
	NInterval                           GetTime() const;


private:
	NInterval                           GetTime(uint64_t startTicks, uint64_t endTicks) const;


private:
	uint64_t                            mStart;
	uint64_t                            mEnd;
};



#endif // NCLOCK_H
