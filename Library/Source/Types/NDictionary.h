/*	NAME:
		NDictionary.h

	DESCRIPTION:
		Dictionary object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDICTIONARY_HDR
#define NDICTIONARY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSharedValue.h"
#include "NContainer.h"
#include "NString.h"
#include "NPoint.h"
#include "NSize.h"
#include "NRectangle.h"
#include "NColor.h"
#include "NDate.h"
#include "NArray.h"
#include "NVariant.h"
#include "NData.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef std::map<NString, NVariant>									NDictionaryValue;
typedef NDictionaryValue::iterator									NDictionaryValueIterator;
typedef NDictionaryValue::const_iterator							NDictionaryValueConstIterator;

typedef NSharedValue<NDictionaryValue>								NSharedValueDictionary;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDictionary :	public NContainer,
					public NSharedValueDictionary {
public:
										 NDictionary(void);
	virtual								~NDictionary(void);


	// Get the size
	NIndex								GetSize(void) const;


	// Join two dictionaries
	//
	// Existing keys will have their values replaced.
	void								Join(const NDictionary &theValue);


	// Does a key exist?
	bool								HasKey(const NString &theKey) const;


	// Remove a key
	void								RemoveKey(const NString &theKey);


	// Get the keys
	NStringList							GetKeys(void) const;


	// Get/set a value
	NVariant							GetValue(const NString &theKey) const;
	void								SetValue(const NString &theKey, const NVariant &theValue);


	// Get a type'd value
	//
	// If the value can not be returned as the specified type, 0/empty is returned.
	bool								GetValueBoolean(   const NString &theKey) const;
	SInt32								GetValueSInt32(    const NString &theKey) const;
	SInt64								GetValueSInt64(    const NString &theKey) const;
	Float32								GetValueFloat32(   const NString &theKey) const;
	Float64								GetValueFloat64(   const NString &theKey) const;
	NString								GetValueString(    const NString &theKey) const;
	NPoint								GetValuePoint(     const NString &theKey) const;
	NSize								GetValueSize(      const NString &theKey) const;
	NRectangle							GetValueRectangle( const NString &theKey) const;
	NColor								GetValueColor(     const NString &theKey) const;
	NDate								GetValueDate(      const NString &theKey) const;
	NArray								GetValueArray(     const NString &theKey) const;
	NDictionary							GetValueDictionary(const NString &theKey) const;
	NData								GetValueData(      const NString &theKey) const;


protected:
	// Get the null value
	const NDictionaryValue				*GetNullValue(void) const;
};





#endif // NDICTIONARY_HDR


