/*	NAME:
		NData.h

	DESCRIPTION:
		Data object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATA_HDR
#define NDATA_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NContainer.h"
#include "NCFObject.h"





//============================================================================
//      Types
//----------------------------------------------------------------------------
class NData;

typedef std::vector<NData>											NDataList;
typedef NDataList::iterator											NDataListIterator;
typedef NDataList::const_iterator									NDataListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NData :	public NCFObject,
				public NContainer {
public:
										NData(CFDataRef cfData=NULL);
	virtual								~NData(void);


	// Get/set the size
	//
    // Changing the size will invalidate any previously returned pointers. If the
	// size is increased, the new space will be zero-filled.
	UInt32								GetSize(void) const;
	bool								SetSize(UInt32 theSize);
 	

	// Get/set the data
	const UInt8							*GetData(UInt32 theOffset=0) const;
	UInt8								*GetData(UInt32 theOffset=0);
	
	bool								SetData(UInt32 theSize, const void *thePtr);


	// Append data
	//
    // Appending to the buffer will invalidate any previously returned pointers.
	// If no data is supplied, the new space will be zero-filled.
	//
	// Returns a pointer to the newly-appended data within the buffer, or NULL
	// if no data was appended.
	UInt8								*AppendData(UInt32 theSize, const void *thePtr=NULL);


	// Replace data
	//
	// Returns a pointer to the newly-replaced data within the buffer.
	UInt8								*ReplaceData(const CFRange &theRange, UInt32 theSize, const void *thePtr);


	// Remove data
    void                                RemoveData(UInt32 theSize, bool fromFront=true);
	void								RemoveData(const CFRange &theRange);


protected:
	// Get the null value
	CFTypeRef							GetNullValue(void);
};





#endif // NDATA_HDR


