/*	NAME:
		NVariant.h

	DESCRIPTION:
		Variant data type.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NVARIANT_HDR
#define NVARIANT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <typeinfo>





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NVariantData;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NVariant {
public:

	template <class T>					NVariant(const T &theValue);

										NVariant(const NVariant &theValue);
										NVariant(void);
	virtual								~NVariant(void);


	// Is the value valid?
	bool								IsValid(void) const;


	// Get the value type
	const std::type_info				&GetType(void) const;


	// Is the value of a type?
	//
	// The type can be supplied directly, or through an example value.
	template <class T> bool				IsType(void)              const;
	template <class T> bool				IsType(const T &theValue) const;


	// Cast to a type
	template <class T> const T			&CastTo(void) const;


	// Operators
	template <class T>					operator T  (void) const;
	NVariant&							operator =  (const NVariant &theValue);
	bool								operator == (const NVariant &theValue) const;
	bool								operator != (const NVariant &theValue) const;


private:
	NVariantData						*mData;
};





#endif // NVARIANT_HDR


