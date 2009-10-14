/*	NAME:
		NTabbedView.h

	DESCRIPTION:
		Tabbed view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTABBEDVIEW_HDR
#define NTABBEDVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NHIView.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Tab info
typedef struct {
	bool		isEnabled;
	NString		theName;
	NIcon		theIcon;
} TabInfo;


// Lists
typedef std::vector<TabInfo>								TabInfoList;
typedef TabInfoList::iterator								TabInfoListIterator;
typedef TabInfoList::const_iterator							TabInfoListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTabbedView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NTabbedView);

										NTabbedView(HIWindowRef					theWindow,
														const HIRect			&theRect,
														const TabInfoList		&theTabs,
														ControlTabSize			theSize      = kControlTabSizeLarge,
														ControlTabDirection		theDirection = kControlTabDirectionNorth);
	virtual								~NTabbedView(void);


	// Get the number of tabs
	UInt32								GetTabCount(void) const;


	// Get/set a tab's info
	TabInfo								GetTabInfo(UInt32 theIndex) const;
	void								SetTabInfo(UInt32 theIndex, const TabInfo &theInfo);


	// Get/set a tab's enabled state
	bool								IsTabEnabled( UInt32 theIndex) const;
	void								SetTabEnabled(UInt32 theIndex, bool isEnabled);
	

	// Get/set a tab's name
	NString								GetTabName(UInt32 theIndex) const;
	void								SetTabName(UInt32 theIndex, const NString &theValue);


	// Get/set a tab's icon
	NIcon								GetTabIcon(UInt32 theIndex) const;
	void								SetTabIcon(UInt32 theIndex, const NIcon &theIcon);


private:


};





#endif // NTABBEDVIEW_HDR


