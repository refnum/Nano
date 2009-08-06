/*	NAME:
		NCFObject.h

	DESCRIPTION:
		CFTypeRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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


// Declare a constructor/cast operator
#define DECLARE_NCF_OPERATOR(_type)											\
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
//		Class declaration
//----------------------------------------------------------------------------
class NCFObject {
public:
										NCFObject(CFTypeRef cfObject, bool takeOwnership);
										NCFObject(const NCFObject &otherObject);

										NCFObject(void);
	virtual								~NCFObject(void);


	// Is the object valid?
	bool								IsValid(void) const;


	// Set the object
	bool								SetObject(CFTypeRef cfObject, bool takeOwnership=true);


	// Operators
	const NCFObject&					operator = (const NCFObject &theObject);

	DECLARE_NCF_OPERATOR(CFArrayRef);
	DECLARE_NCF_OPERATOR(CFBooleanRef);
	DECLARE_NCF_OPERATOR(CFBundleRef);
	DECLARE_NCF_OPERATOR(CFDataRef);
	DECLARE_NCF_OPERATOR(CFDateRef);
	DECLARE_NCF_OPERATOR(CFDictionaryRef);
	DECLARE_NCF_OPERATOR(CFMutableArrayRef);
	DECLARE_NCF_OPERATOR(CFMutableDataRef);
	DECLARE_NCF_OPERATOR(CFMutableDictionaryRef);
	DECLARE_NCF_OPERATOR(CFMutableStringRef);
	DECLARE_NCF_OPERATOR(CFNumberRef);
	DECLARE_NCF_OPERATOR(CFStringRef);
	DECLARE_NCF_OPERATOR(CFTypeRef);
	DECLARE_NCF_OPERATOR(CFURLRef);
	

private:
	void								InitializeSelf(CFTypeRef cfObject, bool takeOwnership);


private:
	CFTypeRef							mObject;
};




#endif // NCFOBJECT_HDR


