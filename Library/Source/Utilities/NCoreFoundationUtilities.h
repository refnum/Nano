/*	NAME:
		NCoreFoundationUtilities.h

	DESCRIPTION:
		CoreFoundation utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOREFOUNDATIONUTILITIES_HDR
#define NCOREFOUNDATIONUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <algorithm>





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const CFRange kCFRangeNone               = CFRangeMake(0, 0);
static const CFRange kCFRangeAll				= CFRangeMake(0, kCFNotFound);

static const CFHashCode kCFHashCodeNull			= 0;





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Default allocator
//
// Nano uses kCFAllocatorNano for all CF allocators; by default this is the
// standard default allocator (but can be adjusted if necessary).
#ifndef kCFAllocatorNano
#define kCFAllocatorNano									kCFAllocatorDefault
#endif


// Retain a CFTypeRef
#ifndef CFSafeRetain
#define CFSafeRetain(_object)												\
	do																		\
		{																	\
		if ((_object) != NULL)												\
			CFRetain((CFTypeRef) (_object));								\
		}																	\
	while(0)
#endif // CFSafeRetain


// Release a CFTypeRef
#ifndef CFSafeRelease
#define CFSafeRelease(_object)												\
	do																		\
		{																	\
		if ((_object) != NULL)												\
			{																\
			CFRelease((CFTypeRef) (_object));								\
			(_object) = NULL;												\
			}																\
		}																	\
	while(0)
#endif // CFSafeRelease





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCoreFoundationUtilities {
public:
	// Create an immutable copy of an object
	//
	// The returned object may be the source object with an incremented
	// reference count, or a new immutable instance with the same value.
	static CFTypeRef					CreateCopy(CFTypeRef cfObject);


	// Create a mutable copy of an object
	//
	// The returned object is a mutabl duplicate of the source object,
	// or NULL if the object can not be mutated.
	static CFTypeRef					CreateMutableCopy(CFTypeRef cfObject);


	// Purge a dictionary's values
	//
	// Removes 0'd/empty values from a dictionary.
	static void							PurgeValues(CFMutableDictionaryRef cfDictionary, bool canRecurse=true);


private:
	static void							InitializeTypeIDs(void);
	
	static void							PurgeArray(const void *theValue, void *userData);


private:
	static CFTypeID						mCFTypeArray;
	static CFTypeID						mCFTypeAttributedString;
	static CFTypeID						mCFTypeBag;
	static CFTypeID						mCFTypeBitVector;
	static CFTypeID						mCFTypeBoolean;
	static CFTypeID						mCFTypeCharacterSet;
	static CFTypeID						mCFTypeData;
	static CFTypeID						mCFTypeDictionary;
	static CFTypeID						mCFTypeNumber;
	static CFTypeID						mCFTypeSet;
	static CFTypeID						mCFTypeString;
};





//============================================================================
//      Inline functions
//----------------------------------------------------------------------------
// Equality operators
inline bool operator ==(const CFRange &range1, const CFRange &range2)
{
    return(range1.location == range2.location && range1.length == range2.length);
}

inline bool operator !=(const CFRange &range1, const CFRange &range2)
{
    return(range1.location != range2.location || range1.length != range2.length);
}


// Range limits
//
// A range has a location and a length: the first and last items are items which lie
// in the range, while the "next item" is the item immediately following the range.
//
// The last item in an empty range is the same as the first: the range has no content,
// but still has a location.
inline CFIndex CFRangeGetFirst(const CFRange &range)
{
    return(range.location);
}

inline CFIndex CFRangeGetLast(const CFRange &range)
{
	if (range.length == 0)
		return(range.location);
	else
		return(range.location + range.length - 1);
}

inline CFIndex CFRangeGetNext(const CFRange &range)
{
	return(range.location + range.length);
}


// Range operations
inline CFRange CFRangeUnion(const CFRange &range1, const CFRange &range2)
{	CFIndex		rangeFirst, rangeLast;

	rangeFirst = std::min(CFRangeGetFirst(range1), CFRangeGetFirst(range2));
	rangeLast  = std::max(CFRangeGetLast( range1), CFRangeGetLast( range2));
	
	return(CFRangeMake(rangeFirst, rangeLast - rangeFirst));
}

inline CFRange CFRangeIntersection(const CFRange &range1, const CFRange &range2)
{	CFIndex		rangeFirst, rangeLast;

	if (CFRangeGetLast(range1) < range2.location || CFRangeGetLast(range2) < range1.location)
		return(kCFRangeNone);

	rangeFirst = std::max(CFRangeGetFirst(range1), CFRangeGetFirst(range2));
	rangeLast  = std::min(CFRangeGetLast( range1), CFRangeGetLast( range2));

	return(CFRangeMake(rangeFirst, rangeLast - rangeFirst));
}

inline bool CFRangeIsEmpty(const CFRange &range)
{
	return(range.length == 0);
}

inline bool CFRangeIsNotEmpty(const CFRange &range)
{
	return(range.length != 0);
}

inline bool CFRangeOverlaps(const CFRange &range1, const CFRange &range2)
{
    return(CFRangeIsNotEmpty(CFRangeIntersection(range1, range2)));
}

inline bool CFRangeContains(const CFRange &range, CFIndex index)
{
    return(index >= CFRangeGetFirst(range) && index <= CFRangeGetLast(range));
}



#endif // NCOREFOUNDATIONUTILITIES_HDR


