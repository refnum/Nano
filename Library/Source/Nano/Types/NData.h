/*	NAME:
		NData.h

	DESCRIPTION:
		Data object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATA_HDR
#define NDATA_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NSharedValue.h"
#include "NComparable.h"
#include "NDebuggable.h"
#include "NEncodable.h"
#include "NContainer.h"
#include "NHashable.h"
#include "NRange.h"





//============================================================================
//      Types
//----------------------------------------------------------------------------
class NData;

typedef std::vector<uint8_t>										NDataValue;
typedef NDataValue::iterator										NDataValueIterator;
typedef NDataValue::const_iterator									NDataValueConstIterator;

typedef std::vector<NData>											NDataList;
typedef NDataList::iterator											NDataListIterator;
typedef NDataList::const_iterator									NDataListConstIterator;

typedef NSharedValue<NDataValue>									NSharedValueData;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NData :	public NContainer,
				public NHashable,
				public NEncodable,
				public NDebuggable,
				public NComparable<NData>,
				public NSharedValueData {
public:
										NENCODABLE_DECLARE(NData);

										NData(const NData &theValue);
										NData(NIndex theSize, const void *thePtr=NULL, bool makeCopy=true);

										NData(void);
	virtual							   ~NData(void);


	// Clear the value
	void								Clear(void);


	// Get/set the size
	//
	// Increasing the size will zero-fill the new content.
	NIndex								GetSize(void) const;
	bool								SetSize(NIndex theSize);


	// Reserve additional space
	//
	// Reserving space may pre-allocate internal storage to amortize future resizes.
	void								Reserve(NIndex theSize);


	// Get the data
	//
	// Returns a NULL pointer if the object is empty.
	NData								GetData(const NRange &theRange) const;
	const uint8_t					   *GetData(NIndex theOffset=0)     const;
	uint8_t							   *GetData(NIndex theOffset=0);


	// Set the data
	//
	// A NULL source pointer is treated as a zero-filled block of the appropriate size.
	void								SetData(NIndex theSize, const void *thePtr, bool makeCopy=true);


	// Insert data
	//
	// Inserts data immediately before the specified index, or at the end of the value
	// if the index is equal to our current size.
	//
	// A NULL source pointer is treated as a zero-filled block of the appropriate size.
	//
	// Returns a pointer to the newly-inserted data, or NULL if no data was appended.
	uint8_t							   *InsertData(NIndex beforeIndex, const NData &theData);
	uint8_t							   *InsertData(NIndex beforeIndex, NIndex theSize, const void *thePtr);


	// Remove data
	void								RemoveData(const NRange &theRange);


	// Append data
	//
	// A NULL source pointer is treated as a zero-filled block of the appropriate size.
	//
	// Returns a pointer to the newly-appended data, or NULL if no data was appended.
	uint8_t							   *AppendData(const NData &theData);
	uint8_t							   *AppendData(      NIndex theSize, const void *thePtr=NULL);


	// Replace data
	//
	// A NULL source pointer is treated as a zero-filled block of the appropriate size.
	//
	// Returns a pointer to the newly-modified data.
	uint8_t							   *ReplaceData(const NRange &theRange, const NData &theData);
	uint8_t							   *ReplaceData(const NRange &theRange,       NIndex theSize, const void *thePtr);


	// Compare the value
	NComparison							Compare(const NData &theValue) const;


	// Operators
	const NData&						operator =  (const NData &theValue);
	const NData&						operator += (const NData &theValue);
	const NData							operator +	(const NData &theValue) const;


	// Operators
										operator NFormatArgument(void) const;


protected:
	// Get the mutable value
	NDataValue						   *GetMutable(void);


	// Get the null value
	const NDataValue				   *GetNullValue(void) const;


	// Calculate the hash
	NHashCode							CalculateHash(void) const;


	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	void								ValueChanged(NDataValue *theValue);

	bool								IsValidSlice(void) const;
	void								ResizeValue(NDataValue *theValue, NIndex theSize);


private:
	NRange								mSlice;
	NIndex								mExternalSize;
	const void						   *mExternalPtr;
};





#endif // NDATA_HDR


