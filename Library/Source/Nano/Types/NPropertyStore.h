/*	NAME:
		NPropertyStore.h

	DESCRIPTION:
		Property store.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPROPERTYSTORE_HDR
#define NPROPERTYSTORE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"
#include "NBroadcaster.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgPropertyValueChanged					= 0x70766368;

static const NString kPropertyChangedAllKey							= "*";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPropertyStore :	public NBroadcaster {
public:
										NPropertyStore(void);
	virtual							   ~NPropertyStore(void);


	// Does a key exist?
	virtual bool						HasKey(const NString &theKey, bool checkDefaults=true) const;


	// Remove a key
	virtual void						RemoveKey(const NString &theKey);


	// Get/set a value
	virtual NVariant					GetValue(const NString &theKey) const;
	virtual void						SetValue(const NString &theKey, const NVariant &theValue);


	// Get a type'd value
	//
	// If the value does not exist, or can not be returned as the specified type, 0/empty is returned.
	bool								GetValueBoolean(   const NString &theKey) const;
	SInt32								GetValueSInt32(    const NString &theKey) const;
	SInt64								GetValueSInt64(    const NString &theKey) const;
	Float32								GetValueFloat32(   const NString &theKey) const;
	Float64								GetValueFloat64(   const NString &theKey) const;
	NString								GetValueString(    const NString &theKey) const;
	NData								GetValueData(      const NString &theKey) const;
	NDate								GetValueDate(      const NString &theKey) const;
	NColor								GetValueColor(     const NString &theKey) const;
	NPoint								GetValuePoint(     const NString &theKey) const;
	NSize								GetValueSize(      const NString &theKey) const;
	NRectangle							GetValueRectangle( const NString &theKey) const;
	NVector								GetValueVector(    const NString &theKey) const;
	NArray								GetValueArray(     const NString &theKey) const;
	NDictionary							GetValueDictionary(const NString &theKey) const;


	// Get/set the properties
	virtual NDictionary					GetProperties(void) const;
	virtual void						SetProperties(const NDictionary &theProperties);


	// Get/set the defaults
	NDictionary							GetDefaults(void);
	void								SetDefaults(const NDictionary &theDefaults);


private:
	NDictionary							mProperties;
	NDictionary							mDefaults;
};





#endif // NPROPERTYSTORE_HDR


