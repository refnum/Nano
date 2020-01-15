/*	NAME:
		TStringEncoder.cpp

	DESCRIPTION:
		NStringEncoder tests.

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
// Nano
#include "NData.h"
#include "NStringEncoder.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const uint8_t kTestUTF8[] =
	{0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x00};





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TStringEncoder){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "Conversion")
{


	// Perform the test
	NData          dataIn(std::size(kTestUTF8), kTestUTF8);
	NData          dataOut;
	NStringEncoder theEncoder;


	theEncoder.Convert(NStringEncoding::UTF8, dataIn, NStringEncoding::UTF16, dataOut);
	REQUIRE(!dataOut.IsEmpty());

	theEncoder.Convert(NStringEncoding::UTF16, dataOut, NStringEncoding::UTF32, dataOut);
	REQUIRE(!dataOut.IsEmpty());

	theEncoder.Convert(NStringEncoding::UTF32, dataOut, NStringEncoding::ASCII, dataOut);
	REQUIRE(!dataOut.IsEmpty());

	theEncoder.Convert(NStringEncoding::ASCII, dataOut, NStringEncoding::MacRoman, dataOut);
	REQUIRE(!dataOut.IsEmpty());

	theEncoder.Convert(NStringEncoding::MacRoman, dataOut, NStringEncoding::ISOLatin1, dataOut);
	REQUIRE(!dataOut.IsEmpty());

	theEncoder.Convert(NStringEncoding::ISOLatin1,
					   dataOut,
					   NStringEncoding::WindowsLatin1,
					   dataOut);
	REQUIRE(!dataOut.IsEmpty());

	theEncoder.Convert(NStringEncoding::WindowsLatin1, dataOut, NStringEncoding::UTF8, dataOut);
	REQUIRE(!dataOut.IsEmpty());

	REQUIRE(dataOut == dataIn);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "Native")
{


	// Perform the test
	REQUIRE(NStringEncoder::GetNativeEncoding(NStringEncoding::UTF16BE) == NStringEncoding::UTF16);
	REQUIRE(NStringEncoder::GetNativeEncoding(NStringEncoding::UTF16LE) == NStringEncoding::UTF16);

	REQUIRE(NStringEncoder::GetNativeEncoding(NStringEncoding::UTF32BE) == NStringEncoding::UTF32);
	REQUIRE(NStringEncoder::GetNativeEncoding(NStringEncoding::UTF32LE) == NStringEncoding::UTF32);
}
