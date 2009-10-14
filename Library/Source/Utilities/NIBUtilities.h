/*	NAME:
		NIBUtilities.h

	DESCRIPTION:
		Interface Builder .nib utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
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
	static MenuRef						CreateMenu(  IBNibRef nibRef, const NString &resName);
	static HIWindowRef					CreateWindow(IBNibRef nibRef, const NString &resName);
	static HIViewRef					CreateView(  IBNibRef nibRef, const NString &resName);


	// Set the menu bar
	static void							SetMenuBar(IBNibRef nibRef, const NString &resName);


	// Set the error state
	//
	// IBCarbonRuntime outputs to stderr if a resource is not present (rdar://4773703).
	//
	// To allow callers to silently test for resources, the error stream state can be
	// temporarily adjusted before/after loading a optional resource.
	static SInt32						SetErrorState(SInt32 errorFD=-1);


private:
	static void							SendEventToViews(HIViewRef theView, const NCarbonEvent &theEvent);
};





#endif // NIBUTILITIES_HDR


