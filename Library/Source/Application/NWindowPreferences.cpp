/*	NAME:
		NWindowPreferences.cpp

	DESCRIPTION:
		Preferences window.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIToolbar.h"
#include "NHIViewUtilities.h"
#include "NWindowPreferences.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Toolbar IDs
static const NString kToolbarPreferencesID							= "com.refnum.nano.toolbar.preferences";
static const NString kToolbarItemFirstID							= kToolbarPreferencesID + ".";


// Commands
static const UInt32 kToolbarCommandFirst							= 'pt00';





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class NToolbarPreferences : public NHIToolbar {
public:
										NToolbarPreferences(const PrefPanelInfoList *thePanels);
	virtual								~NToolbarPreferences(void);


protected:
	// Get the toolbar contents
	HIToolbarItemInfoList				GetToolbarContents(void);


private:
	const PrefPanelInfoList				*mPanels;
};





//============================================================================
//		NToolbarPreferences::NToolbarPreferences : Constructor.
//----------------------------------------------------------------------------
NToolbarPreferences::NToolbarPreferences(const PrefPanelInfoList *thePanels)
				: NHIToolbar(kToolbarPreferencesID)
{


	// Validate our parameters
	NN_ASSERT(thePanels != NULL);
	NN_ASSERT(!thePanels->empty());



	// Initialize ourselves
	mPanels = thePanels;
}





//============================================================================
//		NToolbarPreferences::~NToolbarPreferences : Destructor.
//----------------------------------------------------------------------------
NToolbarPreferences::~NToolbarPreferences(void)
{
}





//============================================================================
//		NToolbarPreferences::GetToolbarContents : Get the toolbar contents.
//----------------------------------------------------------------------------
HIToolbarItemInfoList NToolbarPreferences::GetToolbarContents(void)
{	HIToolbarItemInfo				theItem = { kToolbarItemFirstID };
	UInt32							n, numItems;
	PrefPanelInfo					theInfo;
	HIToolbarItemInfoList			theList;



	// Get the items
	numItems = mPanels->size();
	
	for (n = 0; n < numItems; n++)
		{
		theInfo = mPanels->at(n);
		
		theItem.theIdentifier = kToolbarItemFirstID  + theInfo.keyLabel;
		theItem.theCommand    = kToolbarCommandFirst + n;
		theItem.resImage      = theInfo.resImage;
		theItem.keyLabel      = theInfo.keyLabel;
		theItem.theBehaviour  = kHIToolbarItemBehaviourInConfig | kHIToolbarItemBehaviourIsDefault | kHIToolbarItemBehaviourIsSelectable;

		theList.push_back(theItem);
		}

	return(theList);
}





//============================================================================
//		NWindowPreferences::NWindowPreferences : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NWindowPreferences::NWindowPreferences(void)
{
}





//============================================================================
//		NWindowPreferences::~NWindowPreferences : Destructor.
//----------------------------------------------------------------------------
NWindowPreferences::~NWindowPreferences(void)
{
}





//============================================================================
//		NWindowPreferences::SetPanels : Set the panels.
//----------------------------------------------------------------------------
#pragma mark -
void NWindowPreferences::SetPanels(const PrefPanelInfoList &thePanel)
{	NHIView			*theContainer;
	NHIToolbar		*theToolbar;
	NHIWindow		*theWindow;
	HIRect			theRect;



	// Validate our parameters and state
	NN_ASSERT(!thePanel.empty());
	NN_ASSERT(GetPanelNib() != NULL);



	// Get the state we need
	theWindow      = GetWindow();
	theRect.origin = kHIPointZero;
	theRect.size   = theWindow->GetFrame().size;



	// Initialize ourselves
	mPanels = thePanel;



	// Register for events
	NCFPreferences::GetPrefs()->AddListener(this);



	// Create the container
	theContainer = NHIViewUtilities::CreateHIView(theWindow->GetHIWindow(), theRect);

	theContainer->SetFeatures(kHIViewFeatureAllowsSubviews);
	SetPanelContainer(theContainer);



	// Attach the toolbar
	theToolbar = new NToolbarPreferences(&mPanels);
	theToolbar->Attach(theWindow);



	// Set the panels
	SetPanel(mPanels[0].resPanel);
	theToolbar->GetToolbarItems().front()->SetAttributes(kHIToolbarItemSelected);
}





//============================================================================
//		NWindowPreferences::DoMessage : Handle messages.
//----------------------------------------------------------------------------
#pragma mark -
void NWindowPreferences::DoMessage(BroadcastMsg theMsg, void *msgData)
{	const NString	*msgString = (const NString *) msgData;



	// Handle the message
	switch (theMsg) {
		case kMsgPreferenceValueChanged:
			DoPrefChanged(NCFPreferences::GetPrefs(), *msgString);
			break;

		default:
			NWindowController::DoMessage(theMsg, msgData);
			break;
		}
}





//============================================================================
//		NWindowPreferences::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus NWindowPreferences::DoEventCommandProcess(NCarbonEvent &theEvent)
{	UInt32				n, firstCmd, lastCmd;
	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Get the state we need
	firstCmd = kToolbarCommandFirst;
	lastCmd  = kToolbarCommandFirst + mPanels.size() - 1;



	// Handle the command
	hiCmd  = theEvent.GetHICommand();
	theErr = noErr;
	
	if (hiCmd.commandID >= firstCmd && hiCmd.commandID <= lastCmd)
		{
		n = hiCmd.commandID - firstCmd;
		SetPanel(mPanels[n].resPanel);
		}

	else
		theErr = NWindowController::DoEventCommandProcess(theEvent);

	return(theErr);
}





//============================================================================
//		NWindowPreferences::DoPanelChanged : The panel has changed.
//----------------------------------------------------------------------------
void NWindowPreferences::DoPanelChanged(void)
{


	// Update our state
	//
	// We re-find the views to resolve any NIBViews in the new panel, then
	// perform a forced update of the preferences to bring the UI in sync.
	FindViews();

	DoPrefChanged(NCFPreferences::GetPrefs(), kPrefChangedAllKey);
}





//============================================================================
//		NWindowPreferences::DoPrefChanged : Handle preference changes.
//----------------------------------------------------------------------------
void NWindowPreferences::DoPrefChanged(NCFPreferences */*thePrefs*/, const NString &/*theKey*/)
{
}



