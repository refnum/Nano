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





//============================================================================
//		Constants
//----------------------------------------------------------------------------
typedef enum {
	kCFRetain,
	kCFDuplicate
} CFReferenceMode;

static const CFRange kCFRangeAll				= CFRangeMake(0, kCFNotFound);





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// CF Allocator
//
// Nano uses kCFAllocatorNano for all CF allocators; by default this is the
// standard default allocator (but this can be adjusted if necessary).
#ifndef kCFAllocatorNano
#define kCFAllocatorNano									kCFAllocatorDefault
#endif


// Retain a CFTypeRef
#ifndef CFSafeRetain
#define CFSafeRetain(_object)												\
	do																		\
		{																	\
		if ((_object) != NULL)												\
			{																\
			CFRetain((CFTypeRef) (_object));								\
			}																\
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
class NCFObject {
public:
										NCFObject(const NCFObject &theObject);

										 NCFObject(void);
	virtual								~NCFObject(void);


	// Is the object valid?
	bool								IsValid(void) const;


	// Get the object hash
	CFHashCode							GetHash(void) const;


	// Get the object type
	CFTypeID							GetType(void) const;
	

	// Get/set the reference mode
	//
	// The reference mode can only be changed when no object is assigned.
	CFReferenceMode						GetMode(void) const;
	void								SetMode(CFReferenceMode theMode);


	// Set the object
	//
	// The object will be retained or duplicated based on the current reference mode,
	// then released if desired. Returns as the assigned object was not NULL.
	bool								Set(CFTypeRef cfObject, bool andRelease=true);


	// Operators
	const NCFObject&					operator = (const NCFObject &theObject);

	inline operator CFArrayRef(void)				const { return((CFArrayRef				) mObject); }
	inline operator CFBitVectorRef(void)			const { return((CFBitVectorRef			) mObject); }
	inline operator CFBooleanRef(void)				const { return((CFBooleanRef			) mObject); }
	inline operator CFBundleRef(void)				const { return((CFBundleRef				) mObject); }
	inline operator CFDataRef(void)					const { return((CFDataRef				) mObject); }
	inline operator CFDateFormatterRef(void)		const { return((CFDateFormatterRef		) mObject); }
	inline operator CFDateRef(void)					const { return((CFDateRef				) mObject); }
	inline operator CFDictionaryRef(void)			const { return((CFDictionaryRef			) mObject); }
	inline operator CFHTTPMessageRef(void)			const { return((CFHTTPMessageRef		) mObject); }
	inline operator CFLocaleRef(void)				const { return((CFLocaleRef				) mObject); }
	inline operator CFMutableArrayRef(void)			const { return((CFMutableArrayRef		) mObject); }
	inline operator CFMutableBitVectorRef(void)		const { return((CFMutableBitVectorRef	) mObject); }
	inline operator CFMutableDataRef(void)			const { return((CFMutableDataRef		) mObject); }
	inline operator CFMutableDictionaryRef(void)	const { return((CFMutableDictionaryRef	) mObject); }
	inline operator CFMutableStringRef(void)		const { return((CFMutableStringRef		) mObject); }
	inline operator CFNumberRef(void)				const { return((CFNumberRef				) mObject); }
	inline operator CFReadStreamRef(void)			const { return((CFReadStreamRef			) mObject); }
	inline operator CFStringRef(void)				const { return((CFStringRef				) mObject); }
	inline operator CFTimeZoneRef(void)				const { return((CFTimeZoneRef			) mObject); }
	inline operator CFTypeRef(void)					const { return((CFTypeRef				) mObject); }
	inline operator CFURLRef(void)					const { return((CFURLRef				) mObject); }
	inline operator CFUUIDRef(void)					const { return((CFUUIDRef				) mObject); }
	inline operator CFWriteStreamRef(void)			const { return((CFWriteStreamRef		) mObject); }
	inline operator CFXMLNodeRef(void)				const { return((CFXMLNodeRef			) mObject); }
	inline operator CFXMLTreeRef(void)				const { return((CFXMLTreeRef			) mObject); }
	inline operator CGColorRef(void)				const { return((CGColorRef				) mObject); }
	inline operator CGColorSpaceRef(void)			const { return((CGColorSpaceRef			) mObject); }
	inline operator CGContextRef(void)				const { return((CGContextRef			) mObject); }
	inline operator CGDataProviderRef(void)			const { return((CGDataProviderRef		) mObject); }
	inline operator CGFunctionRef(void)				const { return((CGFunctionRef			) mObject); }
	inline operator CGImageRef(void)				const { return((CGImageRef				) mObject); }
	inline operator CGImageSourceRef(void)			const { return((CGImageSourceRef		) mObject); }
	inline operator CGPDFDocumentRef(void)			const { return((CGPDFDocumentRef		) mObject); }
	inline operator CGPDFPageRef(void)				const { return((CGPDFPageRef			) mObject); }
	inline operator PasteboardRef(void)				const { return((PasteboardRef			) mObject); }
    inline operator FSFileOperationRef(void)		const { return((FSFileOperationRef      ) mObject); }
    inline operator SKDocumentRef(void)             const { return((SKDocumentRef           ) mObject); }
    inline operator SKIndexRef(void)				const { return((SKIndexRef              ) mObject); }
    inline operator SKSearchRef(void)				const { return((SKSearchRef             ) mObject); }


protected:
	// Get the null value
	//
	// Sub-classes should override to adjust the "null value" that is assigned
	// to the object when a NULL CFTypeRef is passed to Set().
	//
	// By default, this value is NULL. Sub-classes which wish to avoid checking
	// that they have a non-NULL value before manipulating their object may wish
	// to adjust this value to a non-NULL value (e.g., an empty string).
	virtual CFTypeRef					GetNullValue(void);


	// Handle an object change
	//
	// Sub-classes should override to handle a change in assigned object.
	virtual void						DoObjectChanging(void);
	virtual void						DoObjectChanged( void);
	
	
private:
	void								InitializeSelf(CFReferenceMode theMode, CFTypeRef cfObject);

	CFTypeRef							CreateDuplicate(CFTypeRef cfObject);


private:
	CFReferenceMode						mMode;
	CFTypeRef							mObject;
};





#endif // NCFOBJECT_HDR


