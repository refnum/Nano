/*	NAME:
		NData.h

	DESCRIPTION:
		Data object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATA_HDR
#define NDATA_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSharedValue.h"
#include "NContainer.h"
#include "NRange.h"





//============================================================================
//      Types
//----------------------------------------------------------------------------
class NData;

typedef std::vector<UInt8>											NDataValue;
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
				public NComparable<NData>,
				public NSharedValueData {
public:
										 NData(NIndex theSize, const void *thePtr=NULL, bool makeCopy=true);

										 NData(void);
	virtual								~NData(void);


	// Clear the value
	void								Clear(void);


	// Get/set the size
	//
    // Changing the size will invalidate any previously returned pointers.
	// If the size is increased, the new space will be zero-filled.
	NIndex								GetSize(void) const;
	void								SetSize(NIndex theSize);
 	

	// Get/set the data
	//
	// GetData will return a NULL pointer if the data object is empty.
	//
	// SetData(xxx, NULL) is equivalent to Clear+AppendData(xxx);
	const UInt8							*GetData(NIndex theOffset=0) const;
	UInt8								*GetData(NIndex theOffset=0);

	NData								GetData(const NRange &theRange) const;
	
	void								SetData(NIndex theSize, const void *thePtr, bool makeCopy=true);


	// Append data
	//
    // Appending will invalidate any previously returned pointers. If no data is
	// supplied, the new space is zero-filled.
	//
	// Returns a pointer to the newly-appended data, or NULL if no data was appended.
	UInt8								*AppendData(NIndex theSize, const void *thePtr=NULL);
	UInt8								*AppendData(const NData &theData);


	// Remove data
    void                                RemoveData(NIndex theSize, bool fromFront=true);
	void								RemoveData(const NRange &theRange);


	// Replace data
	//
	// Returns a pointer to the modified data.
	UInt8								*ReplaceData(const NRange &theRange, NIndex theSize, const void *thePtr);


	// Compare the value
	NComparison							Compare(const NData &theValue) const;


	// Operators
	const NData&						operator += (const NData &theData);
	const NData							operator +	(const NData &theData) const;


protected:
	// Get the value
	NDataValue							*GetMutable(void);


	// Get the null value
	const NDataValue					*GetNullValue(void) const;


private:
	NIndex								mExternalSize;
	const void							*mExternalPtr;
};





#endif // NDATA_HDR


