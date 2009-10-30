/*	NAME:
		NWindowPanels.cpp

	DESCRIPTION:
		Multi-panel window.
		
		A multi-panel window is a window-based NPanelController, which provides
		a toolbar to select the current panel.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIViewUtilities.h"
#include "NWindowPanels.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Toolbar IDs
static const NString kToolbarMultiPanelID							= "com.refnum.nano.toolbar.multipanel.0x%llX";


// Commands
static const UInt32 kToolbarCommandFirst							= 'pt00';





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class NToolbarPanel : public NHIToolbar {
public:
										NToolbarPanel(const PanelInfoList *thePanels);
	virtual								~NToolbarPanel(void);


protected:
	// Get the toolbar contents
	HIToolbarItemInfoList				GetToolbarContents(void);


private:
	NString								GetToolbarID(void);


private:
	const PanelInfoList					*mPanels;
};





//============================================================================
//		NToolbarPanel::NToolbarPanel : Constructor.
//----------------------------------------------------------------------------
NToolbarPanel::NToolbarPanel(const PanelInfoList *thePanels)
				: NHIToolbar(GetToolbarID(), kHIToolbarNoAttributes)
{


	// Validate our parameters
	NN_ASSERT(thePanels != NULL);
	NN_ASSERT(!thePanels->empty());



	// Initialize ourselves
	mPanels = thePanels;
}





//============================================================================
//		NToolbarPanel::~NToolbarPanel : Destructor.
//----------------------------------------------------------------------------
NToolbarPanel::~NToolbarPanel(void)
{
}





//============================================================================
//		NToolbarPanel::GetToolbarContents : Get the toolbar contents.
//----------------------------------------------------------------------------
HIToolbarItemInfoList NToolbarPanel::GetToolbarContents(void)
{	HIToolbarItemInfo				theItem = { "" };
	UInt32							n, numItems;
	PanelInfo						theInfo;
	HIToolbarItemInfoList			theList;



	// Get the items
	numItems = mPanels->size();
	
	for (n = 0; n < numItems; n++)
		{
		theInfo = mPanels->at(n);
		
		theItem.theIdentifier = GetToolbarID()  + "." + theInfo.keyLabel;
		theItem.theCommand    = kToolbarCommandFirst + n;
		theItem.resImage      = theInfo.resImage;
		theItem.keyLabel      = theInfo.keyLabel;
		theItem.theBehaviour  = kHIToolbarItemBehaviourInConfig  |
                                kHIToolbarItemBehaviourIsDefault;

		theList.push_back(theItem);
		}

	return(theList);
}





//============================================================================
//		NToolbarPanel::GetToolbarID : Get the toolbar ID.
//----------------------------------------------------------------------------
NString NToolbarPanel::GetToolbarID(void)
{	NString		theID;



	// Get the toolbar ID
	//
	// Although the toolbar state of a multi-panel window is not currently
	// saved to the preferences, we generate a unique ID for each window to
	// ensure different windows have different IDs.
	theID.Format(kToolbarMultiPanelID, (UInt64) this);
	
	return(theID);
}





//============================================================================
//		NWindowPanels::NWindowPanels : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NWindowPanels::NWindowPanels(void)
{


	// Initialize ourselves
	mToolbar = NULL;
}





//============================================================================
//		NWindowPanels::~NWindowPanels : Destructor.
//----------------------------------------------------------------------------
NWindowPanels::~NWindowPanels(void)
{
}





//============================================================================
//		NWindowPanels::SetPanels : Set the panels.
//----------------------------------------------------------------------------
void NWindowPanels::SetPanels(const PanelInfoList &thePanels)
{	NHIView			*theContainer;
	NHIWindow		*theWindow;
	HIRect			theRect;



	// Validate our parameters and state
	NN_ASSERT(!thePanels.empty());
	NN_ASSERT(GetPanelNib() != NULL);



	// Get the state we need
	theWindow      = GetWindow();
	theRect.origin = kHIPointZero;
	theRect.size   = theWindow->GetFrame().size;



	// Initialize ourselves
	mPanels = thePanels;



	// Create the container
	theContainer = NHIViewUtilities::CreateHIView(theWindow->GetHIWindow(), theRect);
	theContainer->SetFeatures(kHIViewFeatureAllowsSubviews);
	theContainer->SetLayoutInfo(NHIViewUtilities::GetLayoutInfo(kBindAllSides));

	SetPanelContainer(theContainer);



	// Attach the toolbar
    mToolbar = new NToolbarPanel(&mPanels);
    mToolbar->Attach(theWindow);



	// Set the panels
	SetPanel(mPanels[0].resPanel);
}





//============================================================================
//		NWindowPanels::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
#pragma mark -
CommandStatus NWindowPanels::GetCommandStatus(const HICommandExtended &hiCmd)
{	CommandStatus	theStatus;



	// Get the command status
	switch (hiCmd.commandID) {
		case kHICommandShowToolbar:
		case kHICommandHideToolbar:
		case kHICommandCustomizeToolbar:
		case kHICommandNToggleToolbar:
			// Our toolbar is immutable
			theStatus = kCommandDisable;
			break;

		default:
			theStatus = NWindowController::GetCommandStatus(hiCmd);
			break;
		}
	
	return(theStatus);
}





//============================================================================
//		NWindowPanels::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus NWindowPanels::DoEventCommandProcess(NCarbonEvent &theEvent)
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
//		NWindowPanels::DoPanelChanged : The panel has changed.
//----------------------------------------------------------------------------
void NWindowPanels::DoPanelChanged(void)
{	NHIToolbarItemList		theItems;
	UInt32					n;



	// Get the state we need
	theItems = mToolbar->GetToolbarItems();



	// Update the toolbar
	for (n = 0; n < mPanels.size(); n++)
		{
		if (mPanels[n].resPanel == GetPanel())
			theItems[n]->SetAttributes(   kHIToolbarItemSelected);
		else
			theItems[n]->SetAttributes(0, kHIToolbarItemSelected);
		}



    // Find the views
	//
	// Sub-classes will typically have NIBViews contained within them, so by
	// re-searching when a panel changes we will automatically connect them up.
	FindViews();
}


