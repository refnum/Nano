/*	NAME:
		NIBUtilities.h

	DESCRIPTION:
		Interface Builder .nib utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NIBUTILITIES_HDR
#define NIBUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCarbonEvent.h"
#include "NString.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NIBUtilities {
public:
	// Create a .nib reference
	static IBNibRef						CreateNib(const NString &nibName, CFBundleRef cfBundle=NULL);


	// Create a .nib resource
	static MenuRef						CreateMenu(  IBNibRef nibRef, const NString &resName, bool canFail=false);
	static HIWindowRef					CreateWindow(IBNibRef nibRef, const NString &resName, bool canFail=false);
	static HIViewRef					CreateView(  IBNibRef nibRef, const NString &resName, bool canFail=false);


	// Set the menu bar
	static void							SetMenuBar(IBNibRef nibRef, const NString &resName);


private:
	static void							SendEventToViews(HIViewRef theView, const NCarbonEvent &theEvent);
};





#endif // NIBUTILITIES_HDR


