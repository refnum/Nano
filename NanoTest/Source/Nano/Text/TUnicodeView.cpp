/*	NAME:
		TUnicodeView.cpp

	DESCRIPTION:
		NUnicodeView tests.

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
#include "NTestFixture.h"
#include "NUnicodeView.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const utf32_t*   kStringUTF32                        = U"Hello World! \x1F600";
static constexpr size_t kStringSize                         = 14;

static const uint8_t kTestUTF8[]                            = {0x48,
									0x65,
									0x6c,
									0x6c,
									0x6f,
									0x20,
									0x57,
									0x6f,
									0x72,
									0x6c,
									0x64,
									0x21,
									0x20,
									0xf0,
									0x9f,
									0x98,
									0x80};
;
static const uint16_t kTestUTF16[]                          = {0x0048,
									  0x0065,
									  0x006c,
									  0x006c,
									  0x006f,
									  0x0020,
									  0x0057,
									  0x006f,
									  0x0072,
									  0x006c,
									  0x0064,
									  0x0021,
									  0x0020,
									  0xd83d,
									  0xde00};
;
static const uint32_t kTestUTF32[]                          = {0x00000048,
									  0x00000065,
									  0x0000006c,
									  0x0000006c,
									  0x0000006f,
									  0x00000020,
									  0x00000057,
									  0x0000006f,
									  0x00000072,
									  0x0000006c,
									  0x00000064,
									  0x00000021,
									  0x00000020,
									  0x0001f600};





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(NUnicodeView){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NUnicodeView, "UTF8/Iterate")
{


	// Perform the test
	NUnicodeView theView(NStringEncoding::UTF8, sizeof(kTestUTF8), kTestUTF8);
	NVectorUTF32 theResult;

	for (utf32_t codePoint : theView)
	{
		theResult.push_back(codePoint);
	}

	REQUIRE(theResult.size() == kStringSize);
	REQUIRE(memcmp(&theResult[0], kStringUTF32, kStringSize * sizeof(utf32_t)) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NUnicodeView, "UTF16/Iterate")
{


	// Perform the test
	NUnicodeView theView(NStringEncoding::UTF16, sizeof(kTestUTF16), kTestUTF16);
	NVectorUTF32 theResult;

	for (utf32_t codePoint : theView)
	{
		theResult.push_back(codePoint);
	}

	REQUIRE(theResult.size() == kStringSize);
	REQUIRE(memcmp(&theResult[0], kStringUTF32, kStringSize * sizeof(utf32_t)) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NUnicodeView, "UTF32/Iterate")
{


	// Perform the test
	NUnicodeView theView(NStringEncoding::UTF32, sizeof(kTestUTF32), kTestUTF32);
	NVectorUTF32 theResult;

	for (utf32_t codePoint : theView)
	{
		theResult.push_back(codePoint);
	}

	REQUIRE(theResult.size() == kStringSize);
	REQUIRE(memcmp(&theResult[0], kStringUTF32, kStringSize * sizeof(utf32_t)) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NUnicodeView, "UTF8/Codepoint")
{


	// Perform the test
	NUnicodeView theView(NStringEncoding::UTF8, sizeof(kTestUTF8), kTestUTF8);
	size_t       byteOffset = 1 * sizeof(utf8_t);

	REQUIRE(theView.GetCodepoint(byteOffset) == 'e');
	REQUIRE(theView.GetCodepointSize(byteOffset) == sizeof(utf8_t));

	REQUIRE(theView.GetCodepoint(1000) == 0);
	REQUIRE(theView.GetSize(1000) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NUnicodeView, "UTF16/Codepoint")
{


	// Perform the test
	NUnicodeView theView(NStringEncoding::UTF16, sizeof(kTestUTF16), kTestUTF16);
	size_t       byteOffset = 1 * sizeof(utf16_t);

	REQUIRE(theView.GetCodepoint(byteOffset) == 'e');
	REQUIRE(theView.GetCodepointSize(byteOffset) == sizeof(utf16_t));

	REQUIRE(theView.GetCodepoint(1000) == 0);
	REQUIRE(theView.GetSize(1000) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NUnicodeView, "UTF32/Codepoint")
{


	// Perform the test
	NUnicodeView theView(NStringEncoding::UTF32, sizeof(kTestUTF32), kTestUTF32);
	size_t       byteOffset = 1 * sizeof(utf32_t);

	REQUIRE(theView.GetCodepoint(byteOffset) == 'e');
	REQUIRE(theView.GetCodepointSize(byteOffset) == sizeof(utf32_t));

	REQUIRE(theView.GetCodepoint(1000) == 0);
	REQUIRE(theView.GetSize(1000) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NUnicodeView, "UTF8/GetMaxSize")
{


	// Perform the test
	NUnicodeView theView(NStringEncoding::UTF8, sizeof(kTestUTF8), kTestUTF8);

	REQUIRE(theView.GetMaxSize() == (std::size(kTestUTF8) * sizeof(utf32_t)));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NUnicodeView, "UTF16/GetMaxSize")
{


	// Perform the test
	NUnicodeView theView(NStringEncoding::UTF16, sizeof(kTestUTF16), kTestUTF16);

	REQUIRE(theView.GetMaxSize() == (std::size(kTestUTF16) * sizeof(utf32_t)));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NUnicodeView, "UTF32/GetMaxSize")
{


	// Perform the test
	NUnicodeView theView(NStringEncoding::UTF32, sizeof(kTestUTF32), kTestUTF32);

	REQUIRE(theView.GetMaxSize() == (std::size(kTestUTF32) * sizeof(utf32_t)));
}
