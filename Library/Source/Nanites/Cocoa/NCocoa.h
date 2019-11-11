/*	NAME:
		NCocoa.h

	DESCRIPTION:
		Cocoa support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
#include "NCoreGraphics.h"
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
//		Build macros
//----------------------------------------------------------------------------
// NSGeometry
//
// The NSPoint/NSSize/NSRect geometry types are only available in Mac SDKs, so
// we need to avoid references to them on iOS.
//
// In addition some Mac builds (e.g., 64-bit) typedef these types to their CG
// equivalents, so we need to avoid duplicating functions that were already
// provided by NCoreGraphics.h.
#ifdef _APPKITDEFINES_H
	#define NN_NSGEOM_IS_AVAILABLE									1
#else
	#define NN_NSGEOM_IS_AVAILABLE									0
#endif

#if defined(NSGEOMETRY_TYPES_SAME_AS_CGGEOMETRY_TYPES) && NSGEOMETRY_TYPES_SAME_AS_CGGEOMETRY_TYPES
	#define NN_NSGEOM_IS_CGGEOM										1
#else
	#define NN_NSGEOM_IS_CGGEOM										0
#endif





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Nano to Cocoa
#if NN_NSGEOM_IS_AVAILABLE
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
#endif // NN_NSGEOM_IS_AVAILABLE

inline NSRange ToNS(const NRange &theRange)
{
	return(NSMakeRange((NSUInteger) theRange.GetLocation(), (NSUInteger) theRange.GetSize()));
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



// CoreGraphics to Cocoa
#if NN_NSGEOM_IS_AVAILABLE
inline NSPoint ToNS(const CGPoint &thePoint)
{
	return(NSMakePoint(thePoint.x, thePoint.y));
}

inline NSSize ToNS(const CGSize &theSize)
{
	return(NSMakeSize(theSize.width, theSize.height));
}

inline NSRect ToNS(const CGRect &theRect)
{
	return(NSMakeRect(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height));
}
#endif // !NN_NSGEOM_IS_AVAILABLE



// Cocoa to Nano
#if NN_NSGEOM_IS_AVAILABLE && !NN_NSGEOM_IS_CGGEOM
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
#endif // NN_NSGEOM_IS_AVAILABLE && !NN_NSGEOM_IS_CGGEOM

inline NRange ToNN(const NSRange &theRange)
{
	return(NRange((NIndex) theRange.location, (NIndex) theRange.length));
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



// Cocoa to CoreGraphics
#if NN_NSGEOM_IS_AVAILABLE
inline CGPoint ToCG(const NSPoint &thePoint)
{
	return(CGPointMake(thePoint.x, thePoint.y));
}

inline CGSize ToCG(const NSSize &theSize)
{
	return(CGSizeMake(theSize.width, theSize.height));
}

inline CGRect ToCG(const NSRect &theRect)
{
	return(CGRectMake(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height));
}
#endif // NN_NSGEOM_IS_AVAILABLE



// Equality operators
#if NN_NSGEOM_IS_AVAILABLE && !NN_NSGEOM_IS_CGGEOM
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
#endif // NN_NSGEOM_IS_AVAILABLE && !NN_NSGEOM_IS_CGGEOM



// Inequality operators
#if NN_NSGEOM_IS_AVAILABLE && !NN_NSGEOM_IS_CGGEOM
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
#endif // NN_NSGEOM_IS_AVAILABLE && !NN_NSGEOM_IS_CGGEOM





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


// NSBundle
#define NSBundleString(_name)								NSLocalizedString(@_name, @"")

#if NN_TARGET_MACOS
	#define NSBundleImage(_name)							[[NSImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@_name ofType:NULL]]
#else
	#define NSBundleImage(_name)							[UIImage imageNamed:@_name]
#endif




#endif // NCOCOA_HDR





