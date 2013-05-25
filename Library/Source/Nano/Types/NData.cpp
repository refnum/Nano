/*	NAME:
		NData.cpp

	DESCRIPTION:
		Data object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDataDigest.h"
#include "NEncoder.h"
#include "NData.h"





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NData);





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
NData::NData(const NData &theValue)
		: NContainer(), NHashable(), NEncodable(), NComparable<NData>(), NSharedValueData(theValue)
{


	// Initialize ourselves
	mExternalSize = theValue.mExternalSize;
	mExternalPtr  = theValue.mExternalPtr;
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
	ClearHash();

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
		theSize  = (NIndex) theValue->size();
		}

	return(theSize);
}





//============================================================================
//		NData::SetSize : Set the size.
//----------------------------------------------------------------------------
bool NData::SetSize(NIndex theSize)
{	NDataValue		*theValue;
	bool			didSet;



	// Validate our parameters
	NN_ASSERT(theSize >= 0);



	// Check our state
	if (theSize == GetSize())
		return(true);



	// Set the size
	if (theSize == 0)
		Clear();
	else
		{
		// Keep using an external buffer
		//
		// We can't grow an external buffer, so must take a copy.
		if (mExternalSize != 0 && theSize <= mExternalSize)
			mExternalSize = theSize;


		// Or resize our own
		else
			{
			theValue = GetMutable();
			Resize(theValue, theSize);
			}
		}
	
	ClearHash();



	// Check the size
	didSet = (GetSize() == theSize);
	return(didSet);
}





//============================================================================
//		NData::Reserve : Reserve additional space.
//----------------------------------------------------------------------------
void NData::Reserve(NIndex theSize)
{	NDataValue		*theValue;



	// Reserve the space
	theValue = GetMutable();
	theValue->reserve(theSize);
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



	// Get the data
	if (IsEmpty())
		thePtr = NULL;
	
	else
		{
		theValue = GetMutable();
		thePtr   = &theValue->at(theOffset);
		}



	// Update our state
	ClearHash();

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
		Resize(theValue, theSize);

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



	// Update our state
	ClearHash();
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
	oldSize  = (NIndex) theValue->size();



	// Append the data
	Resize(theValue, theSize + oldSize);

	dstPtr = &theValue->at(oldSize);
	if (thePtr != NULL)
		memcpy(dstPtr, thePtr, theSize);



	// Update our state
	ClearHash();

	return(dstPtr);
}





//============================================================================
//		NData::AppendData : Append data.
//----------------------------------------------------------------------------
UInt8 *NData::AppendData(const NData &theData)
{	NDataValue			*dstValue;
	const NDataValue	*srcValue;
	NIndex				oldSize;
	UInt8				*thePtr;



	// Check our parameters
	if (theData.IsEmpty())
		return(NULL);



	// Append to empty
	//
	// If we're empty then we can share the value, rather than appending bytes.
	if (IsEmpty())
		{
		*this  = theData;
		thePtr = GetData();
		}
	
	
	// Append to existing
	//
	// Since we're working with two NDataValues, we can insert the new values
	// directly into the end of our vector to avoid resizing and initialising
	// the new space (and then overwriting it with the new data).
	else
		{
		srcValue = theData.GetImmutable();
		dstValue = GetMutable();
		oldSize  = GetSize();

		dstValue->insert(dstValue->end(), srcValue->begin(), srcValue->end());
		thePtr = &dstValue->at(oldSize);
		}



	// Update our state
	ClearHash();
	
	return(thePtr);
}





//============================================================================
//		NData::RemoveData : Remove data.
//----------------------------------------------------------------------------
void NData::RemoveData(NIndex theSize, bool fromFront)
{	NRange		theRange;



	// Validate our parameters
	NN_ASSERT(theSize > 0 && theSize <= GetSize());



	// Remove all data
	if (theSize == GetSize())
		Clear();



	// Remove a range
	else
		{
		if (fromFront)
			theRange = NRange(0, theSize);
		else
			theRange = NRange(GetSize() - theSize, theSize);

		RemoveData(theRange);
		}
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
	ClearHash();
}





//============================================================================
//		NData::ReplaceData : Replace data.
//----------------------------------------------------------------------------
UInt8 *NData::ReplaceData(const NRange &theRange, NIndex theSize, const void *thePtr)
{	NIndex			oldSize, blockSize, sizeDelta;
	NRange			replaceRange;
	NDataValue		*theValue;
	UInt8			*dstPtr;
	const UInt8		*srcPtr;



	// Validate our parameters
	NN_ASSERT(theSize >= 0);



	// Get the state we need
	theValue = GetMutable();
	oldSize  = (NIndex) theValue->size();
	
	if (theRange == kNRangeNone)
		replaceRange = NRange(0, 0);
	else
		replaceRange = theRange.GetNormalized(GetSize());



	// Insert the block into a smaller range
	if (theSize >= replaceRange.GetSize())
		{
		sizeDelta = theSize - replaceRange.GetSize();
		Resize(theValue, oldSize + sizeDelta);

		srcPtr    = &theValue->at(replaceRange.GetLocation() + replaceRange.GetSize());
		dstPtr    = &theValue->at(replaceRange.GetLocation() + theSize);
		blockSize = oldSize - replaceRange.GetNext();
		memmove(dstPtr, srcPtr, blockSize);

		srcPtr    = (const UInt8 *) thePtr;
		dstPtr    = &theValue->at(replaceRange.GetLocation());
		blockSize = theSize;
		memcpy(dstPtr, srcPtr, blockSize);
		}


	// Insert the block into a larger range
	else
		{
		srcPtr    = (const UInt8 *) thePtr;
		dstPtr    = &theValue->at(replaceRange.GetLocation());
		blockSize = theSize;
		memcpy(dstPtr, srcPtr, blockSize);

		srcPtr    = &theValue->at(replaceRange.GetLocation() + replaceRange.GetSize());
		dstPtr    = &theValue->at(replaceRange.GetLocation() + theSize);
		blockSize = oldSize - replaceRange.GetNext();
		memmove(dstPtr, srcPtr, blockSize);

		sizeDelta = replaceRange.GetSize() - theSize;
		Resize(theValue, oldSize - sizeDelta);
		}

	ClearHash();



	// Get the data
	dstPtr = &theValue->at(replaceRange.GetLocation());

	return(dstPtr);
}





//============================================================================
//		NData::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NData::Compare(const NData &theValue) const
{	const void				*ourPtr, *otherPtr;
	NIndex					ourSize, otherSize;
	NHashCode				ourHash, otherHash;
	NComparison				theResult;



	// Get the state we need
	ourPtr   = (         mExternalPtr != NULL) ?          mExternalPtr  :          GetData();
	otherPtr = (theValue.mExternalPtr != NULL) ? theValue.mExternalPtr  : theValue.GetData();

	ourSize   = (         mExternalPtr != NULL) ?          mExternalSize :          GetSize();
	otherSize = (theValue.mExternalPtr != NULL) ? theValue.mExternalSize : theValue.GetSize();

	ourHash   =          GetHash();
	otherHash = theValue.GetHash();



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	if (ourHash != otherHash)
		theResult = GetComparison(ourHash, otherHash);
	else
		theResult = CompareData(ourSize, ourPtr, otherSize, otherPtr);

	return(theResult);
}





//============================================================================
//		NData::= : Assignment operator.
//----------------------------------------------------------------------------
const NData& NData::operator = (const NData &theValue)
{


	// Assign the object
	if (this != &theValue)
		{
		NSharedValueData::operator=(theValue);
		ClearHash();

		mExternalSize = theValue.mExternalSize;
		mExternalPtr  = theValue.mExternalPtr;
		}

	return(*this);
}





//============================================================================
//		NData::+= : Concatenate operator.
//----------------------------------------------------------------------------
const NData& NData::operator += (const NData &theValue)
{


	// Append the data
	AppendData(theValue);
	
	return(*this);
}





//============================================================================
//		NData::+ : Catenate operator.
//----------------------------------------------------------------------------
const NData NData::operator + (const NData &theValue) const
{	NData	theResult;



	// Append the data
	theResult  = *this;
	theResult += theValue;
	
	return(theResult);
}





//============================================================================
//		NData::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NData::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{size=%ld}", GetSize());

	return(theResult);
}





#pragma mark protected
//============================================================================
//		NData::GetMutable : Get the mutable value.
//----------------------------------------------------------------------------
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
		Resize(theValue, mExternalSize);
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





//============================================================================
//		NData::CalculateHash : Calculate the hash.
//----------------------------------------------------------------------------
NHashCode NData::CalculateHash(void) const
{	NDataDigest		theDigest;



	// Get the hash code
	return((NHashCode) theDigest.GetAdler32(*this));
}





//============================================================================
//      NData::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NData::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeData(kNEncoderValueKey, *this);
}





//============================================================================
//      NData::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NData::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	*this = theEncoder.DecodeData(kNEncoderValueKey);
}





#pragma mark private
//============================================================================
//      NData::Resize : Resize the value.
//----------------------------------------------------------------------------
void NData::Resize(NDataValue *theValue, NIndex theSize)
{


	// Resize the value
	//
	// Reserving the space we need before the resize avoids excessive
	// reallocation if resize is implemented in terms of insert().
	if (theSize >= (NIndex) theValue->capacity())
		theValue->reserve(theSize);

	theValue->resize(theSize, 0x00);
}



