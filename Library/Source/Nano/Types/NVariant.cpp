/*	NAME:
		NVariant.cpp

	DESCRIPTION:
		Variant data type.
		
		An NVariant object can hold any type of value, storing values within
		a template that derives from a common base.
		
		The template value stores the actual value, while the NVariant wrapper
		provides assignment, comparisons, and casting to that value's type.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Linker
//----------------------------------------------------------------------------
#ifndef NVARIANT_CPP

void NVariant_SuppressNoCodeLinkerWarning(void);
void NVariant_SuppressNoCodeLinkerWarning(void)
{
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
	virtual								~NVariantData(void) { }

	virtual const std::type_info		&GetType(void) const = 0;

	virtual NComparison					Compare(const NVariantData *dataOther) const = 0;
};





//============================================================================
//		NVariantValue
//----------------------------------------------------------------------------
template <class T> class NVariantValue : public NVariantData {
public:
										 NVariantValue(const T &theValue) : mValue(theValue) { }
	virtual								~NVariantValue(void)                                 { }

	const std::type_info				&GetType(void) const
	{
		return(typeid(mValue));
	}

	const T								&GetValue(void) const
	{
		return(mValue);
	}

	NComparison							Compare(const NVariantData *dataOther) const
	{	const NVariantValue<T>		*valueOther;
	
	
		// Validate our state
		NN_ASSERT(GetType() == dataOther->GetType());
		
		
		// Compare the values
		valueOther = dynamic_cast<const NVariantValue<T>*>(dataOther);
		
		return(GetComparison(mValue, valueOther->mValue));
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
	isNumeric = IsType(typeid(UInt8))   ||
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
{	NComparison		theResult;



	// Check for NULL values
	//
	// NULL values can never be compared directly. Since we have to return some
	// kind of comparison, we use our address.
	//
	// This will lead to unpredictable results if we are a temporary object, but
	// since the comparison is undefined, this is acceptable.
	if (mData == NULL || theValue.mData == NULL)
		{
		NN_LOG("Attempted to compare a NULL value");
		theResult = GetComparison(this, &theValue);
		}



	// Check for mismatched types
	//
	// Different types can not be compared directly, except for numeric values
	// since we can push these through an NNumber to obtain a comparison.
	//
	// All other mis-matches fail as per NULL values.
	else if (mData->GetType() != theValue.mData->GetType())
		{
		if (IsNumeric() && theValue.IsNumeric())
			theResult = NNumber(*this).Compare(NNumber(theValue));
		else
			{
			NN_LOG("Attempted to compare different types");
			theResult = GetComparison(this, &theValue);
			}
		}



	// Compare the values
	else
		theResult = mData->Compare(theValue.mData);
	
	return(theResult);
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
template <class T> bool NVariant::GetValue(T &theValue) const
{	NVariantValue<T>	*valuePtr;
	bool				canCast;



	// Check our state
	if (mData == NULL)
		return(false);



	// Determine if we can cast
	canCast = (typeid(theValue) == mData->GetType());

	if (canCast)
		{
		valuePtr = dynamic_cast<NVariantValue<T>*>(mData);
		canCast  = (valuePtr != NULL);
		NN_ASSERT(canCast);
		}



	// Get the value
	if (canCast)
		theValue = valuePtr->GetValue();

	return(canCast);
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



