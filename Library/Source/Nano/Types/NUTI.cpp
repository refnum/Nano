/*	NAME:
		NUTI.cpp

	DESCRIPTION:
		Uniform Type Identifier:
		
			http://en.wikipedia.org/wiki/Uniform_Type_Identifier
		
		For now we implement a very simple model, to allow UTIs to be created
		and passed around.
		
		This should be extended with some kind of registration system for
		standard UTI types, to allow us to create a type with one class of tag
		and obtain the equivalent value in another class (e.g., converting a
		MIME type to a filename extension).

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NUTI.h"





//============================================================================
//		NUTI::NUTI : Constructor.
//----------------------------------------------------------------------------
NUTI::NUTI(NUTITagClass theClass, const NString &theTag)
{


	// Validate our parameters
	if (theClass != kNUTITagClassNone)
		NN_ASSERT(!theTag.IsEmpty());


	// Initialize ourselves
	mClass = theClass;
	mTag   = theTag;
}





//============================================================================
//		NUTI::NUTI : Constructor.
//----------------------------------------------------------------------------
NUTI::NUTI(void)
{


	// Initialize ourselves
	mClass = kNUTITagClassNone;
}





//============================================================================
//		NUTI::~NUTI : Destructor.
//----------------------------------------------------------------------------
NUTI::~NUTI(void)
{
}





//============================================================================
//		NUTI::IsValid : Is the UTI valid?
//----------------------------------------------------------------------------
bool NUTI::IsValid(void) const
{


	// Check our state
	return(mClass != kNUTITagClassNone);
}





//============================================================================
//		NUTI::Clear : Clear the UTI.
//----------------------------------------------------------------------------
void NUTI::Clear(void)
{


	// Reset our state
	mClass = kNUTITagClassNone;
	mTag.Clear();
}





//============================================================================
//		NUTI::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NUTI::Compare(const NUTI &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = GetComparison(mClass, theValue.mClass);
		
	if (theResult == kNCompareEqualTo)
		theResult = mTag.Compare(theValue.mTag);

	return(theResult);
}





//============================================================================
//		NUTI::GetMIMEType : Get the MIME type.
//----------------------------------------------------------------------------
NString NUTI::GetMIMEType(void) const
{


	// Validate our state
	NN_ASSERT(mClass == kNUTITagClassMIMEType);
	


	// Get the value
	return(mTag);
}





//============================================================================
//		NUTI::GetFileExtension : Get the file extension.
//----------------------------------------------------------------------------
NString NUTI::GetFileExtension(void) const
{


	// Validate our state
	NN_ASSERT(mClass == kNUTITagClassFileExtension);
	


	// Get the value
	return(mTag);
}


