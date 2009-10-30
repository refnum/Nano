/*	NAME:
		NPropertyStore.h

	DESCRIPTION:
		Property store.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPROPERTYSTORE_HDR
#define NPROPERTYSTORE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NArray.h"
#include "NAlias.h"
#include "NMsgBroadcaster.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgPropertyValueChanged					= 'pvch';

static const NString kPropertyAllValuesKey							= "*";





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NDictionary;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPropertyStore : public NMsgBroadcaster {
public:
										NPropertyStore(void);
	virtual								~NPropertyStore(void);


	// Does a key exist?
	bool								HasKey(const NString &theKey, bool checkDefaults=true);


	// Remove a key
	void								RemoveKey(const NString &theKey);


	// Get/set a value
	//
	// If the key does not exist, a 0'd/empty value is returned.
	bool								GetValueBoolean(   const NString &theKey) const;
	OSType								GetValueOSType(    const NString &theKey) const;
	UInt32								GetValueUInt32(    const NString &theKey) const;
	UInt64								GetValueUInt64(    const NString &theKey) const;
	Float32								GetValueFloat32(   const NString &theKey) const;
	Float64								GetValueFloat64(   const NString &theKey) const;
	NString								GetValueString(    const NString &theKey) const;
	HIPoint								GetValuePoint(     const NString &theKey) const;
	HISize								GetValueSize(      const NString &theKey) const;
	HIRect								GetValueRect(      const NString &theKey) const;
	NColor								GetValueColor(     const NString &theKey) const;
	NAlias								GetValueAlias(     const NString &theKey) const;
	NDate								GetValueDate(      const NString &theKey) const;
	NArray								GetValueArray(     const NString &theKey) const;
	NDictionary							GetValueDictionary(const NString &theKey) const;
	NData								GetValueData(      const NString &theKey) const;

	void								SetValueBoolean(   const NString &theKey, bool                theValue);
	void								SetValueOSType(    const NString &theKey, OSType              theValue);
	void								SetValueUInt32(    const NString &theKey, UInt32              theValue);
	void								SetValueUInt64(    const NString &theKey, UInt64              theValue);
	void								SetValueFloat32(   const NString &theKey, Float32             theValue);
	void								SetValueFloat64(   const NString &theKey, Float64             theValue);
	void								SetValueString(    const NString &theKey, const NString      &theValue);
	void								SetValuePoint(     const NString &theKey, const HIPoint      &theValue);
	void								SetValueSize(      const NString &theKey, const HISize       &theValue);
	void								SetValueRect(      const NString &theKey, const HIRect       &theValue);
	void								SetValueColor(     const NString &theKey, const NColor       &theValue);
	void								SetValueAlias(     const NString &theKey, const NAlias       &theValue);
	void								SetValueDate(      const NString &theKey, const NDate        &theValue);
	void								SetValueArray(     const NString &theKey, const NArray       &theValue);
	void								SetValueDictionary(const NString &theKey, const NDictionary  &theValue);
	void								SetValueData(      const NString &theKey, const NData        &theValue);


	// Get/set the defaults
	NDictionary							GetDefaults(void) const;
	void								SetDefaults(const NDictionary &theDefaults);


	// Get/set the properties
	NDictionary							GetProperties(void) const;
	void								SetProperties(const NDictionary &theProperties);


protected:
	// Remove a property key
	//
	// Sub-classes can override to provide alternative storage.
	virtual void						RemovePropertyKey(const NString &theKey);


	// Get/set a property value
	//
	// Sub-classes can override to provide alternative storage. If passed kPropertyAllValuesKey,
	// the value should be an NDictionary of all properties in the store.
	virtual NCFObject					GetPropertyValue(const NString &theKey) const;
	virtual void						SetPropertyValue(const NString &theKey, const NCFObject &theValue);


private:
	NCFObject							GetValue(const NString &theKey) const;
	void								SetValue(const NString &theKey, const NCFObject &theValue);


private:
	NDictionary							mDefaults;
	NDictionary							mProperties;
};





#endif // NPROPERTYSTORE_HDR
