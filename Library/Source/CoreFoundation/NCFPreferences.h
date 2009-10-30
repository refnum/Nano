/*	NAME:
		NCFPreferences.h

	DESCRIPTION:
		CoreFoundation preferences.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFPREFERENCES_HDR
#define NCFPREFERENCES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPropertyStore.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgPreferenceValueChanged				= 'pval';

static const NString kPrefChangedAllKey								= "*";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFPreferences : public NPropertyStore {
public:
										NCFPreferences(const NString &appID="");
	virtual								~NCFPreferences(void);


	// Save the preferences
	void								Save(void);


	// Get the application preferences
	static NCFPreferences				*GetPrefs(void);


protected:
	// Remove a property key
	void								RemovePropertyKey(const NString &theKey);


	// Get/set a property value
	NCFObject							GetPropertyValue( const NString &theKey) const;
	void								SetPropertyValue( const NString &theKey, const NCFObject &theValue);


private:
	CFStringRef							mAppID;
};





#endif // NCFPREFERENCES_HDR


