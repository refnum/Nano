/*	NAME:
		TFileScanner.cpp

	DESCRIPTION:
		TFileScanner tests.

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
#include "NFileScanner.h"
#include "NTestFixture.h"



#if 0
//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Paths
	#if NN_TARGET_MAC
static const NString kTestParent                            = "/tmp/TFileIterator";

	#elif NN_TARGET_WINDOWS
static const NString kTestParent                            = "c:\\windows\\temp\\TFileIterator";

	#elif NN_TARGET_LINUX
static const NString kTestParent                            = "/tmp/TFileIterator";

	#else
UNKNOWN TARGET
	#endif

static const NString kTestChild                             = kTestParent + NN_DIR + "Child1" + NN_DIR + "Child2" + NN_DIR + "Child3";

#endif





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFileScanner)
{
	NFileScanner fileScanner;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileScanner, "Defailt")
{
}
