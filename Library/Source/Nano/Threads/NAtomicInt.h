/*	NAME:
		NAtomicInt.h

	DESCRIPTION:
		Atomic integer.
	
	COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
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
class NAtomicInt : public NComparable<SInt32> {
public:
										NAtomicInt(SInt32 theValue=0);
	virtual							   ~NAtomicInt(void);


	// Compare the value
	NComparison							Compare(const SInt32 &theValue) const;


	// Increment/decrement the value
	//
	// Returns the new value after the increment/decrement operation.
	SInt32								Increment(SInt32 theDelta=1);
	SInt32								Decrement(SInt32 theDelta=1);


	// Operators
	const NAtomicInt&					operator =(const NAtomicInt &theObject);

	const NAtomicInt&					operator +=(const NAtomicInt &theObject);
	const NAtomicInt&					operator -=(const NAtomicInt &theObject);

	const NAtomicInt&					operator ++(void);
	const NAtomicInt&					operator --(void);

	NAtomicInt							operator ++(int);
	NAtomicInt							operator --(int);

	bool								operator == (int theValue) const;
	bool								operator != (int theValue) const;
	bool								operator <= (int theValue) const;
	bool								operator <	(int theValue) const;
	bool								operator >= (int theValue) const;
	bool								operator >	(int theValue) const;

										operator SInt32(void) const;


private:
	mutable SInt32						mValue;
};




#endif // NATOMICINT_HDR


