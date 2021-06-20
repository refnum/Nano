/*	NAME:
		NCFObject.h

	DESCRIPTION:
		CFTypeRef wrapper.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
#ifndef NCFOBJECT_H
#define NCFOBJECT_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// System
#include <CoreFoundation/CFBase.h>





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// Retain a CFTypeRef
#ifndef CFSafeRetain
	#define CFSafeRetain(_object)                           \
		do                                                  \
		{                                                   \
			if ((_object) != nullptr)                       \
			{                                               \
				CFRetain(CFTypeRef(_object));               \
			}                                               \
		} while (false)
#endif // CFSafeRetain


// Release a CFTypeRef
#ifndef CFSafeRelease
	#define CFSafeRelease(_object)                          \
		do                                                  \
		{                                                   \
			if ((_object) != nullptr)                       \
			{                                               \
				CFRelease(CFTypeRef(_object));              \
				(_object) = nullptr;                        \
			}                                               \
		} while (false)
#endif // CFSafeRelease


// Declare an Obj-C class
#ifdef __OBJC__
	#define NN_DECLARE_OBCJ_CLASS(_name)                    @class _name
#else
	#define NN_DECLARE_OBCJ_CLASS(_name)                    typedef struct ptr##_name* _name
#endif





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// CF source
//
// Some CF classes support data sharing:
//
//		NCFSource::Copy			Copy the data
//
//		NCFSource::View			Create a view onto external data
//
// The underlying data for a View must persist beyond the lifetime of any
// objects viewing that data.
enum class NCFSource
{
	Copy,
	View
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
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





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<typename T>
class NCFObject
{
public:
										NCFObject();
	virtual                            ~NCFObject();

										NCFObject(const NCFObject<T>& otherObject);
	NCFObject<T>&                       operator=(const NCFObject<T>& otherObject);

										NCFObject(NCFObject<T>&& otherObject);
	NCFObject<T>&                       operator=(NCFObject<T>&& otherObject);


	// Is the object valid?
	bool                                IsValid() const;


	// Set the object
	//
	// Transfers ownership of the CF object to this object.
	bool                                Set(T cfObject);


	// Operators
										operator T() const;

private:
	T                                   mObject;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCFObject.inl"


#endif // NCFOBJECT_H
