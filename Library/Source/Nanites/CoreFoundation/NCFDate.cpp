/*	NAME:
		NCFDate.cpp

	DESCRIPTION:
		CFDateRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFDate.h"





//============================================================================
//		NCFDate::NCFDate : Constructor.
//----------------------------------------------------------------------------
NCFDate::NCFDate(const NDate &theDate)
			: NDate(theDate)
{
}





//============================================================================
//		NCFDate::NCFDate : Constructor.
//----------------------------------------------------------------------------
NCFDate::NCFDate(CFDateRef cfObject, bool takeOwnership)
{


	// Initialize ourselves
	SetObject(cfObject, takeOwnership);
}





//============================================================================
//		NCFDate::NCFDate : Constructor.
//----------------------------------------------------------------------------
NCFDate::NCFDate(void)
{
}





//============================================================================
//		NCFDate::~NCFDate : Destructor.
//----------------------------------------------------------------------------
NCFDate::~NCFDate(void)
{
}





//============================================================================
//		NCFDate::GetObject : Get the object.
//----------------------------------------------------------------------------
NCFObject NCFDate::GetObject(void) const
{	NCFObject		theObject;



	// Get the object
	theObject.SetObject(CFDateCreate(kCFAllocatorNano, GetTime()));

	return(theObject);
}





//============================================================================
//		NCFDate::SetObject : Set the object.
//----------------------------------------------------------------------------
bool NCFDate::SetObject(CFDateRef cfObject, bool takeOwnership)
{	NCFObject		theObject(cfObject, takeOwnership);
	bool			isValid;



	// Get the state we need
	isValid = (cfObject != NULL);
	SetTime(0.0);



	// Set the object
	if (isValid)
		SetTime(CFDateGetAbsoluteTime(cfObject));

	return(isValid);
}


