/*	NAME:
		TNetworkBrowser.h

	DESCRIPTION:
		NNetworkBrowser tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef TNETWORKBROWSER_HDR
#define TNETWORKBROWSER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNetworkBrowser.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class TNetworkBrowser {
public:
	// Execute the tests
	static void							Execute(void);


private:
	static void							BrowseEvent(const NNetworkBrowserEvent &theEvent, NString *theResult);
};





#endif // TNETWORKBROWSER_HDR
