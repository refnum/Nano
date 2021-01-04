/*	NAME:
		TDebug.cpp

	DESCRIPTION:
		NDebug tests.

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
#include "NDebug.h"
#include "NTestFixture.h"





//=============================================================================
//		Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TDebug){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDebug, "Default")
{


	// Perform the test
	NN_LOG_INFO("Testing NN_LOG_INFO    : no params");
	NN_LOG_WARNING("Testing NN_LOG_WARNING : no params");
	NN_LOG_ERROR("Testing NN_LOG_ERROR   : no params");

	NN_LOG_INFO("Testing NN_LOG_INFO    : %s params (%d)", "printf", 1);
	NN_LOG_WARNING("Testing NN_LOG_WARNING : %s params (%d)", "printf", 2);
	NN_LOG_ERROR("Testing NN_LOG_ERROR   : %s params (%d)", "printf", 3);

	NN_LOG_INFO("Testing NN_LOG_INFO    : {} params ({})", "std::format", 1);
	NN_LOG_WARNING("Testing NN_LOG_WARNING : {} params ({})", "std::format", 2);
	NN_LOG_ERROR("Testing NN_LOG_ERROR   : {} params ({})", "std::format", 3);
}
