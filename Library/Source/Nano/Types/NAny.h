/*	NAME:
		NVariant.h

	DESCRIPTION:
		Variant data type.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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

#include "NComparable.h"





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
class NVariant : public NComparable<NVariant> {
public:
	template <class T>					NVariant(const T &theValue);
	inline								NVariant(const NVariant &theValue);

	inline								NVariant(void);
	inline virtual					   ~NVariant(void);


	// Is the value valid?
	inline bool							IsValid(void) const;


	// Is the value numeric?
	inline bool							IsNumeric(void) const;


	// Is the value of a type?
	//
	//		NVariant	theValue;
	//		int32_t		theInt;
	//
	//		if (theValue.IsType(typeid(float)))
	//			foo();
	//
	//		if (theValue.IsType(theInt))
	//			bar();
	//
	// The type can be explicit, or obtained implicitly from a value.
	inline bool							IsType(const std::type_info &theType)  const;
	template <class T> bool				IsType(const T              &theValue) const;


	// Compare the value
	//
	// Only values of the same type, or numeric values, can be compared.
	inline NComparison					Compare(const NVariant &theValue) const;


	// Get the type
	inline const std::type_info			&GetType(void) const;	


	// Get the value
	template <class T> const T			*GetValue(void)        const;
	template <class T> bool				 GetValue(T &theValue) const;


	// Operators
	inline NVariant&					operator = (const NVariant &theValue);


	// Compare values
	static NComparison					CompareValues(const NVariant &value1, const NVariant &value2);


private:
	template <class T> static NComparison	CompareValuesT(const NVariant &value1, const NVariant &value2);


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


