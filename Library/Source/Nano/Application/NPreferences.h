/*	NAME:
		NPreferences.h

	DESCRIPTION:
		Preferences.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPREFERENCES_HDR
#define NPREFERENCES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPropertyStore.h"
#include "NListener.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const NBroadcastMsg kMsgNPreferenceValueChanged				= 0x7076616C;	// 'pval'

extern const NString kNPrefChangedAllKey;





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


	// Flush the preferences
	void								Flush(void);


    // Get the instance
    static NPreferences				   *Get(void);


protected:
	// Handle messages
	void								DoMessage(NBroadcastMsg theMsg, const void *msgData);


private:


};





#endif // NPREFERENCES_HDR


