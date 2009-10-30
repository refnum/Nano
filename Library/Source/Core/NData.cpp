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
	Set(cfData, false);
	
	if (cfData != NULL)
		NN_ASSERT(IsValid());
}





//============================================================================
//		NData::~NData : Destructor.
//----------------------------------------------------------------------------
NData::~NData(void)
{
}





//============================================================================
//		NData::GetSize : Get the size.
//----------------------------------------------------------------------------
UInt32 NData::GetSize(void) const
{


	// Get the size
	return(CFDataGetLength(*this));
}





//============================================================================
//		NData::SetSize : Set the size.
//----------------------------------------------------------------------------
bool NData::SetSize(UInt32 theSize)
{


	// Set our size
	if (MakeMutable())
		{
		CFDataSetLength(*this, theSize);
		NN_ASSERT(GetSize() == theSize);
		}

	return(GetSize() == theSize);
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
	thePtr = NULL;
	
	if (MakeMutable())
		{
		thePtr = CFDataGetMutableBytePtr(*this);
		NN_ASSERT(thePtr != NULL);
	
		thePtr += theOffset;
		}
	
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
	if (theSize != 0 && MakeMutable())
		{
		if (thePtr == NULL)
			CFDataIncreaseLength(*this, theSize);
		else
			CFDataAppendBytes(   *this, (const UInt8 *) thePtr, theSize);
        }



	// Get the data
	if (GetSize() == oldSize)
		return(NULL);

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
	if (MakeMutable())
		CFDataReplaceBytes(*this, theRange, (const UInt8 *) thePtr, theSize);
	else
		return(NULL);



	// Get the data
	if (GetSize() != newSize)
		return(NULL);
	
	return(GetData(theRange.location));
}





//============================================================================
//		NData::RemoveData : Remove data from the buffer.
//----------------------------------------------------------------------------
void NData::RemoveData(UInt32 theSize, bool fromFront)
{	CFRange		theRange;



	// Validate our parameters
	NN_ASSERT(theSize <= GetSize());



    // Remove the data
	if (fromFront)
		theRange = CFRangeMake(0, theSize);
	else
		theRange = CFRangeMake(GetSize() - theSize, theSize);

	RemoveData(theRange);
}





//============================================================================
//      NData::RemoveData : Remove data from the buffer.
//----------------------------------------------------------------------------
void NData::RemoveData(const CFRange &theRange)
{


	// Validate our parameters
	NN_ASSERT(theRange.location >= 0);
	NN_ASSERT(CFRangeGetLast(theRange) < (CFIndex) GetSize());



	// Remove the data
	if (MakeMutable())
		CFDataDeleteBytes(*this, theRange);
}





//============================================================================
//		NData::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
CFTypeRef NData::GetNullValue(void)
{	static CFDataRef	sNullValue = CFDataCreate(kCFAllocatorNano, NULL, 0);



	// Get the value
	return(sNullValue);
}


