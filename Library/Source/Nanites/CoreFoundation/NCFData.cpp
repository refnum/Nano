/*	NAME:
		NCFData.cpp

	DESCRIPTION:
		CFDataRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFData.h"





//============================================================================
//		NCFData::NCFData : Constructor.
//----------------------------------------------------------------------------
NCFData::NCFData(const NData &theData)
			: NData(theData)
{
}





//============================================================================
//		NCFData::NCFData : Constructor.
//----------------------------------------------------------------------------
NCFData::NCFData(CFDataRef cfObject, bool takeOwnership)
{


	// Initialize ourselves
	SetObject(cfObject, takeOwnership);
}





//============================================================================
//		NCFData::NCFData : Constructor.
//----------------------------------------------------------------------------
NCFData::NCFData(void)
{
}





//============================================================================
//		NCFData::~NCFData : Destructor.
//----------------------------------------------------------------------------
NCFData::~NCFData(void)
{
}





//============================================================================
//		NCFData::GetObject : Get the object.
//----------------------------------------------------------------------------
NCFObject NCFData::GetObject(void) const
{	NCFObject		theObject;



	// Get the object
	theObject.SetObject(CFDataCreate(kCFAllocatorNano, GetData(), GetSize()));
	
	return(theObject);
}





//============================================================================
//		NCFData::SetObject : Set the object.
//----------------------------------------------------------------------------
bool NCFData::SetObject(CFDataRef cfObject, bool takeOwnership)
{	NCFObject		theObject(cfObject, takeOwnership);
	bool			isValid;



	// Get the state we need
	isValid = (cfObject != NULL);
	Clear();



	// Set the object
	if (isValid)
		*this = NData(CFDataGetLength(cfObject), CFDataGetBytePtr(cfObject));

	return(isValid);
}


