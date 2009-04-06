/*	NAME:
		NData.cpp

	DESCRIPTION:
		Data object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NData.h"





//============================================================================
//		NData::NData : Constructor.
//----------------------------------------------------------------------------
NData::NData(NIndex theSize, const void *thePtr, bool makeCopy)
{


	// Validate our parameters
	NN_ASSERT(theSize >= 0);



	// Initialize ourselves
	mExternalSize = 0;
	mExternalPtr  = NULL;

	if (theSize != 0)
		SetData(theSize, thePtr, makeCopy);
}





//============================================================================
//		NData::NData : Constructor.
//----------------------------------------------------------------------------
NData::NData(void)
{


	// Initialize ourselves
	mExternalSize = 0;
	mExternalPtr  = NULL;
}





//============================================================================
//		NData::~NData : Destructor.
//----------------------------------------------------------------------------
NData::~NData(void)
{
}





//============================================================================
//		NData::Clear : Clear the value.
//----------------------------------------------------------------------------
void NData::Clear(void)
{


	// Clear the value
	NSharedValueData::Clear();

	mExternalSize = 0;
	mExternalPtr  = NULL;
}





//============================================================================
//		NData::GetSize : Get the size.
//----------------------------------------------------------------------------
NIndex NData::GetSize(void) const
{	NIndex		theSize;



	// Get the size
	if (mExternalSize != 0)
		theSize = mExternalSize;
	else
		theSize = GetImmutable()->size();

	return(theSize);
}





//============================================================================
//		NData::SetSize : Set the size.
//----------------------------------------------------------------------------
void NData::SetSize(NIndex theSize)
{	NDataValue		*theData;



	// Validate our parameters
	NN_ASSERT(theSize >= 0);



	// Set the size
	if (theSize == 0)
		Clear();
	else
		{
		// Keep using an external buffer
		if (mExternalSize != 0 && theSize <= mExternalSize)
			mExternalSize = theSize;


		// Or resize our own (if necessary)
		else if (theSize != GetSize())
			{
			theData = GetMutable();
			theData->resize(theSize, 0x00);
			}
		}
}





//============================================================================
//		NData::GetData : Get the data.
//----------------------------------------------------------------------------
const UInt8 *NData::GetData(NIndex theOffset) const
{	const UInt8		*thePtr;



	// Validate our parameters
	NN_ASSERT(theOffset >= 0 && theOffset < GetSize());



	// Get the data
	if (mExternalPtr != NULL)
		thePtr = ((const UInt8 *) mExternalPtr) + theOffset;
	else
		thePtr = &GetImmutable()->at(theOffset);

	return(thePtr);
}





//============================================================================
//		NData::GetData : Get the data.
//----------------------------------------------------------------------------
UInt8 *NData::GetData(NIndex theOffset)
{	UInt8	*thePtr;



	// Validate our parameters and state
	NN_ASSERT(theOffset >= 0 && theOffset < GetSize());
	NN_ASSERT(mExternalSize == 0 && mExternalPtr == NULL);



	// Get the data
	thePtr = &GetMutable()->at(theOffset);

	return(thePtr);
}





//============================================================================
//		NData::SetData : Set the data.
//----------------------------------------------------------------------------
void NData::SetData(NIndex theSize, const void *thePtr, bool makeCopy)
{	NDataValue		*theData;



	// Validate our parameters
	NN_ASSERT(theSize >= 0);



	// Copy the data
	if (makeCopy)
		{
		theData = GetMutable();
		theData->resize(theSize);

		if (theSize != 0)
			memcpy(&theData->at(0), thePtr, theSize);
		}


	// Or use the external data
	else
		{
		Clear();

		mExternalSize = theSize;
		mExternalPtr  = thePtr;
		}
}





//============================================================================
//		NData::AppendData : Append data to the buffer.
//----------------------------------------------------------------------------
UInt8 *NData::AppendData(NIndex theSize, const void *thePtr)
{	NDataValue		*theData;
	UInt8			*dstPtr;
	NIndex			oldSize;



	// Validate our parameters
	NN_ASSERT(theSize >= 0);



	// Check our parameters
	if (theSize == 0)
		return(NULL);



	// Get the state we need
	theData = GetMutable();
	oldSize = theData->size();



	// Append the data
	theData->resize(oldSize + theSize, 0x00);
	dstPtr = &theData->at(oldSize);

	if (thePtr != NULL)
		memcpy(dstPtr, thePtr, theSize);

	return(dstPtr);
}





//============================================================================
//		NData::AppendData : Append data to the buffer.
//----------------------------------------------------------------------------
UInt8 *NData::AppendData(const NData &theData)
{


	// Check our parameters
	if (theData.IsEmpty())
		return(NULL);



	// Append the data
	return(AppendData(theData.GetSize(), theData.GetData()));
}





//============================================================================
//		NData::ReplaceData : Replace data within the buffer.
//----------------------------------------------------------------------------
UInt8 *NData::ReplaceData(const NRange &theRange, NIndex theSize, const void *thePtr)
{	NIndex			oldSize, blockSize, sizeDelta;
	NDataValue		*theData;
	UInt8			*dstPtr;
	const UInt8		*srcPtr;



	// Validate our parameters
	NN_ASSERT(theRange.GetNext() >= 0 && theRange.GetNext() < GetSize());
	NN_ASSERT(theSize            >= 0);



	// Get the state we need
	theData = GetMutable();
	oldSize = theData->size();



	// Insert the block into a smaller range
	if (theSize >= theRange.GetSize())
		{
		sizeDelta = theSize - theRange.GetSize();
		theData->resize(oldSize + sizeDelta);

		srcPtr    = &theData->at(theRange.GetLocation() + theRange.GetSize());
		dstPtr    = &theData->at(theRange.GetLocation() + theSize);
		blockSize = oldSize - theRange.GetNext();
		memmove(dstPtr, srcPtr, blockSize);

		srcPtr    = (const UInt8 *) thePtr;
		dstPtr    = &theData->at(theRange.GetLocation());
		blockSize = theSize;
		memcpy(dstPtr, srcPtr, blockSize);
		}


	// Insert the block into a larger range
	else
		{
		srcPtr    = (const UInt8 *) thePtr;
		dstPtr    = &theData->at(theRange.GetLocation());
		blockSize = theSize;
		memcpy(dstPtr, srcPtr, blockSize);

		srcPtr    = &theData->at(theRange.GetLocation() + theRange.GetSize());
		dstPtr    = &theData->at(theRange.GetLocation() + theSize);
		blockSize = oldSize - theRange.GetNext();
		memmove(dstPtr, srcPtr, blockSize);

		sizeDelta = theRange.GetSize() - theSize;
		theData->resize(oldSize - sizeDelta);
		}



	// Get the data
	dstPtr = &theData->at(theRange.GetLocation());

	return(dstPtr);
}





//============================================================================
//		NData::RemoveData : Remove data from the buffer.
//----------------------------------------------------------------------------
void NData::RemoveData(NIndex theSize, bool fromFront)
{	NRange		theRange;



	// Validate our parameters
	NN_ASSERT(theSize > 0 && theSize <= GetSize());



    // Remove the data
	if (fromFront)
		theRange = NRange(0, theSize);
	else
		theRange = NRange(GetSize() - theSize, theSize);

	RemoveData(theRange);
}





//============================================================================
//      NData::RemoveData : Remove data from the buffer.
//----------------------------------------------------------------------------
void NData::RemoveData(const NRange &theRange)
{	NDataValueIterator		iterStart, iterEnd;
	NDataValue				*theData;



	// Validate our parameters
	NN_ASSERT(theRange.GetLocation() >= 0);
	NN_ASSERT(theRange.GetLast()     <  GetSize());



	// Get the state we need
	theData   = GetMutable();
	iterStart = theData->begin() + theRange.GetFirst();
	iterEnd   = theData->begin() + theRange.GetNext();



	// Remove the data
	theData->erase(iterStart, iterEnd);
}





//============================================================================
//		NData::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NData::Compare(const NData &theValue) const
{	const void				*ourPtr, *otherPtr;
	NIndex					ourSize, otherSize;
	NComparison				theResult;



	// Get the state we need
	ourPtr   = (         mExternalPtr != NULL) ?          mExternalPtr  :          GetData();
	otherPtr = (theValue.mExternalPtr != NULL) ? theValue.mExternalPtr  : theValue.GetData();

	ourSize   = (         mExternalPtr != NULL) ?          mExternalSize :          GetSize();
	otherSize = (theValue.mExternalPtr != NULL) ? theValue.mExternalSize : theValue.GetSize();



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = CompareData(ourSize, ourPtr, otherSize, otherPtr);

	return(theResult);
}





//============================================================================
//		NData::GetMutable : Get the mutable value.
//----------------------------------------------------------------------------
#pragma mark -
NDataValue *NData::GetMutable(void)
{	NDataValue		*theData;



	// Get the state we need
	theData = NSharedValueData::GetMutable();



	// Copy external data
	//
	// External data can be supported indefinitely for immutable
	// access, but must be copied when mutable access is required.
	if (mExternalSize != 0)
		{
		theData->resize(mExternalSize);
		memcpy(&theData->at(0), mExternalPtr, mExternalSize);

		mExternalSize = 0;
		mExternalPtr  = NULL;
		}

	return(theData);
}





//============================================================================
//		NData::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
const NDataValue *NData::GetNullValue(void) const
{	static NDataValue		sNullValue;



	// Get the value
	return(&sNullValue);
}





