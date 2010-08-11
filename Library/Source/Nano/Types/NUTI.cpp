/*	NAME:
		NUTI.cpp

	DESCRIPTION:
		Uniform Type Identifier:
		
			http://en.wikipedia.org/wiki/Uniform_Type_Identifier

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NUTIRegistry.h"
#include "NEncoder.h"
#include "NUTI.h"





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NUTI);





//============================================================================
//		NUTI::NUTI : Constructor.
//----------------------------------------------------------------------------
NUTI::NUTI(NUTITagClass theClass, const NString &theTag)
{


	// Initialize ourselves
	mUTI = NUTIRegistry::Get()->GetUTI(theClass, theTag);
}





//============================================================================
//		NUTI::NUTI : Constructor.
//----------------------------------------------------------------------------
NUTI::NUTI(const NString &theUTI)
{


	// Initialize ourselves
	mUTI = theUTI;
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


	// Validate our state
	NN_ASSERT(kNUTTypeNone.IsEmpty());



	// Check our state
	return(!mUTI.IsEmpty());
}





//============================================================================
//		NUTI::ConformsTo : Does the UTI conform to another?
//----------------------------------------------------------------------------
bool NUTI::ConformsTo(const NUTI &theUTI) const
{	NStringList		conformsTo;
	bool			theResult;



	// Check for conformance
	//
	// TODO - this should also check for inherited conformance rather than
	// direct conformance, possibly by moving this to the registry so that
	// we can do a more efficient global search.
	theResult = (mUTI == theUTI.mUTI);

	if (!theResult)
		{
		conformsTo = NUTIRegistry::Get()->GetConformsTo(mUTI);
		theResult  = contains(conformsTo, theUTI.mUTI);
		}
	
	return(theResult);
}





//============================================================================
//		NUTI::Clear : Clear the UTI.
//----------------------------------------------------------------------------
void NUTI::Clear(void)
{


	// Reset our state
	mUTI = kNUTTypeNone;
}





//============================================================================
//		NUTI::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NUTI::Compare(const NUTI &theValue) const
{


	// Compare the value
	return(mUTI.Compare(theValue.mUTI));
}





//============================================================================
//		NUTI::GetUTI : Get the UTI.
//----------------------------------------------------------------------------
NString NUTI::GetUTI(void) const
{


	// Get the UTI
	return(mUTI);
}





//============================================================================
//		NUTI::GetDescription : Get the description.
//----------------------------------------------------------------------------
NString NUTI::GetDescription(void) const
{


	// Get the value
	return(NUTIRegistry::Get()->GetDescription(mUTI));
}





//============================================================================
//		NUTI::GetTag : Get a tag.
//----------------------------------------------------------------------------
NString NUTI::GetTag(NUTITagClass theClass) const
{


	// Get the value
	return(NUTIRegistry::Get()->GetTag(mUTI, theClass));
}





//============================================================================
//		NUTI::GetMIMEType : Get the MIME type.
//----------------------------------------------------------------------------
NString NUTI::GetMIMEType(void) const
{


	// Get the value
	return(GetTag(kNUTITagClassMIMEType));
}





//============================================================================
//		NUTI::GetFileExtension : Get the file extension.
//----------------------------------------------------------------------------
NString NUTI::GetFileExtension(void) const
{


	// Get the value
	return(GetTag(kNUTITagClassFileExtension));
}





//============================================================================
//		NUTI::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NUTI::operator NFormatArgument(void) const
{


	// Get the value
	return(mUTI);
}





//============================================================================
//      NUTI::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
#pragma mark -
void NUTI::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeString(kNEncoderValueKey, mUTI);
}





//============================================================================
//      NUTI::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NUTI::DecodeSelf(const NEncoder &theEncoder)
{


	// Encode the object
	mUTI = theEncoder.DecodeString(kNEncoderValueKey);
}



