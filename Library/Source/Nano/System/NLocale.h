/*	NAME:
		NLocale.h

	DESCRIPTION:
		System locale.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NLOCALE_HDR
#define NLOCALE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NBroadcaster.h"
#include "NListener.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgLocaleModified						= 0x6C6D6F64;	// 'lmod'

static const NString kLocaleChangedAllKey							= "*";
static const NString kLocaleIsMetricKey								= "IsMetric";


// Locales
static const NString kNLocaleUser									= "*user*";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NLocale : public NBroadcaster,
				public NListener {
public:
										NLocale(const NString &theID=kNLocaleUser);
	virtual							   ~NLocale(void);


	// Is the measurement system metric?
	bool								IsMetric(void) const;


protected:
	// Handle messages
	void								DoMessage(BroadcastMsg theMsg, const void *msgData);


private:
	NVariant							GetValue(const NString &theKey) const;


private:
	NString								mID;
};





#endif // NLOCALE_HDR


