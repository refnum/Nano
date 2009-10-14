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
#include "NCFObject.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NData : public NCFObject {
public:
										NData(CFDataRef cfData);
										
										NData(void);
	virtual								~NData(void);


	// Is the array empty?
	bool								IsEmpty(   void) const;
	bool								IsNotEmpty(void) const;


	// Get/set the size
	//
    // Changing the size will invalidate any previously returned pointers. If the
	// size is increased, the new space will be zero-filled.
	UInt32								GetSize(void) const;
	bool								SetSize(UInt32 theSize);


	// Clear the data
	void								Clear(void);
	

	// Get/set the data
	const UInt8							*GetData(UInt32 theOffset=0) const;
	UInt8								*GetData(UInt32 theOffset=0);
	
	bool								SetData(UInt32 theSize, const void *thePtr);


	// Append data
	//
    // Appending to the buffer will invalidate any previously returned pointers. If
	// no data is supplied, the new space will be zero-filled.
	//
	// Returns a pointer to the newly-appended data within the buffer.
	UInt8								*AppendData(UInt32 theSize, const void *thePtr=NULL);


	// Replace data
	//
	// Returns a pointer to the newly-replaced data within the buffer.
	UInt8								*ReplaceData(const CFRange &theRange, UInt32 theSize, const void *thePtr);


	// Remove data
	void								RemoveData(const CFRange &theRange);


protected:
	// Get the null value
	CFTypeRef							GetNullValue(void);


private:
	void								InitializeSelf(CFDataRef cfData);


private:


};





#endif // NDATA_HDR


