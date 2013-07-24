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
class NAtomicInt : public NComparable<int32_t> {
public:
										NAtomicInt(int32_t theValue=0);
	virtual							   ~NAtomicInt(void);


	// Compare the value
	NComparison							Compare(const int32_t &theValue) const;


	// Increment/decrement the value
	//
	// Returns the new value after the increment/decrement operation.
	int32_t								Increment(int32_t theDelta=1);
	int32_t								Decrement(int32_t theDelta=1);


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

										operator int32_t(void) const;


private:
	mutable int32_t						mValue;
};




#endif // NATOMICINT_HDR


