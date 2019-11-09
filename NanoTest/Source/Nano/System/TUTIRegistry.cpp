/*	NAME:
		TUTIRegistry.cpp

	DESCRIPTION:
		NUTIRegistry tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NUTIRegistry.h"
#include "NTestFixture.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NUTIREGISTRY(...)										TEST_NANO(TUTIRegistry, ##__VA_ARGS__)

FIXTURE_NANO(TUTIRegistry)
{
	NUTIRegistry	*utiRegistry;

	SETUP
	{
		utiRegistry = NUTIRegistry::Get();
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUTIREGISTRY("Default")
{


	// Perform the test
	REQUIRE(utiRegistry->GetUTI(kNUTITagClassFileExtension, "gif") == kNUTTypeGIF);
	REQUIRE(utiRegistry->GetUTI(kNUTITagClassFileExtension, "png") == kNUTTypePNG);
	REQUIRE(utiRegistry->GetUTI(kNUTITagClassFileExtension, "jpg") == kNUTTypeJPEG);
}









