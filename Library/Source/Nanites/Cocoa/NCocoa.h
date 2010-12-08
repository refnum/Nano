/*	NAME:
		NCocoa.h

	DESCRIPTION:
		Cocoa support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOCOA_HDR
#define NCOCOA_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NPoint.h"
#include "NSize.h"
#include "NRectangle.h"
#include "NRange.h"
#include "NURL.h"
#include "NCFArray.h"
#include "NCFData.h"
#include "NCFDate.h"
#include "NCFDictionary.h"
#include "NCFNumber.h"
#include "NCFString.h"
#include "NCFURL.h"





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Nano to Cocoa
#if NN_TARGET_MAC
inline NSPoint ToNS(const NPoint &thePoint)
{
	return(NSMakePoint(thePoint.x, thePoint.y));
}

inline NSSize ToNS(const NSize &theSize)
{
	return(NSMakeSize(theSize.width, theSize.height));
}

inline NSRect ToNS(const NRectangle &theRect)
{
	return(NSMakeRect(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height));
}
#endif // NN_TARGET_MAC

inline NSRange ToNS(const NRange &theRange)
{
	return(NSMakeRange(theRange.GetLocation(), theRange.GetSize()));
}

inline NSArray *ToNS(const NArray &theArray)
{	NCFObject		cfObject  = NCFArray(theArray).GetObject();
	NSArray			*nsObject = cfObject;

	return([[nsObject retain] autorelease]);
}

inline NSData *ToNS(const NData &theData)
{	NCFObject		cfObject  = NCFData(theData).GetObject();
	NSData			*nsObject = cfObject;

	return([[nsObject retain] autorelease]);
}

inline NSDate *ToNS(const NDate &theDate)
{	NCFObject		cfObject  = NCFDate(theDate).GetObject();
	NSDate			*nsObject = cfObject;

	return([[nsObject retain] autorelease]);
}

inline NSDictionary *ToNS(const NDictionary &theDictionary)
{	NCFObject		cfObject  = NCFDictionary(theDictionary).GetObject();
	NSDictionary	*nsObject = cfObject;

	return([[nsObject retain] autorelease]);
}

inline NSNumber *ToNS(const NNumber &theNumber)
{	NCFObject		cfObject  = NCFNumber(theNumber).GetObject();
	NSNumber		*nsObject = cfObject;

	return([[nsObject retain] autorelease]);
}

inline NSString *ToNS(const NString &theString)
{	NCFObject		cfObject  = NCFString(theString).GetObject();
	NSString		*nsObject = cfObject;

	return([[nsObject retain] autorelease]);
}

inline NSURL *ToNS(const NURL &theURL)
{
	return([NSURL URLWithString:ToNS(theURL.GetValue())]);
}



// Cocoa to Nano
#if NN_TARGET_MAC
inline NPoint ToNN(const NSPoint &thePoint)
{
	return(NPoint(thePoint.x, thePoint.y));
}

inline NSize ToNN(const NSSize &theSize)
{
	return(NSize(theSize.width, theSize.height));
}

inline NRectangle ToNN(const NSRect &theRect)
{
	return(NRectangle(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height));
}
#endif // NN_TARGET_MAC

inline NRange ToNN(const NSRange &theRange)
{
	return(NRange(theRange.location, theRange.length));
}

inline NArray ToNN(NSArray *theArray)
{
	return(NCFArray((CFArrayRef) theArray, false));
}

inline NData ToNN(NSData *theData)
{
	return(NCFData((CFDataRef) theData, false));
}

inline NDate ToNN(NSDate *theDate)
{
	return(NCFDate((CFDateRef) theDate, false));
}

inline NDictionary ToNN(NSDictionary *theDictionary)
{
	return(NCFDictionary((CFDictionaryRef) theDictionary, false));
}

inline NNumber ToNN(NSNumber *theNumber)
{
	return(NCFNumber((CFNumberRef) theNumber, false));
}

inline NString ToNN(NSString *theString)
{
	return(NCFString((CFStringRef) theString, false));
}

inline NURL ToNN(NSURL *theURL)
{	NString		theString = ToNN([theURL absoluteString]);

	return(NURL(theString));
}

inline NArray ToNN(NSMutableArray *theArray)
{
	return(NCFArray((CFMutableArrayRef) theArray, false));
}

inline NData ToNN(NSMutableData *theData)
{
	return(NCFData((CFMutableDataRef) theData, false));
}

inline NDictionary ToNN(NSMutableDictionary *theDictionary)
{
	return(NCFDictionary((CFMutableDictionaryRef) theDictionary, false));
}

inline NString ToNN(NSMutableString *theString)
{
	return(NCFString((CFMutableStringRef) theString, false));
}



// Equality operators
#if NN_TARGET_MAC
inline bool operator==(const NSPoint &value1, const NSPoint &value2)
{
	return(	NMathUtilities::AreEqual(value1.x, value2.x) &&
			NMathUtilities::AreEqual(value1.y, value2.y));
}

inline bool operator==(const NSSize &value1, const NSSize &value2)
{
	return(	NMathUtilities::AreEqual(value1.width,  value2.width) &&
			NMathUtilities::AreEqual(value1.height, value2.height));
}

inline bool operator==(const NSRect &value1, const NSRect &value2)
{
	return(value1.origin == value2.origin && value1.size == value2.size);
}
#endif // NN_TARGET_MAC



// Inequality operators
#if NN_TARGET_MAC
inline bool operator!=(const NSPoint &value1, const NSPoint &value2)
{
	return(	!NMathUtilities::AreEqual(value1.x, value2.x) ||
			!NMathUtilities::AreEqual(value1.y, value2.y));
}

inline bool operator!=(const NSSize &value1, const NSSize &value2)
{
	return(	!NMathUtilities::AreEqual(value1.width,  value2.width) ||
			!NMathUtilities::AreEqual(value1.height, value2.height));
}

inline bool operator!=(const NSRect &value1, const NSRect &value2)
{
	return(value1.origin != value2.origin || value1.size != value2.size);
}
#endif // NN_TARGET_MAC





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// NSObject
#define NSInitSuper(...)													\
		do																	\
			{																\
			if ((self = [super __VA_ARGS__]) == NULL)						\
				return(NULL);												\
			}																\
		while(0)


// NSBundle
#define NSBundleString(_name)								NSLocalizedString(@_name, @"")
#define NSBundleImage( _name)								[[NSImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@_name ofType:NULL]]


// Category
//
// Nano's default build configuration includes -ObjC, which ensures that
// categories defined in static libraries are linked in to the executable.
//
// However a linker bug prevents this from having an effect on 64-bit/iOS
// builds if the category file only contains a category.
//
//		http://developer.apple.com/library/mac/#qa/qa2006/qa1490.html
//
// This macro can be used to declare a dummy class to work around this bug.
#define NSInitCategory(_category)											\
	@interface		NWorkaround_QA1490_ ## _category						\
	@end																	\
																			\
	@implementation	NWorkaround_QA1490_ ## _category						\
	@end




#endif // NCOCOA_HDR





