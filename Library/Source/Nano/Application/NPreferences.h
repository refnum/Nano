/*	NAME:
		NPreferences.h

	DESCRIPTION:
		Preferences.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPREFERENCES_HDR
#define NPREFERENCES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"
#include "NBroadcaster.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgPreferenceValueChanged				= 0x7076616C;

static const NString kPrefChangedAllKey								= "*";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPreferences : public NBroadcaster {
public:
										 NPreferences(void);
	virtual								~NPreferences(void);


	// Save the preferences
	void								Save(void);


	// Does a key exist?
	bool								HasKey(const NString &theKey, bool checkDefaults=true);


	// Remove a key
	void								RemoveKey(const NString &theKey);


	// Get/set a value
	//
	// If the value does not exist, or can not be returned as the specified type, 0/empty is returned.
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

	void								SetValueBoolean(   const NString &theKey,       bool         theValue);
	void								SetValueSInt32(    const NString &theKey,       SInt32       theValue);
	void								SetValueSInt64(    const NString &theKey,       SInt64       theValue);
	void								SetValueFloat32(   const NString &theKey,       Float32      theValue);
	void								SetValueFloat64(   const NString &theKey,       Float64      theValue);
	void								SetValueString(    const NString &theKey, const NString     &theValue);
	void								SetValuePoint(     const NString &theKey, const NPoint      &theValue);
	void								SetValueSize(      const NString &theKey, const NSize       &theValue);
	void								SetValueRectangle( const NString &theKey, const NRectangle  &theValue);
	void								SetValueColor(     const NString &theKey, const NColor      &theValue);
	void								SetValueDate(      const NString &theKey, const NDate       &theValue);
	void								SetValueArray(     const NString &theKey, const NArray      &theValue);
	void								SetValueDictionary(const NString &theKey, const NDictionary &theValue);
	void								SetValueData(      const NString &theKey, const NData       &theValue);


	// Get/set the defaults
	static NDictionary					&GetDefaults(void);
	void								SetDefaults(const NDictionary &theDefaults);


private:
	void								ChangedValue(const NString &theKey);


private:


};





#endif // NPREFERENCES_HDR


