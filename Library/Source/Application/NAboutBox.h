/*	NAME:
		NAboutBox.h

	DESCRIPTION:
		Nano about box.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NABOUTBOX_HDR
#define NABOUTBOX_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCarbonEventHandler.h"
#include "NWindowController.h"
#include "NReflectionView.h"
#include "NPushButton.h"
#include "NShadingView.h"
#include "NStaticText.h"
#include "NIconView.h"
#include "NDictionary.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Properties
//
// All keys are optional. If the dialog is modal, one or more buttons must be
// supplied to close the dialog.
static const NString kAboutTitleKey									= "AboutTitle";
static const NString kAboutProductKey								= "AboutProduct";
static const NString kAboutVersionKey								= "AboutVersion";
static const NString kAboutCopyrightKey								= "AboutCopyright";
static const NString kAboutDescriptionKey							= "AboutDescription";
static const NString kAboutButton1TitleKey							= "AboutButton1Title";
static const NString kAboutButton2TitleKey							= "AboutButton2Title";
static const NString kAboutButton3TitleKey							= "AboutButton3Title";
static const NString kAboutButton1URLKey							= "AboutButton1URL";
static const NString kAboutButton2URLKey							= "AboutButton2URL";
static const NString kAboutButton3URLKey							= "AboutButton3URL";
static const NString kAboutIsModalKey								= "AboutIsModal";


// Commands
//
// The about window will include a push button for each kAboutButton<N>Key in
// the window properties, which will dispatch the corresponding command to the
// application when selected.
//
// If a button has an associated URL associated, NAboutBox will process the
// command for the application and launch the URL.
static const UInt32 kAboutCommandButton1							= 'abo1';
static const UInt32 kAboutCommandButton2							= 'abo2';
static const UInt32 kAboutCommandButton3							= 'abo3';





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NAboutBox : public NWindowController {
public:
										DECLARE_WINDOW_CONTROLLER(NAboutBox);

										NAboutBox(void);
	virtual								~NAboutBox(void);


	// Do the dialog
	//
	// If the kAboutIsModalKey property is set to true, the dialog
	// will be a modal dialog that must be closed with EndDialog.
	void								DoDialog(void);


	// End the dialog
	void								EndDialog(void);
	

	// Get the properties dictionary
	NDictionary							*GetProperties(void);


protected:
	// Initialize ourselves
	OSStatus							InitializeSelf(void);


	// Handle events
	OSStatus							DoEventCommandProcess(NCarbonEvent &theEvent);


private:
	void								PrepareWindow(void);

	NString								GetProductText(const NString &theSuffix="");
	OSStatus							DoAboutButton( const NString &theURL);


private:
	NDictionary							mProperties;

	NIB_VIEW('icon', NIconView,			AppIcon);
	NIB_VIEW('refl', NReflectionView,	Reflection);
	NIB_VIEW('shad', NShadingView,		Shading);
	NIB_VIEW('prod', NStaticText,		TextProduct);
	NIB_VIEW('copy', NStaticText,		TextCopyright);
	NIB_VIEW('desc', NStaticText,		TextDescription);
	NIB_VIEW('but1', NPushButton,		Button1);
	NIB_VIEW('but2', NPushButton,		Button2);
	NIB_VIEW('but3', NPushButton,		Button3);
};





#endif // NABOUTBOX_HDR


