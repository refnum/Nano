/*	NAME:
		TUTIRegistry.cpp

	DESCRIPTION:
		NUTIRegistry tests.

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
#include "NTestFixture.h"
#include "NUTIRegistry.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TUTIRegistry)
{
	NUTIRegistry* utiRegistry;

	SETUP
	{
		utiRegistry = &(NUTIRegistry::Get());
	}
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTIRegistry, "HasUTI")
{


	// Perform the test
	REQUIRE(utiRegistry->HasUTI(kNUTTypeImage));
	REQUIRE(utiRegistry->HasUTI(kNUTTypeXML));
	REQUIRE(utiRegistry->HasUTI(kNUTTypeKMZ));

	REQUIRE(!utiRegistry->HasUTI("invalid.tutiregistry.hasuti.apple"));
	REQUIRE(!utiRegistry->HasUTI("invalid.tutiregistry.hasuti.banana"));
	REQUIRE(!utiRegistry->HasUTI("invalid.tutiregistry.hasuti.pear"));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTIRegistry, "FindUTI")
{


	// Perform the test
	REQUIRE(utiRegistry->FindUTI(NUTITag::MIMEType, "image/gif") == kNUTTypeGIF);
	REQUIRE(utiRegistry->FindUTI(NUTITag::MIMEType, "image/png") == kNUTTypePNG);
	REQUIRE(utiRegistry->FindUTI(NUTITag::MIMEType, "image/jpeg") == kNUTTypeJPEG);

	REQUIRE(utiRegistry->FindUTI(NUTITag::Extension, "gif") == kNUTTypeGIF);
	REQUIRE(utiRegistry->FindUTI(NUTITag::Extension, "png") == kNUTTypePNG);
	REQUIRE(utiRegistry->FindUTI(NUTITag::Extension, "jpg") == kNUTTypeJPEG);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTIRegistry, "GetDescription")
{


	// Perform the test
	REQUIRE(!utiRegistry->GetDescription(kNUTTypeGIF).IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTIRegistry, "GetConformance")
{


	// Perform the test
	REQUIRE(!utiRegistry->GetConformance(kNUTTypeGIF).empty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTIRegistry, "GetTags")
{


	// Perform the test
	REQUIRE(!utiRegistry->GetTags(kNUTTypeGIF).empty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTIRegistry, "GetTag")
{


	// Perform the test
	REQUIRE(utiRegistry->GetTag(kNUTTypeGIF, NUTITag::Extension) == "gif");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTIRegistry, "ConformsTo")
{


	// Perform the test
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeData, kNUTTypeItem));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeDirectory, kNUTTypeItem));

	REQUIRE(utiRegistry->ConformsTo(kNUTTypeText, kNUTTypeData));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeText, kNUTTypeContent));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeDelimitedText, kNUTTypeText));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeCSV, kNUTTypeDelimitedText));

	REQUIRE(utiRegistry->ConformsTo(kNUTTypeXML, kNUTTypeText));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeGPX, kNUTTypeXML));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeKML, kNUTTypeXML));

	REQUIRE(utiRegistry->ConformsTo(kNUTTypeImage, kNUTTypeData));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeImage, kNUTTypeContent));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeGIF, kNUTTypeImage));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeJPEG, kNUTTypeImage));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypePNG, kNUTTypeImage));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeSVG, kNUTTypeImage));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeTIFF, kNUTTypeImage));

	REQUIRE(utiRegistry->ConformsTo(kNUTTypeDMG, kNUTTypeDiskImage));
	REQUIRE(utiRegistry->ConformsTo(kNUTTypeISO, kNUTTypeDiskImage));

	REQUIRE(utiRegistry->ConformsTo(kNUTTypeKMZ, kNUTTypeData));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TUTIRegistry, "Add")
{


	// Perform the test
	NString testUTI = "invalid.tutiregistry.add";

	REQUIRE(!utiRegistry->HasUTI(testUTI));

	utiRegistry->Add(testUTI,
					 "TUTIRegistry Test",
					 {kNUTTypeContent},
					 {{NUTITag::MIMEType, "tutiregistry/foo"}, {NUTITag::Extension, "foo"}});

	REQUIRE(utiRegistry->HasUTI(testUTI));
}
