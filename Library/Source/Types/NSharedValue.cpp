/*	NAME:
		NSharedValue.cpp

	DESCRIPTION:
		Base class for shared-value objects.

		A "shared value" object uses copy-on-write to share an immutable value
		between objects, creating a new copy of the data when mutable access
		is requested.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Linker stub
//----------------------------------------------------------------------------
#ifndef NSHAREDVALUE_CPP

void SuppressNoCodeLinkerWarning_NSharedValue(void);
void SuppressNoCodeLinkerWarning_NSharedValue(void)
{
}

#else





//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSharedValue.h"





//============================================================================
//		NSharedValue::NSharedValue : Constructor.
//----------------------------------------------------------------------------
template<class T> NSharedValue<T>::NSharedValue(const NSharedValue<T> &theValue)
{


	// Initialise ourselves
	mValue    = NULL;
	mRefCount = NULL;

	AcquireValue(theValue);
}





//============================================================================
//		NSharedValue::NSharedValue : Constructor.
//----------------------------------------------------------------------------
template<class T> NSharedValue<T>::NSharedValue(void)
{


	// Initialise ourselves
	mValue    = NULL;
	mRefCount = NULL;
}





//============================================================================
//		NSharedValue::~NSharedValue : Destructor.
//----------------------------------------------------------------------------
template<class T> NSharedValue<T>::~NSharedValue(void)
{


	// Clean up
	ReleaseValue();
}





//============================================================================
//		NSharedValue::Clear : Clear the value.
//----------------------------------------------------------------------------
template<class T> void NSharedValue<T>::Clear(void)
{


	// Clear the value
	ReleaseValue();
}





//============================================================================
//		NSharedValue::= : Assignment operator.
//----------------------------------------------------------------------------
template<class T> const NSharedValue<T>& NSharedValue<T>::operator = (const NSharedValue<T> &theValue)
{


	// Assign the object
	if (this != &theValue)
		AcquireValue(theValue);

	return(*this);
}





//============================================================================
//		NSharedValue::GetImmutable : Get the immutable value.
//----------------------------------------------------------------------------
template<class T> const T *NSharedValue<T>::GetImmutable(void) const
{


	// Get the value
	//
	// To defer value creation as long as possible, we try and handle
	// immutable access with the shared immutable null value.
	if (mValue == NULL)
		return(GetNullValue());

	return(mValue);
}





//============================================================================
//		NSharedValue::GetMutable : Get the mutable value.
//----------------------------------------------------------------------------
template<class T> T *NSharedValue<T>::GetMutable(void)
{	T	*newValue;



	// Create the value
	//
	// Values are created on demand, to allow us to use the shared
	// null value to handle immutable access for as long as possible.
	if (mValue == NULL)
		{
		mValue    = new T(*GetNullValue());
		mRefCount = new NAtomicInt(1);

		return(mValue);
		}



	// Copy the value
	//
	// If someone else is using the value, we need to take a copy.
	//
	// This does not need a lock, since we also hold a reference to the value
	// so we know it can't be destroyed/mutated before we release that reference.
	if (*mRefCount != 1)
		{
		newValue = new T(*mValue);
		ReleaseValue();

		mValue    = newValue;
		mRefCount = new NAtomicInt(1);
		}

	return(mValue);
}





//============================================================================
//		NSharedValue::AcquireValue  Acquire a value.
//----------------------------------------------------------------------------
template<class T> void NSharedValue<T>::AcquireValue(const NSharedValue<T> &theValue)
{


	// Acquire the value
	if (theValue.mRefCount != NULL)
		theValue.mRefCount->Increment();



	// Replace our value
	ReleaseValue();

	mValue    = theValue.mValue;
	mRefCount = theValue.mRefCount;
}





//============================================================================
//		NSharedValue::ReleaseValue : Release the value.
//----------------------------------------------------------------------------
template<class T> void NSharedValue<T>::ReleaseValue(void)
{


	// Release the value
	if (mRefCount != NULL)
		{
		if (mRefCount->Decrement() == 0)
			{
			delete mValue;
			delete mRefCount;
			}
		}



	// Reset our state
	mValue    = NULL;
	mRefCount = NULL;
}

#endif // NSHAREDVALUE_CPP

