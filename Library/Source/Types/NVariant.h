/*	NAME:
		NVariant.h

	DESCRIPTION:
		Variant data type.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
class NVariant;
class NVariantData;

typedef std::vector<NVariant>										NVariantList;
typedef NVariantList::iterator										NVariantListIterator;
typedef NVariantList::const_iterator								NVariantListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NVariant {
public:
	template <class T>					 NVariant(const T &theValue);
	inline								 NVariant(const NVariant &theValue);

	inline								 NVariant(void);
	inline virtual						~NVariant(void);


	// Is the value valid?
	inline bool							IsValid(void) const;


	// Is the value of a type?
	//
	//		NVariant	theValue;
	//		SInt32		theInt;
	//
	//		if (theValue.IsType(typeid(theInt)))
	//			;
	//
	//		if (theValue.IsType(theInt))
	//			;
	//
	// The type can be explicit, or obtained implicitly from a value.
	inline bool							IsType(const std::type_info &theType)  const;
	template <class T> bool				IsType(const T              &theValue) const;


	// Get the type
	inline const std::type_info			&GetType(void) const;	


	// Get the value
	template <class T> bool				GetValue(T &theValue) const;


	// Operators
	inline NVariant&					operator =  (const NVariant &theValue);
	inline bool							operator == (const NVariant &theValue) const;
	inline bool							operator != (const NVariant &theValue) const;


private:
	NVariantData						*mData;
};





//============================================================================
//		Class definition
//----------------------------------------------------------------------------
#define   NVARIANT_CPP
#include "NVariant.cpp"
#undef    NVARIANT_CPP




#endif // NVARIANT_HDR


