/*	NAME:
		NCoreFoundation.cpp

	DESCRIPTION:
		CoreFoundation support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreFoundation.h"
#include "NCocoa.h"





//============================================================================
//		Nano to CoreFoundation
//----------------------------------------------------------------------------
//		Note :	To allow us to return CFxxxRefs, we convert the object to the
//				equivalent toll-free bridged NSxxx form then cast.
//
//				Assuming an auto-release pool is in place, this allows us to
//				return raw pointers that can be retained by the caller if
//				needed but will automatically be released in the future.
//
//				If this proves problematic, e.g., we ever need to convert a
//				type which isn't toll-free bridged, we could convert to an
//				NCFObject then have our own deferred-release pool to release
//				the returned reference.
//----------------------------------------------------------------------------
CFArrayRef ToCF(const NArray &theArray)
{
	return((CFArrayRef) ToNS(theArray));
}

CFDataRef ToCF(const NData &theData)
{
	return((CFDataRef) ToNS(theData));
}

CFDateRef ToCF(const NDate &theDate)
{
	return((CFDateRef) ToNS(theDate));
}

CFDictionaryRef ToCF(const NDictionary &theDictionary)
{
	return((CFDictionaryRef) ToNS(theDictionary));
}

CFNumberRef ToCF(const NNumber &theNumber)
{
	return((CFNumberRef) ToNS(theNumber));
}

CFStringRef ToCF(const NString &theString)
{
	return((CFStringRef) ToNS(theString));
}

