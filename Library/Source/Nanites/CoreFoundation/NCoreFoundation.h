/*	NAME:
		NCoreFoundation.h

	DESCRIPTION:
		CoreFoundation support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOREFOUNDATION_HDR
#define NCOREFOUNDATION_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRange.h"
#include "NCFArray.h"
#include "NCFData.h"
#include "NCFDate.h"
#include "NCFDictionary.h"
#include "NCFNumber.h"
#include "NCFString.h"
#include "NCFURL.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
extern const CFStringRef kNanoRunLoopMode;





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Nano to CoreFoundation
inline CFRange ToCF(const NRange &theRange)
{
	return(CFRangeMake(theRange.GetLocation(), theRange.GetSize()));
}

CFArrayRef		ToCF(const NArray		&theArray);
CFDataRef		ToCF(const NData		&theData);
CFDateRef		ToCF(const NDate		&theDate);
CFDictionaryRef	ToCF(const NDictionary	&theDictionary);
CFNumberRef		ToCF(const NNumber		&theNumber);
CFStringRef		ToCF(const NString		&theString);
CFURLRef		ToCF(const NURL			&theURL);



// CoreFoundation to Nano
inline NRange ToNN(const CFRange &theRange)
{
	return(NRange(theRange.location, theRange.length));
}

inline NArray ToNN(CFArrayRef theArray)
{
	return(NCFArray(theArray, false));
}

inline NData ToNN(CFDataRef theData)
{
	return(NCFData(theData, false));
}

inline NDate ToNN(CFDateRef theDate)
{
	return(NCFDate(theDate, false));
}

inline NDictionary ToNN(CFDictionaryRef theDictionary)
{
	return(NCFDictionary(theDictionary, false));
}

inline NNumber ToNN(CFNumberRef theNumber)
{
	return(NCFNumber(theNumber, false));
}

inline NString ToNN(CFStringRef theString)
{
	return(NCFString(theString, false));
}

inline NURL ToNN(CFURLRef theURL)
{
	return(NCFURL(theURL, false));
}

inline NArray ToNN(CFMutableArrayRef theArray)
{
	return(NCFArray(theArray, false));
}

inline NData ToNN(CFMutableDataRef theData)
{
	return(NCFData(theData, false));
}

inline NDictionary ToNN(CFMutableDictionaryRef theDictionary)
{
	return(NCFDictionary(theDictionary, false));
}

inline NString ToNN(CFMutableStringRef theString)
{
	return(NCFString(theString, false));
}





#endif // NCOREFOUNDATION_HDR





