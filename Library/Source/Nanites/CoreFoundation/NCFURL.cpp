/*	NAME:
		NCFURL.cpp

	DESCRIPTION:
		CFURLRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreFoundation.h"
#include "NCFString.h"
#include "NCFURL.h"





//============================================================================
//		NCFURL::NCFURL : Constructor.
//----------------------------------------------------------------------------
NCFURL::NCFURL(const NURL &theURL)
			: NURL(theURL)
{
}





//============================================================================
//		NCFURL::NCFURL : Constructor.
//----------------------------------------------------------------------------
NCFURL::NCFURL(CFURLRef cfObject, bool takeOwnership)
{


	// Initialize ourselves
	SetObject(cfObject, takeOwnership);
}





//============================================================================
//		NCFURL::NCFURL : Constructor.
//----------------------------------------------------------------------------
NCFURL::NCFURL(NSURL *nsObject, bool takeOwnership)
{


	// Initialize ourselves
	SetObject((CFURLRef) nsObject, takeOwnership);
}





//============================================================================
//		NCFURL::NCFURL : Constructor.
//----------------------------------------------------------------------------
NCFURL::NCFURL(void)
{
}





//============================================================================
//		NCFURL::~NCFURL : Destructor.
//----------------------------------------------------------------------------
NCFURL::~NCFURL(void)
{
}





//============================================================================
//		NCFURL::GetObject : Get the object.
//----------------------------------------------------------------------------
NCFObject NCFURL::GetObject(void) const
{	NCFObject	theObject;



	// Get the object
	theObject.SetObject(CFURLCreateWithString(kCFAllocatorNano, ToCF(GetValue()), NULL));

	return(theObject);
}





//============================================================================
//		NCFURL::SetObject : Set the object.
//----------------------------------------------------------------------------
bool NCFURL::SetObject(CFURLRef cfObject, bool takeOwnership)
{	NCFObject		theObject(cfObject, takeOwnership);
	NCFString		theString;
	bool			isValid;



	// Get the state we need
	isValid = (cfObject != NULL);
	Clear();



	// Set the object
	if (isValid)
		{
		if (theString.SetObject(CFURLGetString(cfObject)))
			SetValue(theString);
		}

	return(isValid);
}

