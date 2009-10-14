/*	NAME:
		NCFPreferences.h

	DESCRIPTION:
		CoreFoundation preferences.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFPREFERENCES_HDR
#define NCFPREFERENCES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"
#include "NDictionary.h"
#include "NCFDate.h"
#include "NString.h"
#include "NColor.h"
#include "NArray.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgPreferenceValueChanged				= 'pval';

static const NString kPrefChangedAllKey								= "*";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFPreferences : public NMsgBroadcaster {
public:
										NCFPreferences(const NString &appID="");
	virtual								~NCFPreferences(void);


	// Save the preferences
	void								Save(void);


	// Does a preference exist?
	bool								HasValue(const NString &theKey, bool checkDefaults=true);


	// Remove a preference
	void								RemoveValue(const NString &theKey);


	// Get/set a value
	//
	// If no value is associated with the key, and the key does not exist in
	// the defaults dictionary, a 0'd/empty value is returned.
	bool								GetValueBoolean(   const NString &theKey) const;
	OSType								GetValueOSType(    const NString &theKey) const;
	UInt32								GetValueUInt32(    const NString &theKey) const;
	Float64								GetValueFloat64(   const NString &theKey) const;
	NString								GetValueString(    const NString &theKey) const;
	HIPoint								GetValuePoint(     const NString &theKey) const;
	HISize								GetValueSize(      const NString &theKey) const;
	HIRect								GetValueRect(      const NString &theKey) const;
	NColor								GetValueColor(     const NString &theKey) const;
	NCFDate								GetValueDate(      const NString &theKey) const;
	NArray								GetValueArray(     const NString &theKey) const;
	NDictionary							GetValueDictionary(const NString &theKey) const;

	void								SetValueBoolean(   const NString &theKey, bool               theValue);
	void								SetValueOSType(    const NString &theKey, OSType             theValue);
	void								SetValueUInt32(    const NString &theKey, UInt32             theValue);
	void								SetValueFloat64(   const NString &theKey, Float64            theValue);
	void								SetValueString(    const NString &theKey, const NString     &theValue);
	void								SetValuePoint(     const NString &theKey, const HIPoint      &theValue);
	void								SetValueSize(      const NString &theKey, const HISize       &theValue);
	void								SetValueRect(      const NString &theKey, const HIRect       &theValue);
	void								SetValueColor(     const NString &theKey, const NColor       &theValue);
	void								SetValueDate(      const NString &theKey, const NCFDate      &theValue);
	void								SetValueArray(     const NString &theKey, const NArray      &theValue);
	void								SetValueDictionary(const NString &theKey, const NDictionary &theValue);


	// Get/set the defaults dictionary
	//
	// The defaults dictionary is used to provide default values for keys
	// that do not have a value in the preferences (e.g., on first launch).
	NDictionary							GetDefaults(void) const;
	void								SetDefaults(const NDictionary &theDefaults);


	// Get the application preferences
	static NCFPreferences				*GetPrefs(void);
	

private:
	void								DoPrefChanged(const NString &theKey);


private:
	NString								mAppID;
	NDictionary							mDefaults;
};





#endif // NCFPREFERENCES_HDR


