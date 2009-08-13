/*	NAME:
		NPreferences.h

	DESCRIPTION:
		Preferences.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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


	// Does a key exist?
	bool								HasKey(const NString &theKey, bool checkDefaults=true) const;


	// Remove a key
	void								RemoveKey(const NString &theKey);


	// Get/set a value
	NVariant							GetValue(const NString &theKey) const;
	void								SetValue(const NString &theKey, const NVariant &theValue);


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


	// Get/set the defaults
	NDictionary							GetDefaults(void);
	void								SetDefaults(const NDictionary &theDefaults);


    // Get the preferences
    static NPreferences					*Get(void);


private:
	NDictionary							mDefaults;
};





#endif // NPREFERENCES_HDR


