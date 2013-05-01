/*	NAME:
		NVariant.cpp

	DESCRIPTION:
		Variant data type.
		
		An NVariant object can hold any type of value, storing values within
		a template that derives from a common base.
		
		The template value stores the actual value, while the NVariant wrapper
		provides assignment, comparisons, and casting to that value's type.
		
		
		Note that prior to invoking dynamic_cast, we must call FixDynamicCast
		on the template.
		
		This is due to a (probable) bug in gcc 4.2's 32-bit x86 implementation
		of RTTI, possibly due to the use of dynamic_cast on templated types
		which fall in different translation units.
		
		There is a known issue with this compiler (std::type_info compares by
		pointer, not value) with dynamic_cast across shared library boundaries,
		which may be related.
		
		
		The behavior seen was that an instance of an NData placed in an NVariant
		could not be fetched back out again via the bool-GetValue() in a
		specific app.
		
		If this app called T*-GetValue(), it got a valid pointer. If it then
		called bool-GetValue(), this returned false due to the internal call
		to T*-GetValue() returning NULL.
		
		Calling typeid() on the type was sufficient to resolve the problem.
		
		
		The problem was only seen with one specific data type (NData), and an
		exact duplicate of that class with a different name did not fail.

		The problem was only observed on 32-bit x86 systems in release builds;
		debug builds, PowerPC builds, or 64-bit x86 builds were unaffected.
		
		The problem was not observed with gcc 4.0.1, and valgrind did not detect
		any memory corruption in the host application.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NVARIANT_CPP
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NBitVector.h"
#include "NColor.h"
#include "NData.h"
#include "NDate.h"
#include "NDictionary.h"
#include "NNumber.h"
#include "NPoint.h"
#include "NRange.h"
#include "NFile.h"
#include "NRectangle.h"
#include "NSize.h"
#include "NVector.h"
#include "NString.h"
#include "NVariant.h"





//============================================================================
//		NVariant::CompareValuesT : Compare two values.
//----------------------------------------------------------------------------
template <class T> NComparison NVariant::CompareValuesT(const NVariant &value1, const NVariant &value2)
{	const T			*valuePtr1;
	const T			*valuePtr2;
	NComparison		theResult;



	// Get the state we need
	value1.mData->FixDynamicCast(typeid(T));
	value2.mData->FixDynamicCast(typeid(T));

	valuePtr1 = dynamic_cast<NVariantValue<T>*>(value1.mData)->GetValue();
	valuePtr2 = dynamic_cast<NVariantValue<T>*>(value2.mData)->GetValue();



	// Compare the values
	theResult = valuePtr1->Compare(*valuePtr2);

	return(theResult);
}





//============================================================================
//		NVariant::CompareValues : Compare two values.
//----------------------------------------------------------------------------
NComparison NVariant::CompareValues(const NVariant &value1, const NVariant &value2)
{	NComparison		theResult;



	// Check the values
	//
	// Testing by address gives us a fast test for equality, and a default
	// order for undefined cases (e.g., comparing different types).
	theResult = GetComparison(value1.mData, value2.mData);
	if (theResult == kNCompareEqualTo)
		return(theResult);



	// Check for NULL values
	//
	// NULL values can't be compared.
	if (!value1.IsValid() || !value2.IsValid())
		NN_LOG("Attempted to compare a NULL value");



	// Check for mismatched types
	//
	// Different types can't be compared, except for numeric values which we can
	// push through an NNumber to obtain a comparison.
	else if (value1.GetType() != value2.GetType())
		{
		if (value1.IsNumeric() && value2.IsNumeric())
			theResult = NNumber(value1).Compare(NNumber(value2));
		else
			NN_LOG("Attempted to compare different types (%s) (%s)", value1.GetType().name(), value2.GetType().name());
		}



	// Compare the values
	//
	// NVariant provides default comparisons for standard comparable objects.
	//
	// This list can be extended in the future to support new types. Unfortunately we can't
	// automatically determine if a type is a sub-class of NComparable, since our type may
	// also be a built-in type like 'long' rather than an object.
	else
		{
		if (value1.IsNumeric())
			theResult = NNumber(value1).Compare(NNumber(value2));

		else if (value1.IsType(typeid(NBitVector)))
			theResult = CompareValuesT<NBitVector>(value1, value2);

		else if (value1.IsType(typeid(NColor)))
			theResult = CompareValuesT<NColor>(value1, value2);

		else if (value1.IsType(typeid(NData)))
			theResult = CompareValuesT<NData>(value1, value2);

		else if (value1.IsType(typeid(NDate)))
			theResult = CompareValuesT<NDate>(value1, value2);

		else if (value1.IsType(typeid(NDictionary)))
			theResult = CompareValuesT<NDictionary>(value1, value2);

		else if (value1.IsType(typeid(NFile)))
			theResult = CompareValuesT<NFile>(value1, value2);

		else if (value1.IsType(typeid(NPoint)))
			theResult = CompareValuesT<NPoint>(value1, value2);

		else if (value1.IsType(typeid(NRange)))
			theResult = CompareValuesT<NRange>(value1, value2);

		else if (value1.IsType(typeid(NRectangle)))
			theResult = CompareValuesT<NRectangle>(value1, value2);

		else if (value1.IsType(typeid(NSize)))
			theResult = CompareValuesT<NSize>(value1, value2);

		else if (value1.IsType(typeid(NString)))
			theResult = CompareValuesT<NString>(value1, value2);

		else if (value1.IsType(typeid(NVector)))
			theResult = CompareValuesT<NVector>(value1, value2);

		else
			NN_LOG("NVariant::CompareValues passed an unknown type (%s)", value1.GetType().name());
		}

	return(theResult);
}

#else





//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRefCounted.h"
#include "NNumber.h"
#include "NVariant.h"





//============================================================================
//		NVariantData
//----------------------------------------------------------------------------
class NVariantData : public NRefCounted {
public:
										NVariantData(void) { }
	virtual							   ~NVariantData(void) { }

	virtual void						FixDynamicCast(const std::type_info &theType) const = 0;

	virtual const std::type_info	   &GetType(void) const = 0;
};





//============================================================================
//		NVariantValue
//----------------------------------------------------------------------------
template <class T> class NVariantValue : public NVariantData {
public:
										NVariantValue(const T &theValue) : mValue(theValue) { }
	virtual							   ~NVariantValue(void)                                 { }

	void FixDynamicCast(const std::type_info &/*theType*/) const
	{
		// Work around gcc 4.2 bug
	}

	const std::type_info			   &GetType(void) const
	{
		return(typeid(mValue));
	}

	const T							   *GetValue(void) const
	{
		return(&mValue);
	}


private:
	T									mValue;
};





//============================================================================
//		NVariant::NVariant : Constructor.
//----------------------------------------------------------------------------
template<typename T> NVariant::NVariant(const T &theValue)
{


	// Initialize ourselves
	mData = new NVariantValue<T>(theValue);
}





//============================================================================
//		NVariant::NVariant : Constructor.
//----------------------------------------------------------------------------
NVariant::NVariant(const NVariant &theValue)
		: NComparable<NVariant>()
{


	// Initialize ourselves
	mData = theValue.mData;
	
	if (mData != NULL)
		mData->Retain();
}





//============================================================================
//		NVariant::NVariant : Constructor.
//----------------------------------------------------------------------------
NVariant::NVariant(void)
{


	// Initialize ourselves
	mData = NULL;
}





//============================================================================
//		NVariant::~NVariant : Destructor.
//----------------------------------------------------------------------------
NVariant::~NVariant(void)
{


	// Clean up
	if (mData != NULL)
		mData->Release();
}





//============================================================================
//		NVariant::IsValid : Is the value valid?
//----------------------------------------------------------------------------
bool NVariant::IsValid(void) const
{


	// Get our state
	return(mData != NULL);
}





//============================================================================
//		NVariant::IsNumeric : Is the value numeric?
//----------------------------------------------------------------------------
bool NVariant::IsNumeric(void) const
{	bool	isNumeric;



	// Check our type
	//
	// As per NNumber, some unsized types are also allowed to support literal constants.
	isNumeric = IsType(typeid(NNumber)) ||
				IsType(typeid(UInt8))   ||
				IsType(typeid(UInt16))  ||
				IsType(typeid(UInt32))  ||
				IsType(typeid(UInt64))  ||
				IsType(typeid(SInt8))   ||
				IsType(typeid(SInt16))  ||
				IsType(typeid(SInt32))  ||
				IsType(typeid(SInt64))  ||
				IsType(typeid(Float32)) ||
				IsType(typeid(Float64)) ||
				IsType(typeid(int))     ||
				IsType(typeid(long));

	return(isNumeric);
}





//============================================================================
//		NVariant::IsType : Is the value of a type?
//----------------------------------------------------------------------------
bool NVariant::IsType(const std::type_info &theType) const
{


	// Check our type
	return(GetType() == theType);
}





//============================================================================
//		NVariant::IsType : Is the value of a type?
//----------------------------------------------------------------------------
template <class T> bool NVariant::IsType(const T &theValue) const
{


	// Check our type
	return(GetType() == typeid(theValue));
}





//============================================================================
//		NVariant::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NVariant::Compare(const NVariant &theValue) const
{


	// Compare the value
	return(NVariant::CompareValues(*this, theValue));
}





//============================================================================
//		NVariant::GetType : Get the type.
//----------------------------------------------------------------------------
const std::type_info &NVariant::GetType(void) const
{


	// Get the type
	if (mData == NULL)
		return(typeid(void));
	
	return(mData->GetType());
}





//============================================================================
//		NVariant::GetValue : Get the value.
//----------------------------------------------------------------------------
template <class T> const T *NVariant::GetValue(void) const
{	const T				*valuePtr;
	NVariantValue<T>	*dataPtr;



	// Get the value
	valuePtr = NULL;

	if (mData != NULL)
		{
		mData->FixDynamicCast(typeid(T));

		dataPtr = dynamic_cast<NVariantValue<T>*>(mData);
		if (dataPtr != NULL)
			valuePtr = dataPtr->GetValue();
		}

	return(valuePtr);
}





//============================================================================
//		NVariant::GetValue : Get the value.
//----------------------------------------------------------------------------
template <class T> bool NVariant::GetValue(T &theValue) const
{	const T		*valuePtr;



	// Get the value
	valuePtr = GetValue<T>();
	if (valuePtr != NULL)
		theValue = *valuePtr;
	
	return(valuePtr != NULL);
}





//============================================================================
//		NVariant::= : Assignment operator.
//----------------------------------------------------------------------------
NVariant& NVariant::operator = (const NVariant &theValue)
{


	// Adjust the reference counts
	if (theValue.mData != NULL)
		theValue.mData->Retain();
		
	if (mData != NULL)
		mData->Release();



	// Assign the value
	mData = theValue.mData;

	return(*this);
}





#endif // NVARIANT_CPP



