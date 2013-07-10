/*	NAME:
		NCFObject.h

	DESCRIPTION:
		CFTypeRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFOBJECT_HDR
#define NCFOBJECT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Default allocator
#ifndef kCFAllocatorNano
#define kCFAllocatorNano									kCFAllocatorDefault
#endif





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Retain a CFTypeRef
#ifndef CFSafeRetain
#define CFSafeRetain(_object)												\
	do																		\
		{																	\
		if ((_object) != NULL)												\
			CFRetain((CFTypeRef) (_object));								\
		}																	\
	while (false)
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
	while (false)
#endif // CFSafeRelease


// Declare an Obj-C class
#ifdef __OBJC__
	#define NN_DECLARE_OBCJ_CLASS(_name)	@class							_name
#else
	#define NN_DECLARE_OBCJ_CLASS(_name)	typedef struct ptr ## _name		*_name
#endif


// Declare a constructor/cast operator
#define NCFOBJECT_OPERATOR(_type)											\
	inline NCFObject(_type cfObject)										\
	{																		\
		InitializeSelf(cfObject, false);									\
	}																		\
																			\
	inline operator _type(void) const										\
	{																		\
		return((_type) mObject);											\
	}





//============================================================================
//		Types
//----------------------------------------------------------------------------
NN_DECLARE_OBCJ_CLASS(NSArray);
NN_DECLARE_OBCJ_CLASS(NSAutoreleasePool);
NN_DECLARE_OBCJ_CLASS(NSData);
NN_DECLARE_OBCJ_CLASS(NSDate);
NN_DECLARE_OBCJ_CLASS(NSDictionary);
NN_DECLARE_OBCJ_CLASS(NSMutableArray);
NN_DECLARE_OBCJ_CLASS(NSMutableData);
NN_DECLARE_OBCJ_CLASS(NSMutableDictionary);
NN_DECLARE_OBCJ_CLASS(NSMutableString);
NN_DECLARE_OBCJ_CLASS(NSNumber);
NN_DECLARE_OBCJ_CLASS(NSObject);
NN_DECLARE_OBCJ_CLASS(NSString);
NN_DECLARE_OBCJ_CLASS(NSURL);





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFObject {
public:
										NCFObject(CFTypeRef cfObject, bool takeOwnership);
										NCFObject(const NCFObject &otherObject);

										NCFObject(void);
	virtual							   ~NCFObject(void);


	// Is the object valid?
	bool								IsValid(void) const;


	// Set the object
	bool								SetObject(CFTypeRef cfObject, bool takeOwnership=true);


	// Operators
	const NCFObject&					operator = (const NCFObject &theObject);

	NCFOBJECT_OPERATOR(CFArrayRef)
	NCFOBJECT_OPERATOR(CFAttributedStringRef)
	NCFOBJECT_OPERATOR(CFBitVectorRef)
	NCFOBJECT_OPERATOR(CFBooleanRef)
	NCFOBJECT_OPERATOR(CFBundleRef)
	NCFOBJECT_OPERATOR(CFDataRef)
	NCFOBJECT_OPERATOR(CFDateFormatterRef)
	NCFOBJECT_OPERATOR(CFDateRef)
	NCFOBJECT_OPERATOR(CFDictionaryRef)
	NCFOBJECT_OPERATOR(CFLocaleRef)
	NCFOBJECT_OPERATOR(CFMachPortRef)
	NCFOBJECT_OPERATOR(CFMutableArrayRef)
	NCFOBJECT_OPERATOR(CFMutableBitVectorRef)
	NCFOBJECT_OPERATOR(CFMutableDataRef)
	NCFOBJECT_OPERATOR(CFMutableDictionaryRef)
	NCFOBJECT_OPERATOR(CFMutableSetRef)
	NCFOBJECT_OPERATOR(CFMutableStringRef)
	NCFOBJECT_OPERATOR(CFNumberFormatterRef)
	NCFOBJECT_OPERATOR(CFNumberRef)
	NCFOBJECT_OPERATOR(CFReadStreamRef)
	NCFOBJECT_OPERATOR(CFRunLoopSourceRef)
	NCFOBJECT_OPERATOR(CFSetRef)
	NCFOBJECT_OPERATOR(CFSocketRef)
	NCFOBJECT_OPERATOR(CFStringRef)
	NCFOBJECT_OPERATOR(CFTimeZoneRef)
	NCFOBJECT_OPERATOR(CFTypeRef)
	NCFOBJECT_OPERATOR(CFURLRef)
	NCFOBJECT_OPERATOR(CFUUIDRef)
	NCFOBJECT_OPERATOR(CFWriteStreamRef)
	NCFOBJECT_OPERATOR(CGColorRef)
	NCFOBJECT_OPERATOR(CGColorSpaceRef)
	NCFOBJECT_OPERATOR(CGContextRef)
	NCFOBJECT_OPERATOR(CGDataProviderRef)
	NCFOBJECT_OPERATOR(CGFontRef)
	NCFOBJECT_OPERATOR(CGFunctionRef)
	NCFOBJECT_OPERATOR(CGGradientRef)
	NCFOBJECT_OPERATOR(CGImageRef)
	NCFOBJECT_OPERATOR(CGPDFDocumentRef)
	NCFOBJECT_OPERATOR(CGPDFPageRef)
	NCFOBJECT_OPERATOR(CGShadingRef)
	NCFOBJECT_OPERATOR(CTFontRef)
	NCFOBJECT_OPERATOR(CTLineRef)
	NCFOBJECT_OPERATOR(NSArray*)
	NCFOBJECT_OPERATOR(NSData*)
	NCFOBJECT_OPERATOR(NSDate*)
	NCFOBJECT_OPERATOR(NSDictionary*)
	NCFOBJECT_OPERATOR(NSNumber*)
	NCFOBJECT_OPERATOR(NSString*)


	// Mac-specific CF operators
#if NN_TARGET_MAC
	NCFOBJECT_OPERATOR(CFHTTPMessageRef)
	NCFOBJECT_OPERATOR(CFXMLNodeRef)
	NCFOBJECT_OPERATOR(CFXMLTreeRef)
	NCFOBJECT_OPERATOR(CGImageDestinationRef)
	NCFOBJECT_OPERATOR(CGImageSourceRef)
    NCFOBJECT_OPERATOR(FSFileOperationRef)
	NCFOBJECT_OPERATOR(HIMutableShapeRef)
	NCFOBJECT_OPERATOR(HIShapeRef)
	NCFOBJECT_OPERATOR(PasteboardRef)
    NCFOBJECT_OPERATOR(SKIndexRef)
    NCFOBJECT_OPERATOR(SKSearchRef)
#endif // NN_TARGET_MAC


private:
	void								InitializeSelf(CFTypeRef cfObject, bool takeOwnership);


private:
	CFTypeRef							mObject;
};




#endif // NCFOBJECT_HDR


