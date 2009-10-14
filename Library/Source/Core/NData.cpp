/*	NAME:
		NData.cpp

	DESCRIPTION:
		Data object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"
#include "NData.h"





//============================================================================
//		NData::NData : Constructor.
//----------------------------------------------------------------------------
NData::NData(CFDataRef cfData)
{


	// Initialize ourselves
	//
	// We will release the object when we initialize ourselves, so
	// we retain it first to avoid releasing our caller's reference.
	CFSafeRetain(cfData);
	
	InitializeSelf(cfData);
}





//============================================================================
//		NData::NData : Constructor.
//----------------------------------------------------------------------------
NData::NData(void)
{


	// Initialize ourselves
	InitializeSelf(NULL);
}





//============================================================================
//		NData::~NData : Destructor.
//----------------------------------------------------------------------------
NData::~NData(void)
{
}





//============================================================================
//		NData::IsEmpty : Is the data empty?
//----------------------------------------------------------------------------
bool NData::IsEmpty(void) const
{


	// Check our size
	return(GetSize() == 0);
}





//============================================================================
//		NData::IsNotEmpty : Is the data not empty?
//----------------------------------------------------------------------------
bool NData::IsNotEmpty(void) const
{


	// Check our size
	return(GetSize() != 0);
}





//============================================================================
//		NData::GetSize : Get the size.
//----------------------------------------------------------------------------
UInt32 NData::GetSize(void) const
{


	// Get our size
	return(CFDataGetLength(*this));
}





//============================================================================
//		NData::SetSize : Set the size.
//----------------------------------------------------------------------------
bool NData::SetSize(UInt32 theSize)
{


	// Set our size
	CFDataSetLength(*this, theSize);
	NN_ASSERT(GetSize() == theSize);

	return(GetSize() == theSize);
}





//============================================================================
//		NData::Clear : Clear the data.
//----------------------------------------------------------------------------
void NData::Clear(void)
{


	// Clear the data
	SetSize(0);
}





//============================================================================
//		NData::GetData : Get the data.
//----------------------------------------------------------------------------
const UInt8 *NData::GetData(UInt32 theOffset) const
{	const UInt8		*thePtr;



	// Validate our parameters
	NN_ASSERT(theOffset == 0 || theOffset < GetSize());



	// Get the data
	thePtr = CFDataGetBytePtr(*this);
	NN_ASSERT(thePtr != NULL);
	
	thePtr += theOffset;
	
	return(thePtr);
}





//============================================================================
//		NData::GetData : Get the data.
//----------------------------------------------------------------------------
UInt8 *NData::GetData(UInt32 theOffset)
{	UInt8		*thePtr;



	// Validate our parameters
	NN_ASSERT(theOffset == 0 || theOffset < GetSize());



	// Get the data
	thePtr = CFDataGetMutableBytePtr(*this);
	NN_ASSERT(thePtr != NULL);
	
	thePtr += theOffset;
	
	return(thePtr);
}





//============================================================================
//		NData::SetData : Set the data.
//----------------------------------------------------------------------------
bool NData::SetData(UInt32 theSize, const void *thePtr)
{


	// Set the data
	Clear();
	
	return(AppendData(theSize, thePtr) != NULL);
}





//============================================================================
//		NData::AppendData : Append data to the buffer.
//----------------------------------------------------------------------------
UInt8 *NData::AppendData(UInt32 theSize, const void *thePtr)
{	UInt32	oldSize, newSize;



	// Get the state we need
	oldSize = GetSize();
	newSize = oldSize + theSize;



	// Append the data
	if (theSize != 0)
		{
		if (thePtr == NULL)
			CFDataIncreaseLength(*this, theSize);
		else
			CFDataAppendBytes(   *this, (const UInt8 *) thePtr, theSize);
        
		if (GetSize() != newSize)
			return(NULL);
		}

	return(GetData(oldSize));
}





//============================================================================
//		NData::ReplaceData : Replace data within the buffer.
//----------------------------------------------------------------------------
UInt8 *NData::ReplaceData(const CFRange &theRange, UInt32 theSize, const void *thePtr)
{	UInt32	newSize;



	// Validate our parameters
	NN_ASSERT((theRange.location + theRange.length) >= 0);
	NN_ASSERT((theRange.location + theRange.length) <  (CFIndex) GetSize());



	// Get the state we need
	newSize = GetSize() - theRange.length + theSize;



	// Replace the data
	CFDataReplaceBytes(*this, theRange, (const UInt8 *) thePtr, theSize);

	if (GetSize() != newSize)
		return(NULL);
	
	return(GetData(theRange.location));
}





//============================================================================
//		NData::RemoveData : Remove data from the buffer.
//----------------------------------------------------------------------------
void NData::RemoveData(const CFRange &theRange)
{


	// Remove the data
	CFDataDeleteBytes(*this, theRange);
}





//============================================================================
//		NData::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
CFTypeRef NData::GetNullValue(void)
{


	// Get the value
	return(CFDataCreateMutable(kCFAllocatorNano, 0));
}





//============================================================================
//		NData::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NData::InitializeSelf(CFDataRef cfData)
{


	// Initialize ourselves
	SetMode(kCFDuplicate);
	
	Set(cfData);



	// Validate our state
	NN_ASSERT(IsValid());
}



