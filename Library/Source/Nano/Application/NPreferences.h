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
#include "NListener.h"
#include "NPropertyStore.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgPreferenceValueChanged				= 0x7076616C;	// 'pval'

static const NString kPrefChangedAllKey								= "*";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPreferences :	public NListener,
						public NPropertyStore {
public:
										NPreferences(void);
	virtual							   ~NPreferences(void);


	// Does a key exist?
	bool								HasKey(const NString &theKey, bool checkDefaults=true) const;


	// Remove a key
	void								RemoveKey(const NString &theKey);


	// Get/set a value
	NVariant							GetValue(const NString &theKey) const;
	void								SetValue(const NString &theKey, const NVariant &theValue);


    // Get the preferences
    static NPreferences					*Get(void);


protected:
	// Handle messages
	void								DoMessage(BroadcastMsg theMsg, const void *msgData);


private:


};





#endif // NPREFERENCES_HDR


