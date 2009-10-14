/*	NAME:
		NCFObject.h

	DESCRIPTION:
		CoreFoundation object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFOBJECT_HDR
#define NCFOBJECT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreFoundationUtilities.h"
#include "NAppKitUtilities.h"





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Type accessor
//
// NCFObject provides a set of operators to provide automatic type-casting.
#define DECLARE_NCF_OPERATOR(_isMutable, _type)								\
	inline operator _type(void) const										\
	{																		\
		if ((_isMutable))													\
			NN_ASSERT(mIsMutable);											\
																			\
		return((_type) mObject);											\
	}





//============================================================================
//		Types
//----------------------------------------------------------------------------
DECLARE_OBJC_CLASS(NSArray);
DECLARE_OBJC_CLASS(NSData);
DECLARE_OBJC_CLASS(NSDictionary);
DECLARE_OBJC_CLASS(NSString);
DECLARE_OBJC_CLASS(NSMutableArray);
DECLARE_OBJC_CLASS(NSMutableData);
DECLARE_OBJC_CLASS(NSMutableDictionary);
DECLARE_OBJC_CLASS(NSMutableString);





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFObject {
public:
										NCFObject(const NCFObject &theObject);
										NCFObject(void);
	virtual								~NCFObject(void);


	// Is the object valid?
	bool								IsValid(void) const;


	// Clear the object
	//
	// Clearing an object will assign the "null value" to the object.
	virtual void						Clear(void);


	// Get the object hash
	virtual CFHashCode					GetHash(void) const;


	// Get the object type
	CFTypeID							GetType(void) const;


	// Assign an object
	//
	// Assigns a new CF object; either taking ownership of the reference, or
	// retaining/copying the object as appropriate.
	//
	// If ownership is not transferred, the newly assigned value will be an
	// immutable copy of the object.
	//
	// Returns as the cfObject parameter was non-NULL; if it was NULL, the
	// "null value" for the object will be assigned instead.
	bool								Set(CFTypeRef cfObject, bool takeOwnership=true);


	// Operators
	const NCFObject&					operator = (const NCFObject &theObject);

	DECLARE_NCF_OPERATOR(false, CFArrayRef);
	DECLARE_NCF_OPERATOR(false, CFBitVectorRef);
	DECLARE_NCF_OPERATOR(false, CFBooleanRef);
	DECLARE_NCF_OPERATOR(false, CFBundleRef);
	DECLARE_NCF_OPERATOR(false, CFDataRef);
	DECLARE_NCF_OPERATOR(false, CFDateFormatterRef);
	DECLARE_NCF_OPERATOR(false, CFDateRef);
	DECLARE_NCF_OPERATOR(false, CFDictionaryRef);
	DECLARE_NCF_OPERATOR(false, CFHTTPMessageRef);
	DECLARE_NCF_OPERATOR(false, CFLocaleRef);
	DECLARE_NCF_OPERATOR(false, CFNumberRef);
	DECLARE_NCF_OPERATOR(false, CFReadStreamRef);
	DECLARE_NCF_OPERATOR(false, CFStringRef);
	DECLARE_NCF_OPERATOR(false, CFTimeZoneRef);
	DECLARE_NCF_OPERATOR(false, CFTypeRef);
	DECLARE_NCF_OPERATOR(false, CFURLRef);
	DECLARE_NCF_OPERATOR(false, CFUUIDRef);
	DECLARE_NCF_OPERATOR(false, CFWriteStreamRef);
	DECLARE_NCF_OPERATOR(false, CFXMLNodeRef);
	DECLARE_NCF_OPERATOR(false, CFXMLTreeRef);
	DECLARE_NCF_OPERATOR(false, CGColorRef);
	DECLARE_NCF_OPERATOR(false, CGColorSpaceRef);
	DECLARE_NCF_OPERATOR(false, CGContextRef);
	DECLARE_NCF_OPERATOR(false, CGDataProviderRef);
	DECLARE_NCF_OPERATOR(false, CGFunctionRef);
	DECLARE_NCF_OPERATOR(false, CGImageRef);
	DECLARE_NCF_OPERATOR(false, CGImageSourceRef);
	DECLARE_NCF_OPERATOR(false, CGPDFDocumentRef);
	DECLARE_NCF_OPERATOR(false, CGPDFPageRef);
	DECLARE_NCF_OPERATOR(false, CGShadingRef);
    DECLARE_NCF_OPERATOR(false, FSFileOperationRef);
	DECLARE_NCF_OPERATOR(false, HIShapeRef);
	DECLARE_NCF_OPERATOR(false, NSArray*);
	DECLARE_NCF_OPERATOR(false, NSData*);
	DECLARE_NCF_OPERATOR(false, NSDictionary*);
	DECLARE_NCF_OPERATOR(false, NSString*);
	DECLARE_NCF_OPERATOR(false, PasteboardRef);
    DECLARE_NCF_OPERATOR(false, SKDocumentRef);
    DECLARE_NCF_OPERATOR(false, SKIndexRef);
    DECLARE_NCF_OPERATOR(false, SKSearchRef);
	DECLARE_NCF_OPERATOR(true,  CFMutableArrayRef);
	DECLARE_NCF_OPERATOR(true,  CFMutableBitVectorRef);
	DECLARE_NCF_OPERATOR(true,  CFMutableDataRef);
	DECLARE_NCF_OPERATOR(true,  CFMutableDictionaryRef);
	DECLARE_NCF_OPERATOR(true,  CFMutableStringRef);
	DECLARE_NCF_OPERATOR(true,  HIMutableShapeRef);
	DECLARE_NCF_OPERATOR(true,  NSMutableArray*);
	DECLARE_NCF_OPERATOR(true,  NSMutableData*);
	DECLARE_NCF_OPERATOR(true,  NSMutableDictionary*);
	DECLARE_NCF_OPERATOR(true,  NSMutableString*);
	

protected:
	// Make the object mutable
	//
	// Objects that come in both mutable and immutable flavors (e.g., strings)
	// will typically stored as an immutable object until a modification is made.
	//
	// Sub-classes should use MakeMutable to ensure their object is mutable prior
	// to passing it to a mutator API.
	bool								MakeMutable(void);


	// Get the null value
	//
	// The "null value" is the immutable value assigned to an object when it is
	// cleared or assigned a NULL value.
	//
	// By default this value is NULL, however sub-classes can provide their own
	// null value to avoid having to test for NULL before using CF APIs.
	virtual CFTypeRef					GetNullValue(void);


	// Handle an object change
	//
	// Sub-classes should override to sync non-CF state when their assigned object
	// is changed (e.g., resetting any buffers used to hold object state).
	virtual void						DoObjectChanging(void);
	virtual void						DoObjectChanged( void);


private:
	void								InitializeSelf(void);


private:
	bool								mIsMutable;
	CFTypeRef							mObject;
};





#endif // NCFOBJECT_HDR


