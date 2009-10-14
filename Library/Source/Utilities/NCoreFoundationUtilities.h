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





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const CFRange kCFRangeNone               = CFRangeMake(0, 0);
static const CFRange kCFRangeAll				= CFRangeMake(0, kCFNotFound);





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


private:
	static void							InitializeTypeIDs(void);
	

private:
	static CFTypeID						mCFTypeArray;
	static CFTypeID						mCFTypeAttributedString;
	static CFTypeID						mCFTypeBag;
	static CFTypeID						mCFTypeBitVector;
	static CFTypeID						mCFTypeCharacterSet;
	static CFTypeID						mCFTypeData;
	static CFTypeID						mCFTypeDictionary;
	static CFTypeID						mCFTypeSet;
	static CFTypeID						mCFTypeString;
};





//============================================================================
//      Inline functions
//----------------------------------------------------------------------------
inline bool operator ==(const CFRange &value1, const CFRange &value2)
{
    return(value1.location == value2.location && value1.length == value2.length);
}

inline bool operator !=(const CFRange &value1, const CFRange &value2)
{
    return(value1.location != value2.location || value1.length != value2.length);
}






#endif // NCOREFOUNDATIONUTILITIES_HDR


