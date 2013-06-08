/*	NAME:
		NCacheValue.cpp

	DESCRIPTION:
		Cache value.
	
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
#include "NCacheValue.h"





//============================================================================
//		NCacheValue::NCacheValue : Constructor.
//----------------------------------------------------------------------------
NCacheValue::NCacheValue(NIndex theCost)
{


	// Validate our parameters
	NN_ASSERT(theCost >= 0);



	// Initialise ourselves
	mCost       = theCost;
	mAccessTime = NTimeUtilities::GetTime();
}





//============================================================================
//		NCacheValue::~NCacheValue : Destructor.
//----------------------------------------------------------------------------
NCacheValue::~NCacheValue(void)
{
}





//============================================================================
//		NCacheValue::IsPurgeable : Is the value purgeable?
//----------------------------------------------------------------------------
bool NCacheValue::IsPurgeable(void) const
{


	// Check our state
	//
	// Sub-classes can override to prevent a value from being purged.
	return(GetRetainCount() == 1);
}





//============================================================================
//		NCacheValue::GetCost : Get the cost.
//----------------------------------------------------------------------------
NIndex NCacheValue::GetCost(void) const
{


	// Get the cost
	return(mCost);
}





//============================================================================
//		NCacheValue::GetAccessTime : Get the access time.
//----------------------------------------------------------------------------
NTime NCacheValue::GetAccessTime(void) const
{


	// Get the access time
	return(mAccessTime);
}





//============================================================================
//		NCacheValue::SetAccessTime : Set the access time.
//----------------------------------------------------------------------------
void NCacheValue::SetAccessTime(void)
{


	// Set the access time
	mAccessTime = NTimeUtilities::GetTime();
}
