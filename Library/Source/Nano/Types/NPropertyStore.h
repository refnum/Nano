/*	NAME:
		NPropertyStore.h

	DESCRIPTION:
		Property store.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
// Messages
static const NBroadcastMsg kMsgNPropertyStoreValueChanged			= 0x70766368;	// 'pvch'

extern const NString kNPropertyStoreChangedAllKey;





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
	int32_t								GetValueInt32(     const NString &theKey) const;
	int64_t								GetValueInt64(     const NString &theKey) const;
	float32_t							GetValueFloat32(   const NString &theKey) const;
	float64_t							GetValueFloat64(   const NString &theKey) const;
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


