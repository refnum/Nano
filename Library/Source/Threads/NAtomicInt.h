/*	NAME:
		NAtomicInt.h

	DESCRIPTION:
		Atomic integer.
	
	COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NATOMICINT_HDR
#define NATOMICINT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NComparable.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NAtomicInt {
public:
										 NAtomicInt(SInt32 theValue=0);
	virtual								~NAtomicInt(void);


	// Increment/decrement the value
	SInt32								Increment(SInt32 theDelta=1);
	SInt32								Decrement(SInt32 theDelta=1);


	// Operators
	const NAtomicInt&					operator =(const NAtomicInt &theObject);

	const NAtomicInt&					operator +=(const NAtomicInt &theObject);
	const NAtomicInt&					operator -=(const NAtomicInt &theObject);

	const NAtomicInt&					operator ++(void);
	const NAtomicInt&					operator --(void);

										operator SInt32(void) const;


protected:
	// Compare two objects
	NComparison							Compare(const NComparable &theObject) const;


private:
    SInt32								mValue;
};




#endif // NATOMICINT_HDR


