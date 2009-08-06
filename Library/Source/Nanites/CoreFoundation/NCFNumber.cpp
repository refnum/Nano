/*	NAME:
		NCFNumber.cpp

	DESCRIPTION:
		CFNumberRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFNumber.h"





//============================================================================
//		NCFNumber::NCFNumber : Constructor.
//----------------------------------------------------------------------------
NCFNumber::NCFNumber(const NVariant &theValue)
			: NNumber(theValue)
{
}





//============================================================================
//		NCFNumber::NCFNumber : Constructor.
//----------------------------------------------------------------------------
NCFNumber::NCFNumber(const NNumber &theNumber)
			: NNumber(theNumber)
{
}





//============================================================================
//		NCFNumber::NCFNumber : Constructor.
//----------------------------------------------------------------------------
NCFNumber::NCFNumber(CFNumberRef cfObject, bool takeOwnership)
{


	// Initialize ourselves
	SetObject(cfObject, takeOwnership);
}





//============================================================================
//		NCFNumber::NCFNumber : Constructor.
//----------------------------------------------------------------------------
NCFNumber::NCFNumber(void)
{
}





//============================================================================
//		NCFNumber::~NCFNumber : Destructor.
//----------------------------------------------------------------------------
NCFNumber::~NCFNumber(void)
{
}





//============================================================================
//		NCFNumber::GetObject : Get the object.
//----------------------------------------------------------------------------
NCFObject NCFNumber::GetObject(void) const
{	Float64			valueFloat64;
	Float32			valueFloat32;
	SInt64			valueSInt64;
	NCFObject		theObject;



	// Get the object
	if (GetValueSInt64(valueSInt64))
		theObject.SetObject(CFNumberCreate(kCFAllocatorNano, kCFNumberLongLongType, &valueSInt64));

	else if (GetValueFloat32(valueFloat32))
		theObject.SetObject(CFNumberCreate(kCFAllocatorNano, kCFNumberFloatType,    &valueFloat32));

	else if (GetValueFloat64(valueFloat64))
		theObject.SetObject(CFNumberCreate(kCFAllocatorNano, kCFNumberDoubleType,   &valueFloat64));
	
	else
		NN_LOG("Unable to convert '%@' to CFNumber", GetString());

	return(theObject);
}





//============================================================================
//		NCFNumber::SetObject : Set the object.
//----------------------------------------------------------------------------
bool NCFNumber::SetObject(CFNumberRef cfObject, bool takeOwnership)
{	NCFObject		theObject(cfObject, takeOwnership);
	Float64			valueFloat64;
	Float32			valueFloat32;
	SInt64			valueSInt64;
	bool			isValid;



	// Get the state we need
	isValid = (cfObject != NULL);
	SetValueSInt8(0);



	// Set the object
	if (isValid)
		{
		if (CFNumberGetValue(     cfObject, kCFNumberLongLongType, &valueSInt64))
			SetValueSInt64(valueSInt64);
		
		else if (CFNumberGetValue(cfObject, kCFNumberFloatType,    &valueFloat32))
			SetValueFloat32(valueFloat32);
		
		else if (CFNumberGetValue(cfObject, kCFNumberDoubleType,   &valueFloat64))
			SetValueFloat64(valueFloat64);

		else
			NN_LOG("Unable to convert CFNumber to NNumber");
		}

	return(isValid);
}


