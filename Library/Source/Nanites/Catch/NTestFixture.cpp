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
//		NTestFixture::GetTestID : Get the current test ID.
//----------------------------------------------------------------------------
NString NTestFixture::GetTestID(void) const
{


	// Validate our state
	NN_ASSERT(!mID.IsEmpty());



	// Get the name
	return(mID);
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





#pragma mark protected
//============================================================================
//		NTestFixture::WillSetUp : Before SETUP.
//----------------------------------------------------------------------------
void NTestFixture::WillSetUp(void)
{
}





//============================================================================
//		NTestFixture::DidSetUp : After SETUP.
//----------------------------------------------------------------------------
void NTestFixture::DidSetUp(void)
{
}





//============================================================================
//		NTestFixture::WillTearDown : Before TEARDOWN.
//----------------------------------------------------------------------------
void NTestFixture::WillTearDown(void)
{
}





//============================================================================
//		NTestFixture::DidTearDown : After TEARDOWN.
//----------------------------------------------------------------------------
void NTestFixture::DidTearDown(void)
{
}





#pragma mark protected
//============================================================================
//		NTestFixture::setCurrentTest : Set the current test.
//----------------------------------------------------------------------------
void NTestFixture::setCurrentTest(const std::string &testName)
{


	// Validate our parameters
	NN_ASSERT(!testName.empty());
	
	
	
	// Set the name
	mID = NString(testName.c_str(), kNStringLength);
}





//============================================================================
//		NTestFixture::willSetUp : About to invoke setup.
//----------------------------------------------------------------------------
void NTestFixture::willSetUp(void)
{


	// Update our state
	WillSetUp();
}





//============================================================================
//		NTestFixture::didSetUp : Have invoked setUp.
//----------------------------------------------------------------------------
void NTestFixture::didSetUp(void)
{


	// Update our state
	DidSetUp();
}





//============================================================================
//		NTestFixture::willTearDown : About to invoke tearDown.
//----------------------------------------------------------------------------
void NTestFixture::willTearDown(void)
{


	// Update our state
	WillTearDown();
}





//============================================================================
//		NTestFixture::didTearDown : Have invoked tearDown.
//----------------------------------------------------------------------------
void NTestFixture::didTearDown(void)
{


	// Update our state
	DidTearDown();
}



