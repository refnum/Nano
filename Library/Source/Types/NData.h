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
#include "NContainer.h"
#include "NSharedValue.h"
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
										 NData(NIndex theSize, const void *thePtr, bool makeCopy=true);

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
	const UInt8							*GetData(NIndex theOffset=0) const;
	UInt8								*GetData(NIndex theOffset=0);
	
	void								SetData(NIndex theSize, const void *thePtr, bool makeCopy=true);


	// Append data
	//
    // Appending to the buffer will invalidate any previously returned
	// pointers. If no data is supplied, the new space is zero-filled.
	//
	// Returns a pointer to the newly-appended data within the buffer,
	// or NULL if no data was appended.
	UInt8								*AppendData(NIndex theSize, const void *thePtr=NULL);


	// Replace data
	//
	// Returns a pointer to the newly-replaced data within the buffer.
	UInt8								*ReplaceData(const NRange &theRange, NIndex theSize, const void *thePtr);


	// Remove data
    void                                RemoveData(NIndex theSize, bool fromFront=true);
	void								RemoveData(const NRange &theRange);


	// Compare the value
	NComparison							Compare(const NData &theValue) const;


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


