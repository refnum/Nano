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
//		NTestFixture::GetElapsedTime : Get the elapsed time.
//----------------------------------------------------------------------------
NTime NTestFixture::GetElapsedTime(void) const
{	NTime	theTime;



	// Get the time
	theTime = NTimeUtilities::GetTime() - mTimeStart;

	return(theTime);
}





//============================================================================
//		NTestFixture::TimeUnder : Is the time under a threshold?
//----------------------------------------------------------------------------
bool NTestFixture::TimeUnder(NTime theTime) const
{


	// Check the time
	return(GetElapsedTime() <= theTime);
}





//============================================================================
//		NTestFixture::TimeOver : Is the time over a threshold?
//----------------------------------------------------------------------------
bool NTestFixture::TimeOver(NTime theTime) const
{


	// Check the time
	return(GetElapsedTime() >= theTime);
}




