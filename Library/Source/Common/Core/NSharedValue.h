/*	NAME:
		NSharedValue.h

	DESCRIPTION:
		Base class for shared-value objects.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSHAREDVALUE_HDR
#define NSHAREDVALUE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAtomicInt.h"
#include "NComparable.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
template<class T> class NSharedValue : public NComparable {
public:
										 NSharedValue(const NSharedValue<T> &theValue);

										 NSharedValue(void);
	virtual								~NSharedValue(void);


	// Clear the value
	virtual void						Clear(void);


	// Operators
	const NSharedValue<T>&				operator = (const NSharedValue<T> &theValue);


protected:
	// Get the value
	const T								*GetImmutable(void) const;
	virtual T							*GetMutable(  void);


	// Get the null value
	//
	// The "null value" is the immutable value assigned to an object
	// when it is newly created, or cleared.
	//
	// The same null value must be returned for all instances of the
	// object (e.g., by returning a global or local static).
	virtual const T						*GetNullValue(void) const = 0;


	// Compare two objects
	virtual NComparison					Compare(const NComparable &theObject) const;


private:
	void								AcquireValue(const NSharedValue<T> &theValue);
	void								ReleaseValue(void);


private:
	T									*mValue;
	NAtomicInt							*mRefCount;
};





//============================================================================
//		Class definition
//----------------------------------------------------------------------------
#define   NSHAREDVALUE_CPP
#include "NSharedValue.cpp"
#undef    NSHAREDVALUE_CPP





#endif // NSHAREDVALUE_HDR
