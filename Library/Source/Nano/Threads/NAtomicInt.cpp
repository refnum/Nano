/*	NAME:
		NAtomicInt.cpp

	DESCRIPTION:
		Atomic integer.
	
	COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThreadUtilities.h"
#include "NAtomicInt.h"





//============================================================================
//		NAtomicInt::NAtomicInt : Constructor.
//----------------------------------------------------------------------------
NAtomicInt::NAtomicInt(SInt32 theValue)
{


	// Validate our state
	//
	// We assume that read/writes from a 4-byte aligned 32-bit integer are atomic.
	NN_ASSERT((((UInt64) &mValue) % 4) == 0);



	// Initialize ourselves
	mValue = theValue;
}





//============================================================================
//		NAtomicInt::~NAtomicInt : Destructor.
//----------------------------------------------------------------------------
NAtomicInt::~NAtomicInt(void)
{
}





//============================================================================
//		NAtomicInt::Compare : Compare the value.
//----------------------------------------------------------------------------
#pragma mark -
NComparison NAtomicInt::Compare(const SInt32 &theValue) const
{	volatile SInt32		myValue;



	// Compare the value
	//
	// The value is copied so the comparison is atomic.
	myValue = mValue;

	return(GetComparison(myValue, theValue));
}





//============================================================================
//		NAtomicInt::Increment : Increment the value.
//----------------------------------------------------------------------------
SInt32 NAtomicInt::Increment(SInt32 theDelta)
{


	// Adjust the value
	NThreadUtilities::AtomicAdd32(mValue, theDelta);

	return(mValue);
}





//============================================================================
//		NAtomicInt::Decrement : Decrement the value.
//----------------------------------------------------------------------------
SInt32 NAtomicInt::Decrement(SInt32 theDelta)
{


	// Adjust the value
	NThreadUtilities::AtomicAdd32(mValue, -theDelta);

	return(mValue);
}





//============================================================================
//		NAtomicInt::= : Assignment operator.
//----------------------------------------------------------------------------
const NAtomicInt& NAtomicInt::operator = (const NAtomicInt &theObject)
{


	// Assign the object
	if (this != &theObject)
		mValue = theObject.mValue;

	return(*this);
}





//============================================================================
//		NAtomicInt::+= : Addition operator.
//----------------------------------------------------------------------------
const NAtomicInt& NAtomicInt::operator += (const NAtomicInt &theObject)
{


	// Adjust the value
	Increment(theObject.mValue);

	return(*this);
}





//============================================================================
//		NAtomicInt::-= : Subtraction operator.
//----------------------------------------------------------------------------
const NAtomicInt& NAtomicInt::operator -= (const NAtomicInt &theObject)
{


	// Adjust the value
	Decrement(theObject.mValue);

	return(*this);
}





//============================================================================
//		NAtomicInt::++ : Prefix increment operator.
//----------------------------------------------------------------------------
const NAtomicInt& NAtomicInt::operator ++ (void)
{


	// Adjust the value
	Increment();

	return(*this);
}





//============================================================================
//		NAtomicInt::-- : Prefix decrement operator.
//----------------------------------------------------------------------------
const NAtomicInt& NAtomicInt::operator -- (void)
{


	// Adjust the value
	Decrement();

	return(*this);
}





//============================================================================
//		NAtomicInt::++ : Postfix increment operator.
//----------------------------------------------------------------------------
NAtomicInt NAtomicInt::operator ++ (int)
{	NAtomicInt		prevValue(mValue);



	// Adjust the value
	Increment();

	return(prevValue);
}





//============================================================================
//		NAtomicInt::-- : Postfix decrement operator.
//----------------------------------------------------------------------------
NAtomicInt NAtomicInt::operator -- (int)
{	NAtomicInt		prevValue(mValue);



	// Adjust the value
	Decrement();

	return(prevValue);
}





//============================================================================
//		NAtomicInt::== : Equality operator
//----------------------------------------------------------------------------
bool NAtomicInt::operator == (int theValue) const
{


	// Compare the objects
	return(Compare(theValue) == kNCompareEqualTo);
}





//============================================================================
//		NAtomicInt::!= : Inequality operator.
//----------------------------------------------------------------------------
bool NAtomicInt::operator != (int theValue) const
{


	// Compare the objects
	return(Compare(theValue) != kNCompareEqualTo);
}





//============================================================================
//		NAtomicInt::<= : Comparison operator.
//----------------------------------------------------------------------------
bool NAtomicInt::operator <= (int theValue) const
{


	// Compare the objects
	return(Compare(theValue) != kNCompareGreaterThan);
}





//============================================================================
//		NAtomicInt::< : Comparison operator.
//----------------------------------------------------------------------------
bool NAtomicInt::operator < (int theValue) const
{


	// Compare the objects
	return(Compare(theValue) == kNCompareLessThan);
}





//============================================================================
//		NAtomicInt::>= : Comparison operator.
//----------------------------------------------------------------------------
bool NAtomicInt::operator >= (int theValue) const
{


	// Compare the objects
	return(Compare(theValue) != kNCompareLessThan);
}





//============================================================================
//		NAtomicInt::> : Comparison operator.
//----------------------------------------------------------------------------
bool NAtomicInt::operator > (int theValue) const
{


	// Compare the objects
	return(Compare(theValue) == kNCompareGreaterThan);
}





//============================================================================
//		NAtomicInt::SInt32 : SInt32 operator.
//----------------------------------------------------------------------------
NAtomicInt::operator SInt32(void) const
{


	// Get the value
	return(mValue);
}



