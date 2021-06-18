/*	NAME:
		TUTI.cpp

	DESCRIPTION:
		NUTI tests.

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
#include "NFile.h"
#include "NFormat.h"
#include "NTestFixture.h"
#include "NUTI.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TUTI)
{
	NUTI theUTI;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTI, "Default")
{


	// Perform the test
	REQUIRE(!theUTI.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTI, "ConformsTo")
{


	// Perform the test
	theUTI = NUTI(NUTITag::Extension, "jpg");

	REQUIRE(theUTI.ConformsTo(kNUTTypeJPEG));
	REQUIRE(theUTI.ConformsTo(kNUTTypeImage));
	REQUIRE(theUTI.ConformsTo(kNUTTypeData));
	REQUIRE(theUTI.ConformsTo(kNUTTypeContent));

	REQUIRE(!theUTI.ConformsTo(kNUTTypePNG));
	REQUIRE(!theUTI.ConformsTo(kNUTTypeText));
	REQUIRE(!theUTI.ConformsTo(kNUTTypeDiskImage));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTI, "Clear")
{


	// Perform the test
	theUTI = NUTI(NUTITag::Extension, "jpg");
	REQUIRE(theUTI.IsValid());

	theUTI.Clear();
	REQUIRE(!theUTI.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTI, "GetDescription")
{


	// Perform the test
	theUTI = NUTI(NUTITag::Extension, "jpg");

	REQUIRE(!theUTI.GetDescription().IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTI, "GetTag")
{


	// Perform the test
	theUTI = NUTI(NUTITag::Extension, "jpg");

	REQUIRE(theUTI.GetTag(NUTITag::MIMEType) == "image/jpeg");
	REQUIRE(theUTI.GetTag(NUTITag::Extension) == "jpg");

	REQUIRE(theUTI.GetTag(NUTITag::MIMEType) == theUTI.GetMIMEType());
	REQUIRE(theUTI.GetTag(NUTITag::Extension) == theUTI.GetExtension());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTI, "GetMIMEType")
{


	// Perform the test
	theUTI = NUTI(NUTITag::Extension, "jpg");

	REQUIRE(theUTI.GetMIMEType() == "image/jpeg");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTI, "GetExtension")
{


	// Perform the test
	theUTI = NUTI(NUTITag::MIMEType, "image/jpeg");

	REQUIRE(theUTI.GetExtension() == "jpg");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTI, "GetType")
{


	// Perform the test
	theUTI = NUTI(NUTITag::Extension, "jpg");

	REQUIRE(theUTI.GetType() == kNUTTypeJPEG);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTI, "SetType")
{


	// Perform the test
	theUTI.SetType(kNUTTypeJPEG);
	REQUIRE(theUTI.GetType() != kNUTTypePNG);

	theUTI.SetType(kNUTTypePNG);
	REQUIRE(theUTI.GetType() == kNUTTypePNG);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTI, "CompareEqual")
{


	// Perform the test
	NUTI utiA(kNUTTypeJPEG);
	NUTI utiB(kNUTTypePNG);

	REQUIRE(utiA == utiA);
	REQUIRE(utiB == utiB);
	REQUIRE(utiA != utiB);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTI, "CompareOrder")
{


	// Perform the test
	NUTI utiA(kNUTTypeJPEG);
	NUTI utiB(kNUTTypePNG);

	REQUIRE(utiA <= utiA);
	REQUIRE(utiB <= utiB);
	REQUIRE(utiA < utiB);
	REQUIRE(utiB > utiA);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTI, "Format")
{


	// Perform the test
	REQUIRE(NFormat("{}", kNUTTypeJPEG) == "public.jpeg");
}
