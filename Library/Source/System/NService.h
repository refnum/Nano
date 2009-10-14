/*	NAME:
		NService.h

	DESCRIPTION:
		Service support.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSERVICE_HDR
#define NSERVICE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NString kServiceSearchDictionary					= "Look Up in Dictionary";
static const NString kServiceSearchSpotlight					= "Spotlight";
static const NString kServiceSearchGoogle						= "Search With Google";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NService {
public:
										NService(const NString &theService, const NString &theValue);

										NService( void);
	virtual								~NService(void);


	// Get/set the service
	NString								GetService(void) const;
	void								SetService(const NString &theService);


	// Get/set the value
	NString								GetValue(void) const;
	void								SetValue(const NString &theValue);


	// Execute the service
	OSStatus							Execute(void) const;


private:
	NString								mService;
	NString								mValue;
};




#endif // NSERVICE_HDR


