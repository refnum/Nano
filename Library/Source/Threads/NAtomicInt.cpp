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
	NN_ASSERT((((UInt32) &mValue) % sizeof(mValue)) == 0);



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
//		NAtomicInt::Increment : Increment the value.
//----------------------------------------------------------------------------
SInt32 NAtomicInt::Increment(SInt32 theDelta)
{


	// Adjust the value
	NThreadUtilities::AtomicAdd32(mValue, theDelta);

	return(mValue);										// dair, make assignment atomic
}





//============================================================================
//		NAtomicInt::Decrement : Decrement the value.
//----------------------------------------------------------------------------
SInt32 NAtomicInt::Decrement(SInt32 theDelta)
{


	// Adjust the value
	NThreadUtilities::AtomicAdd32(mValue, -theDelta);

	return(mValue);										// dair, make assignment atomic
}





//============================================================================
//		NAtomicInt::= : Assignment operator.
//----------------------------------------------------------------------------
const NAtomicInt& NAtomicInt::operator = (const NAtomicInt &theObject)
{


	// Assign the object
	if (this != &theObject)
		mValue = theObject.mValue;		// dair, make atomic

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
	Increment(1);

	return(*this);
}





//============================================================================
//		NAtomicInt::-- : Prefix decrement operator.
//----------------------------------------------------------------------------
const NAtomicInt& NAtomicInt::operator -- (void)
{


	// Adjust the value
	Decrement(1);

	return(*this);
}





//============================================================================
//		NAtomicInt::++ : Postfix increment operator.
//----------------------------------------------------------------------------
NAtomicInt NAtomicInt::operator ++ (int)
{	NAtomicInt		prevValue(mValue);



	// Adjust the value
	Increment(1);

	return(prevValue);
}





//============================================================================
//		NAtomicInt::-- : Postfix decrement operator.
//----------------------------------------------------------------------------
NAtomicInt NAtomicInt::operator -- (int)
{	NAtomicInt		prevValue(mValue);



	// Adjust the value
	Decrement(1);

	return(prevValue);
}





//============================================================================
//		NAtomicInt::SInt32 : Cast operator.
//----------------------------------------------------------------------------
NAtomicInt::operator SInt32(void) const
{


	// Get the value
	return(mValue);
}





//============================================================================
//		NAtomicInt::Compare : Compare two objects.
//----------------------------------------------------------------------------
#pragma mark -
NComparison NAtomicInt::Compare(const NComparable &theObject) const
{	NAtomicInt		*otherInt = (NAtomicInt *) &theObject;
	SInt32			myValue, otherValue;



	// Compare the objects
	myValue    = mValue;										// dair, make atomic
	otherValue = otherInt->mValue;								// dair, make atomic

	return(GET_COMPARISON(myValue, otherValue));
}
