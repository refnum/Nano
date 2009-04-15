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
{	const NDataValue		*theValue;
	NIndex					theSize;



	// Get the size
	if (mExternalSize != 0)
		theSize = mExternalSize;
	else
		{
		theValue = GetImmutable();
		theSize  = theValue->size();
		}

	return(theSize);
}





//============================================================================
//		NData::SetSize : Set the size.
//----------------------------------------------------------------------------
void NData::SetSize(NIndex theSize)
{	NDataValue		*theValue;



	// Validate our parameters
	NN_ASSERT(theSize >= 0);



	// Check our state
	if (theSize == GetSize())
		return;



	// Set the size
	if (theSize == 0)
		Clear();
	else
		{
		// Keep using an external buffer
		if (mExternalSize != 0 && theSize <= mExternalSize)
			mExternalSize = theSize;


		// Or resize our own
		else
			{
			theValue = GetMutable();
			theValue->resize(theSize, 0x00);
			}
		}
}





//============================================================================
//		NData::GetData : Get the data.
//----------------------------------------------------------------------------
const UInt8 *NData::GetData(NIndex theOffset) const
{	const NDataValue		*theValue;
	const UInt8				*thePtr;



	// Validate our parameters
#if NN_DEBUG
	if (!IsEmpty())
		NN_ASSERT(theOffset >= 0 && theOffset < GetSize());
#endif



	// Get the data
	if (IsEmpty())
		thePtr = NULL;

	else if (mExternalPtr != NULL)
		thePtr = ((const UInt8 *) mExternalPtr) + theOffset;

	else
		{
		theValue = GetImmutable();
		thePtr   = &theValue->at(theOffset);
		}

	return(thePtr);
}





//============================================================================
//		NData::GetData : Get the data.
//----------------------------------------------------------------------------
UInt8 *NData::GetData(NIndex theOffset)
{	NDataValue		*theValue;
	UInt8			*thePtr;



	// Validate our parameters and state
#if NN_DEBUG
	if (!IsEmpty())
		NN_ASSERT(theOffset >= 0 && theOffset < GetSize());
#endif

	NN_ASSERT(mExternalSize == 0 && mExternalPtr == NULL);



	// Get the data
	if (IsEmpty())
		thePtr = NULL;
	
	else
		{
		theValue = GetMutable();
		thePtr   = &theValue->at(theOffset);
		}

	return(thePtr);
}





//============================================================================
//		NData::GetData : Get the data.
//----------------------------------------------------------------------------
NData NData::GetData(const NRange &theRange) const
{	NData		theResult;



	// Get the data
	theResult.SetData(theRange.GetSize(), GetData(theRange.GetLocation()));
	
	return(theResult);
}





//============================================================================
//		NData::SetData : Set the data.
//----------------------------------------------------------------------------
void NData::SetData(NIndex theSize, const void *thePtr, bool makeCopy)
{	NDataValue		*theValue;



	// Validate our parameters
	NN_ASSERT(theSize >= 0);



	// Copy the data
	if (makeCopy)
		{
		theValue = GetMutable();
		theValue->resize(theSize);

		if (theSize != 0 && thePtr != NULL)
			memcpy(&theValue->at(0), thePtr, theSize);
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
//		NData::AppendData : Append data.
//----------------------------------------------------------------------------
UInt8 *NData::AppendData(NIndex theSize, const void *thePtr)
{	NDataValue		*theValue;
	UInt8			*dstPtr;
	NIndex			oldSize;



	// Validate our parameters
	NN_ASSERT(theSize >= 0);



	// Check our parameters
	if (theSize == 0)
		return(NULL);



	// Get the state we need
	theValue = GetMutable();
	oldSize  = theValue->size();



	// Append the data
	theValue->resize(oldSize + theSize, 0x00);
	dstPtr = &theValue->at(oldSize);

	if (thePtr != NULL)
		memcpy(dstPtr, thePtr, theSize);

	return(dstPtr);
}





//============================================================================
//		NData::AppendData : Append data.
//----------------------------------------------------------------------------
UInt8 *NData::AppendData(const NData &theData)
{	UInt8		*thePtr;



	// Check our parameters
	if (theData.IsEmpty())
		return(NULL);



	// Append the data
	//
	// If we're empty then we can share the value, rather than appending bytes.
	if (IsEmpty())
		{
		*this  = theData;
		thePtr = GetData();
		}
	else
		thePtr = AppendData(theData.GetSize(), theData.GetData());
	
	return(thePtr);
}





//============================================================================
//		NData::RemoveData : Remove data.
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
//      NData::RemoveData : Remove data.
//----------------------------------------------------------------------------
void NData::RemoveData(const NRange &theRange)
{	NDataValueIterator		iterStart, iterEnd;
	NDataValue				*theValue;



	// Validate our parameters
	NN_ASSERT(theRange.GetLocation() >= 0);
	NN_ASSERT(theRange.GetLast()     <  GetSize());



	// Get the state we need
	theValue  = GetMutable();
	iterStart = theValue->begin() + theRange.GetFirst();
	iterEnd   = theValue->begin() + theRange.GetNext();



	// Remove the data
	theValue->erase(iterStart, iterEnd);
}





//============================================================================
//		NData::ReplaceData : Replace data.
//----------------------------------------------------------------------------
UInt8 *NData::ReplaceData(const NRange &theRange, NIndex theSize, const void *thePtr)
{	NIndex			oldSize, blockSize, sizeDelta;
	NDataValue		*theValue;
	UInt8			*dstPtr;
	const UInt8		*srcPtr;



	// Validate our parameters
	NN_ASSERT(theRange.GetNext() >= 0 && theRange.GetNext() < GetSize());
	NN_ASSERT(theSize            >= 0);



	// Get the state we need
	theValue = GetMutable();
	oldSize  = theValue->size();



	// Insert the block into a smaller range
	if (theSize >= theRange.GetSize())
		{
		sizeDelta = theSize - theRange.GetSize();
		theValue->resize(oldSize + sizeDelta);

		srcPtr    = &theValue->at(theRange.GetLocation() + theRange.GetSize());
		dstPtr    = &theValue->at(theRange.GetLocation() + theSize);
		blockSize = oldSize - theRange.GetNext();
		memmove(dstPtr, srcPtr, blockSize);

		srcPtr    = (const UInt8 *) thePtr;
		dstPtr    = &theValue->at(theRange.GetLocation());
		blockSize = theSize;
		memcpy(dstPtr, srcPtr, blockSize);
		}


	// Insert the block into a larger range
	else
		{
		srcPtr    = (const UInt8 *) thePtr;
		dstPtr    = &theValue->at(theRange.GetLocation());
		blockSize = theSize;
		memcpy(dstPtr, srcPtr, blockSize);

		srcPtr    = &theValue->at(theRange.GetLocation() + theRange.GetSize());
		dstPtr    = &theValue->at(theRange.GetLocation() + theSize);
		blockSize = oldSize - theRange.GetNext();
		memmove(dstPtr, srcPtr, blockSize);

		sizeDelta = theRange.GetSize() - theSize;
		theValue->resize(oldSize - sizeDelta);
		}



	// Get the data
	dstPtr = &theValue->at(theRange.GetLocation());

	return(dstPtr);
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
//		NData::+= : Append data.
//----------------------------------------------------------------------------
const NData& NData::operator += (const NData &theData)
{


	// Append the data
	AppendData(theData);
	
	return(*this);
}





//============================================================================
//		NData::+ : Append data.
//----------------------------------------------------------------------------
const NData NData::operator + (const NData &theData) const
{	NData	theResult;



	// Append the data
	theResult  = *this;
	theResult += theData;
	
	return(theResult);
}





//============================================================================
//		NData::GetMutable : Get the mutable value.
//----------------------------------------------------------------------------
#pragma mark -
NDataValue *NData::GetMutable(void)
{	NDataValue		*theValue;



	// Get the state we need
	theValue = NSharedValueData::GetMutable();



	// Copy external data
	//
	// External data can be supported indefinitely for immutable access,
	// but must be copied whenever mutable access is required.
	if (mExternalSize != 0)
		{
		theValue->resize(mExternalSize);
		memcpy(&theValue->at(0), mExternalPtr, mExternalSize);

		mExternalSize = 0;
		mExternalPtr  = NULL;
		}

	return(theValue);
}





//============================================================================
//		NData::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
const NDataValue *NData::GetNullValue(void) const
{	static NDataValue		sNullValue;



	// Get the value
	return(&sNullValue);
}





