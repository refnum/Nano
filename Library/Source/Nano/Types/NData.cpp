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
NData::NData(const NData &theValue)
		: NSharedValueData(theValue)
{


	// Initialize ourselves
	mSlice        = theValue.mSlice;
	mExternalSize = theValue.mExternalSize;
	mExternalPtr  = theValue.mExternalPtr;
}





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

	ValueChanged(NULL);



	// Assign the data
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

	ValueChanged(NULL);
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

	mSlice        = NRange(0, 0);
	mExternalSize = 0;
	mExternalPtr  = NULL;

	ValueChanged(NULL);
}





//============================================================================
//		NData::GetSize : Get the size.
//----------------------------------------------------------------------------
NIndex NData::GetSize(void) const
{


	// Get the size
	return(mSlice.GetSize());
}





//============================================================================
//		NData::SetSize : Set the size.
//----------------------------------------------------------------------------
bool NData::SetSize(NIndex theSize)
{	NIndex			oldSize, newSize;
	NDataValue		*theValue;



	// Validate our parameters
	NN_ASSERT(theSize >= 0);



	// Get the state we need
	oldSize = GetSize();



	// Resize to 0
	//
	// Clearing, rather than resizing, allows us to share the null value.
	if (theSize == 0)
		Clear();



	// Resize to smaller
	//
	// If we're shrinking then we can leave the data alone (external or not)
	// and just reduce our slice.
	//
	// This means that allocating a large block then reducing its size will
	// leave the previous allocation around, possibly indefinitely.
	//
	// But given that the slice can be turned into an exact block by requesting
	// a mutable pointer this seems a reasonable trade-off given that it lets
	// us continue to share data when shrinking.
	else if (theSize <= GetSize())
		mSlice.SetSize(theSize);
	
	
	
	// Resize to larger
	else
		{
		theValue = GetMutable();
		ResizeValue(theValue, theSize);
		}



	// Check our state
	NN_ASSERT(IsValidSlice());

	newSize = GetSize();

	if (newSize != oldSize)
		ValueChanged(NULL);
	
	return(newSize == theSize);
}





//============================================================================
//		NData::Reserve : Reserve additional space.
//----------------------------------------------------------------------------
void NData::Reserve(NIndex theSize)
{	NDataValue		*theValue;



	// Reserve the space
	theValue = GetMutable();
	theValue->reserve(theSize);



	// Check our state
	NN_ASSERT(IsValidSlice());
}





//============================================================================
//		NData::GetData : Get the data.
//----------------------------------------------------------------------------
NData NData::GetData(const NRange &theRange) const
{	NRange		finalRange;
	NData		theData;



	// Get the state we need
	finalRange = GetNormalized(theRange);



	// Get the data
	//
	// The new data can use a slice of our data.
	//
	// As we may also be using a slice, the new range is relative to ours.
	if (!finalRange.IsEmpty())
		{
		theData = *this;

		theData.mSlice.SetLocation(mSlice.GetOffset(finalRange.GetLocation()));
		theData.mSlice.SetSize(                     finalRange.GetSize());

		NN_ASSERT(theData.IsValidSlice());
		}

	return(theData);
}





//============================================================================
//		NData::GetData : Get the data.
//----------------------------------------------------------------------------
const uint8_t *NData::GetData(NIndex theOffset) const
{	const NDataValue		*theValue;
	const uint8_t			*thePtr;



	// Validate our parameters
#if NN_DEBUG
	if (IsEmpty())
		NN_ASSERT(theOffset == 0);
	else
		NN_ASSERT(mSlice.Contains(mSlice.GetOffset(theOffset)));
#endif



	// Get empty data
	if (IsEmpty())
		thePtr = NULL;



	// Get immutable data
	//
	// The offset is relative to our slice.
	else
		{
		if (mExternalPtr != NULL)
			thePtr = (const uint8_t *) mExternalPtr;
		else
			{
			theValue = GetImmutable();
			thePtr   = &theValue->at(0);
			}
		
		thePtr += mSlice.GetOffset(theOffset);
		}

	return(thePtr);
}





//============================================================================
//		NData::GetData : Get the data.
//----------------------------------------------------------------------------
uint8_t *NData::GetData(NIndex theOffset)
{	NDataValue		*theValue;
	uint8_t			*thePtr;



	// Validate our parameters
#if NN_DEBUG
	if (IsEmpty())
		NN_ASSERT(theOffset == 0);
	else
		NN_ASSERT(mSlice.Contains(mSlice.GetOffset(theOffset)));
#endif



	// Get empty data
	//
	// Although we have to return a mutable pointer, an empty object can just
	// return NULL rather than becoming mutable.
	if (IsEmpty())
		thePtr = NULL;



	// Get mutable data
	//
	// Obtaining mutable data will copy any external data we might have been
	// using, and resolve any slice we had of any shared data.
	//
	// As such we're left with a single slice onto our own data, anchored at
	// 0, so the offset can be used directly.
	else
		{
		theValue = GetMutable();
		thePtr   = &theValue->at(theOffset);

		NN_ASSERT(mSlice.GetLocation() == 0);
		}



	// Update our state
	ClearHash();
	
	return(thePtr);
}





//============================================================================
//		NData::SetData : Set the data.
//----------------------------------------------------------------------------
void NData::SetData(NIndex theSize, const void *thePtr, bool makeCopy)
{


	// Validate our parameters
	NN_ASSERT(theSize >= 0);

	if (NN_DEBUG && thePtr != NULL && !IsEmpty())
		NN_ASSERT(!(thePtr >= GetData(0) && thePtr < GetData(GetSize()-1)));



	// Reset our state
	if (!IsEmpty())
		Clear();



	// Set the data
	if (theSize != 0)
		{
		if (makeCopy || thePtr == NULL)
			InsertData(GetSize(), theSize, thePtr);
		else
			{
			mSlice        = NRange(0, theSize);
			mExternalSize = theSize;
			mExternalPtr  = thePtr;

			ValueChanged(NULL);
			}
		}



	// Check our state
	NN_ASSERT(IsValidSlice());
}





//============================================================================
//		NData::InsertData : Insert data.
//----------------------------------------------------------------------------
uint8_t *NData::InsertData(NIndex beforeIndex, const NData &theData)
{	NDataValueConstIterator		srcBegin, srcEnd;
	uint8_t						*resultPtr;
	NDataValueIterator			dstInsert;
	NDataValue					*dstValue;
	const NDataValue			*srcValue;



	// Validate our parameters
	NN_ASSERT(beforeIndex >= 0 && beforeIndex <= GetSize());



	// Check our parameters
	if (theData.IsEmpty())
		return(NULL);



	// Get the state we need
	srcValue = theData.GetImmutable();
	srcBegin = srcValue->begin() + theData.mSlice.GetFirst();
	srcEnd   = srcValue->begin() + theData.mSlice.GetNext();

	dstValue  = GetMutable();
	dstInsert = dstValue->begin() + beforeIndex;



	// Insert the data
	//
	// Inserting via iterators allows us to avoid manually resizing our
	// value, which saves a redundant initialisation of the new area.
	dstValue->insert(dstInsert, srcBegin, srcEnd);
	ValueChanged(dstValue);



	// Get the pointer
	resultPtr = &dstValue->at(beforeIndex);

	return(resultPtr);
}





//============================================================================
//		NData::InsertData : Insert data.
//----------------------------------------------------------------------------
uint8_t *NData::InsertData(NIndex beforeIndex, NIndex theSize, const void *thePtr)
{	uint8_t					*resultPtr;
	NDataValue				*dstValue;
	NDataValueIterator		dstInsert;



	// Validate our parameters
	NN_ASSERT(beforeIndex >= 0 && beforeIndex <= GetSize());



	// Check our parameters
	if (theSize == 0)
		return(NULL);



	// Get the state we need
	dstValue  = GetMutable();
	dstInsert = dstValue->begin() + beforeIndex;



	// Insert the data
	//
	// Since insert will zero-fill we can skip the copy if our source is NULL.
	dstValue->insert(dstInsert, theSize, 0x00);
	ValueChanged(dstValue);

	if (thePtr != NULL)
		{
		resultPtr = &dstValue->at(beforeIndex);
		memmove(resultPtr, thePtr, theSize);
		}



	// Get the pointer
	resultPtr = &dstValue->at(beforeIndex);

	return(resultPtr);
}





//============================================================================
//      NData::RemoveData : Remove data.
//----------------------------------------------------------------------------
void NData::RemoveData(const NRange &theRange)
{	NDataValueIterator		removeFirst, removeLast;
	NRange					finalRange;
	NDataValue				*theValue;



	// Get the range
	finalRange = GetNormalized(theRange);
	if (finalRange.IsEmpty())
		return;



	// Get the state we need
	theValue    = GetMutable();
	removeFirst = theValue->begin() + finalRange.GetFirst();
	removeLast  = theValue->begin() + finalRange.GetNext();



	// Remove the data
	theValue->erase(removeFirst, removeLast);
	ValueChanged(theValue);
}





//============================================================================
//		NData::AppendData : Append data.
//----------------------------------------------------------------------------
uint8_t *NData::AppendData(const NData &theData)
{


	// Append the data
	return(InsertData(GetSize(), theData));
}





//============================================================================
//		NData::AppendData : Append data.
//----------------------------------------------------------------------------
uint8_t *NData::AppendData(NIndex theSize, const void *thePtr)
{


	// Append the data
	return(InsertData(GetSize(), theSize, thePtr));
}





//============================================================================
//		NData::ReplaceData : Replace data.
//----------------------------------------------------------------------------
uint8_t *NData::ReplaceData(const NRange &theRange, const NData &theData)
{


	// Replace the data
	return(ReplaceData(theRange, theData.GetSize(), theData.GetData()));
}





//============================================================================
//		NData::ReplaceData : Replace data.
//----------------------------------------------------------------------------
uint8_t *NData::ReplaceData(const NRange &theRange, NIndex theSize, const void *thePtr)
{	NRange			finalRange;
	NIndex			sizeDelta;
	uint8_t			*dstPtr;



	// Validate our parameters
	NN_ASSERT(!GetNormalized(theRange).IsEmpty());
	NN_ASSERT(theSize >= 0);



	// Get the state we need
	finalRange = GetNormalized(theRange);
	sizeDelta  = theSize - finalRange.GetSize();



	// Replace everything
	//
	// Replacing everything can be done more efficiently as an assignment,
	// as it lets us switch to the null value if we're replacing with empty.
	if (IsFullRange(finalRange))
		SetData(theSize, thePtr);



	// Replace a range
	//
	// Inserting a larger range requires us to add a filler before we copy.
	//
	// This filler is placed at the end of the block we're going to overwrite,
	// to minimise the amount of data beyond it that needs to be shuffled up.
	else
		{
		// Resize the value
		if (sizeDelta > 0)
			InsertData(finalRange.GetNext(), sizeDelta, NULL);

		else if (sizeDelta < 0)
			RemoveData(NRange(finalRange.GetLocation(), -sizeDelta));



		// Replace the range
		dstPtr = GetData(finalRange.GetLocation());

		if (thePtr == NULL)
			memset( dstPtr,   0x00, theSize);
		else
			memmove(dstPtr, thePtr, theSize);
		}



	// Get the pointer
	dstPtr = GetData(finalRange.GetLocation());

	return(dstPtr);
}





//============================================================================
//		NData::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NData::Compare(const NData &theValue) const
{	NComparison	theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = GetComparison(GetHash(), theValue.GetHash());

	if (theResult == kNCompareEqualTo)
		theResult = CompareData(GetSize(), GetData(), theValue.GetSize(), theValue.GetData());

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

		mSlice        = theValue.mSlice;
		mExternalSize = theValue.mExternalSize;
		mExternalPtr  = theValue.mExternalPtr;
		
		ValueChanged(NULL);
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
	NIndex			sliceSize;



	// Get the state we need
	theValue  = NSharedValueData::GetMutable();
	sliceSize = mSlice.GetSize();



	// Resolve external data
	//
	// If we're using external data then we need to take a copy to be able
	// to return a mutable version.
	//
	// We may or may not have been using a slice of the external data, but
	// either way we'll end up with a slice that spans exactly what we own.
	if (mExternalPtr != NULL)
		{
		ResizeValue(theValue, sliceSize);
		memcpy(&theValue->at(0), mExternalPtr, sliceSize);
		}



	// Resolve any slice
	//
	// Mutable access breaks the ability to share data, so if we had a slice
	// of some larger data we resize to leave us with exactly what we own.
	else if (sliceSize != (NIndex) theValue->size())
		{
		NN_ASSERT(sliceSize < (NIndex) theValue->size());

		if (mSlice.GetLocation() != 0)
			memmove(&theValue->at(0), &theValue->at(mSlice.GetLocation()), sliceSize);

		ResizeValue(theValue, sliceSize);
		}



	// Update our state
	//
	// Mutable access always leaves us with our own copy of the data, and
	// a slice that spans the entire block of data.
	mSlice        = NRange(0, sliceSize);
	mExternalSize = 0;
	mExternalPtr  = NULL;

	NN_ASSERT(IsValidSlice());

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
//      NData::ValueChanged : Our value has been changed.
//----------------------------------------------------------------------------
void NData::ValueChanged(NDataValue *theValue)
{


	// Update the slice
	//
	// If we have a mutable value then we have the only copy of the data,
	// so by definition our slice spans the whole range.
	if (theValue != NULL)
		{
		mSlice = NRange(0, (NIndex) theValue->size());
		NN_ASSERT(IsValidSlice());
		}



	// Update the debug summary
#if NN_DEBUG
	UpdateSummary("%ld %s", mSlice.GetSize(), mSlice.GetSize() == 1 ? "byte" : "bytes");
#endif



	// Reset our state
	ClearHash();

	NN_ASSERT(IsValidSlice());
}





//============================================================================
//      NData::IsValidSlice : Is our slice valid?
//----------------------------------------------------------------------------
bool NData::IsValidSlice(void) const
{	const NDataValue		*theValue;
	bool					isValid;



	// External slice
	if (mExternalPtr != NULL)
		{
		NN_ASSERT(mExternalSize != 0);
		isValid = (mSlice.GetFirst() >= 0 && mSlice.GetLast() < mExternalSize);
		}
	
	
	// Internal slice
	else
		{
		NN_ASSERT(mExternalSize == 0);
		theValue = GetImmutable();

		if (theValue->empty())
			isValid = mSlice.IsEmpty();
		else
			isValid = (mSlice.GetFirst() >= 0 && mSlice.GetLast() < (NIndex) theValue->size());
		}

	return(isValid);
}





//============================================================================
//      NData::ResizeValue : Resize the value.
//----------------------------------------------------------------------------
void NData::ResizeValue(NDataValue *theValue, NIndex theSize)
{


	// Resize the value
	//
	// Reserving the space we need before the resize avoids excessive
	// reallocation if resize is implemented in terms of insert().
	if (theSize > (NIndex) theValue->capacity())
		theValue->reserve(theSize);

	theValue->resize(theSize, 0x00);



	// Update our state
	ValueChanged(theValue);
}




