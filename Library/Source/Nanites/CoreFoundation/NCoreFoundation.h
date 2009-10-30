/*	NAME:
		NCoreFoundation.h

	DESCRIPTION:
		CoreFoundation support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Convert to CoreFoundation
inline CFRange ToCF(const NRange &theRange)
{
	return(CFRangeMake(theRange.GetLocation(), theRange.GetSize()));
}

inline NCFObject ToCF(const NArray &theArray)
{
	return(NCFArray(theArray).GetObject());
}

inline NCFObject ToCF(const NData &theData)
{
	return(NCFData(theData).GetObject());
}

inline NCFObject ToCF(const NDate &theDate)
{
	return(NCFDate(theDate).GetObject());
}

inline NCFObject ToCF(const NDictionary &theDictionary)
{
	return(NCFDictionary(theDictionary).GetObject());
}

inline NCFObject ToCF(const NNumber &theNumber)
{
	return(NCFNumber(theNumber).GetObject());
}

inline NCFObject ToCF(const NString &theString)
{
	return(NCFString(theString).GetObject());
}

inline NCFObject ToCF(const NCFArray &theArray)
{
	return(theArray.GetObject());
}

inline NCFObject ToCF(const NCFData &theData)
{
	return(theData.GetObject());
}

inline NCFObject ToCF(const NCFDate &theDate)
{
	return(theDate.GetObject());
}

inline NCFObject ToCF(const NCFDictionary &theDictionary)
{
	return(theDictionary.GetObject());
}

inline NCFObject ToCF(const NCFNumber &theNumber)
{
	return(theNumber.GetObject());
}

inline NCFObject ToCF(const NCFString &theString)
{
	return(theString.GetObject());
}



// Convert from CoreFoundation
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




#endif // NCOREFOUNDATION_HDR





