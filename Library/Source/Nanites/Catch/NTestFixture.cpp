/*	NAME:
		NTestFixture.cpp

	DESCRIPTION:
		Nano test fixture for catch.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTimeUtilities.h"

#include "NTestFixture.h"





//============================================================================
//		NTestFixture::NTestFixture : Constructor.
//----------------------------------------------------------------------------
NTestFixture::NTestFixture(void)
{


	// Initialise ourselves
	ResetTime();
}





//============================================================================
//		NTestFixture::~NTestFixture : Destructor.
//----------------------------------------------------------------------------
NTestFixture::~NTestFixture(void)
{
}





//============================================================================
//		NTestFixture::ResetTime : Reset the time.
//----------------------------------------------------------------------------
void NTestFixture::ResetTime(void)
{


	// Reset the time
	mTimeStart = NTimeUtilities::GetTime();
}





//============================================================================
//		NTestFixture::TimeIsUnder : Is the time under a threshold?
//----------------------------------------------------------------------------
bool NTestFixture::TimeIsUnder(NTime theTime) const
{	NTime	elapsedTime;



	// Check the time
	elapsedTime = NTimeUtilities::GetTime() - mTimeStart;

	return(theTime <= elapsedTime);
}





//============================================================================
//		NTestFixture::TimeIsOver : Is the time over a threshold?
//----------------------------------------------------------------------------
bool NTestFixture::TimeIsOver(NTime theTime) const
{	NTime	elapsedTime;



	// Check the time
	elapsedTime = NTimeUtilities::GetTime() - mTimeStart;

	return(theTime >= elapsedTime);
}



