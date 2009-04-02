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
//		Linker stub
//----------------------------------------------------------------------------
#ifndef NVARIANT_CPP

void SuppressNoCodeLinkerWarning_NVariant(void);
void SuppressNoCodeLinkerWarning_NVariant(void)
{
}

#else





//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRefCounted.h"
#include "NVariant.h"





//============================================================================
//		NVariantData
//----------------------------------------------------------------------------
class NVariantData : public NRefCounted {
public:
										 NVariantData(void) { }
	virtual								~NVariantData(void) { }

	virtual const std::type_info		&GetType(void) const = 0;

	virtual bool						EqualTo(const NVariantData *dataOther) const = 0;
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

	bool								EqualTo(const NVariantData *dataOther) const
	{	const NVariantValue<T>		*valueOther;
	
		valueOther = dynamic_cast<const NVariantValue<T>*>(dataOther);
		if (valueOther == NULL)
			return(false);
		
		return(mValue == valueOther->mValue);
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



	// Get the state we need
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
NVariant& NVariant::operator = (const NVariant &theVariant)
{


	// Adjust the reference counts
	if (theVariant.mData != NULL)
		theVariant.mData->Retain();
		
	if (mData != NULL)
		mData->Release();



	// Assign the value
	mData = theVariant.mData;

	return(*this);
}





//============================================================================
//		NVariant::== : Equality operator.
//----------------------------------------------------------------------------
bool NVariant::operator == (const NVariant &theValue) const
{


	// Compare the values
	//
	// A NULL value is never equal to anything.
	if (mData == NULL || theValue.mData == NULL)
		return(false);

	return(mData->EqualTo(theValue.mData));
}





//============================================================================
//		NVariant::!= : Inequality operator.
//----------------------------------------------------------------------------
bool NVariant::operator != (const NVariant &theValue) const
{


	// Compare the values
	return(!(*this == theValue));
}




#endif // NVARIANT_CPP


