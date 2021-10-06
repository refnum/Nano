/*	NAME:
		TCommandLine.cpp

	DESCRIPTION:
		NCommandLine tests.

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
#include "NCommandLine.h"
#include "NFile.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const int   kTestArgC                                = 10;
static const char* kTestArgV[kTestArgC]                     = {"app",
										   "-arg1",
										   "-arg2=true",
										   "-arg3=-2",
										   "--arg4=3.0",
										   "--arg5=/tmp/test",
										   "--arg6=\"s t r i n g\"",
										   "--arg7=s t r i n g",
										   "apple",
										   "banana"};





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TCommandLine){SETUP{


	// Set the arguments
	NCommandLine::SetArguments(kTestArgC, kTestArgV);
}
}
;





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCommandLine, "HasFlag")
{


	// Perform the test
	REQUIRE(NCommandLine::HasFlag("arg1"));
	REQUIRE(NCommandLine::HasFlag("arg2"));
	REQUIRE(NCommandLine::HasFlag("arg3"));
	REQUIRE(NCommandLine::HasFlag("arg4"));
	REQUIRE(NCommandLine::HasFlag("arg5"));
	REQUIRE(NCommandLine::HasFlag("arg6"));
	REQUIRE(NCommandLine::HasFlag("arg7"));
	REQUIRE(!NCommandLine::HasFlag("arg99"));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCommandLine, "GetBool")
{


	// Perform the test
	REQUIRE(!NCommandLine::GetBool("arg1"));
	REQUIRE(NCommandLine::GetBool("arg2"));
	REQUIRE(!NCommandLine::GetBool("arg3"));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCommandLine, "GetInt64")
{


	// Perform the test
	REQUIRE(NCommandLine::GetInt64("arg1") == 0);
	REQUIRE(NCommandLine::GetInt64("arg3") == -2);
	REQUIRE(NCommandLine::GetInt64("arg4") == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCommandLine, "GetFloat64")
{


	// Perform the test
	REQUIRE(NCommandLine::GetFloat64("arg1") == 0);
	REQUIRE(NCommandLine::GetFloat64("arg3") == -2.0);
	REQUIRE(NCommandLine::GetFloat64("arg4") == 3.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCommandLine, "GetFile")
{


	// Perform the test
	REQUIRE(!NCommandLine::GetFile("arg1").IsValid());
	REQUIRE(NCommandLine::GetFile("arg5").IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCommandLine, "GetString")
{


	// Perform the test
	REQUIRE(NCommandLine::GetString("arg1").IsEmpty());
	REQUIRE(NCommandLine::GetString("arg2") == "true");
	REQUIRE(NCommandLine::GetString("arg3") == "-2");
	REQUIRE(NCommandLine::GetString("arg4") == "3.0");
	REQUIRE(NCommandLine::GetString("arg5") == "/tmp/test");
	REQUIRE(NCommandLine::GetString("arg6") == "s t r i n g");
	REQUIRE(NCommandLine::GetString("arg7") == "s t r i n g");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCommandLine, "GetArguments")
{


	// Perform the test
	NVectorString theArgs;

	theArgs = NCommandLine::GetArguments(NArguments::All);
	REQUIRE(theArgs.size() == 10);
	REQUIRE(theArgs[0] == kTestArgV[0]);
	REQUIRE(theArgs[1] == kTestArgV[1]);
	REQUIRE(theArgs[2] == kTestArgV[2]);
	REQUIRE(theArgs[3] == kTestArgV[3]);
	REQUIRE(theArgs[4] == kTestArgV[4]);
	REQUIRE(theArgs[5] == kTestArgV[5]);
	REQUIRE(theArgs[6] == kTestArgV[6]);
	REQUIRE(theArgs[7] == kTestArgV[7]);
	REQUIRE(theArgs[8] == kTestArgV[8]);
	REQUIRE(theArgs[9] == kTestArgV[9]);

	theArgs = NCommandLine::GetArguments(NArguments::Named);
	REQUIRE(theArgs.size() == 7);
	REQUIRE(theArgs[0] == kTestArgV[1]);
	REQUIRE(theArgs[1] == kTestArgV[2]);
	REQUIRE(theArgs[2] == kTestArgV[3]);
	REQUIRE(theArgs[3] == kTestArgV[4]);
	REQUIRE(theArgs[4] == kTestArgV[5]);
	REQUIRE(theArgs[5] == kTestArgV[6]);
	REQUIRE(theArgs[6] == kTestArgV[7]);

	theArgs = NCommandLine::GetArguments(NArguments::Unnamed);
	REQUIRE(theArgs.size() == 3);
	REQUIRE(theArgs[0] == kTestArgV[0]);
	REQUIRE(theArgs[1] == kTestArgV[8]);
	REQUIRE(theArgs[2] == kTestArgV[9]);

	REQUIRE(NCommandLine::GetArguments() == NCommandLine::GetArguments(NArguments::All));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCommandLine, "SetArguments/NVectorString")
{


	// Perform the test
	NVectorString theArgs{"a", "b", "c"};
	REQUIRE(NCommandLine::GetArguments() != theArgs);

	NCommandLine::SetArguments(theArgs);
	REQUIRE(NCommandLine::GetArguments() == theArgs);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCommandLine, "SetArguments/argc")
{


	// Perform the test
	const char* theArgs[] = {"a", "b", "c"};

	REQUIRE(NCommandLine::GetArguments().size() != std::size(theArgs));
	NCommandLine::SetArguments(int(std::size(theArgs)), theArgs);

	NVectorString stringArgs = NCommandLine::GetArguments();
	REQUIRE(stringArgs.size() == std::size(theArgs));
	REQUIRE(stringArgs[0] == theArgs[0]);
	REQUIRE(stringArgs[1] == theArgs[1]);
	REQUIRE(stringArgs[2] == theArgs[2]);
}
