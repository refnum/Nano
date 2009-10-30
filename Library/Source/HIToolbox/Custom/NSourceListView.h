/*	NAME:
		NSourceListView.h

	DESCRIPTION:
		Source list view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NSOURCELISTVIEW_HDR
#define NSOURCELISTVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NMsgBroadcaster.h"
#include "NSourceListItem.h"
#include "NScrollableView.h"
#include "NCGShading.h"
#include "NCGContext.h"
#include "NEditText.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgSourceItemRemoved						= 'sirm';
static const BroadcastMsg kMsgSourceItemRenamed						= 'sire';

static const BroadcastMsg kMsgSourceListChangedSelection			= 'ssel';


// Drag hilite
typedef enum {
	kDragHiliteNone,
	kDragHiliteList,
	kDragHiliteItem
} SourceListDragHilite;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Rows
typedef struct {
	HIRect				theRect;
	bool				isHeader;
	UInt32				theIndent;
	NSourceListItem		*theItem;
} SLRow;


// Lists
typedef std::vector<SLRow>											SLRowList;
typedef SLRowList::iterator											SLRowListIterator;
typedef SLRowList::const_iterator									SLRowListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSourceListDragDelegate {
public:
										 NSourceListDragDelegate(void) { }
	virtual								~NSourceListDragDelegate(void) { }


	// Handle source list drags
	virtual SourceListDragHilite		SLDragUpdate( const NDrag &theDrag, const NSourceListItem *theTarget) = 0;
	virtual DragActions					SLDragReceive(const NDrag &theDrag, const NSourceListItem *theTarget) = 0;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSourceListView : public NScrollableView, public NMsgBroadcaster {
public:
										DECLARE_HIVIEW_SUBCLASS(NSourceListView);

										NSourceListView(HIWindowRef theWindow, const HIRect &theRect);
	virtual								~NSourceListView(void);


	// Get an item
	const NSourceListItem				*GetItem(const NString &theID) const;


	// Get/set the items
	NSourceListItemList					GetItems(void) const;
	void								SetItems(const NSourceListItemList &theItems);


	// Get/set the selection
	NString								GetSelection(void) const;
	void								SetSelection(const NString &theID);


	// Remove the selection
	void								RemoveSelection(void);


	// Edit the selection
	void								EditSelection(void);


	// Get/set the drag delegate
	NSourceListDragDelegate				*GetDragDelegate(void) const;
	void								SetDragDelegate(NSourceListDragDelegate *theDelegate);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventCommandProcess(     NCarbonEvent &theEvent);
	OSStatus							DoEventControlBoundsChanged(NCarbonEvent &theEvent);
	OSStatus							DoEventControlGetFocusPart( NCarbonEvent &theEvent);
	OSStatus							DoEventControlSetFocusPart( NCarbonEvent &theEvent);
	OSStatus							DoEventRawKeyDown(         NCarbonEvent &theEvent);
	OSStatus							DoEventScrollableScrollTo( NCarbonEvent &theEvent);


	// Handle drags
	ThemeCursor							DoDragUpdate( const NDrag &theDrag, bool isWithin);
	DragActions							DoDragReceive(const NDrag &theDrag);


	// Handle mouse tracking
	bool								DoTrackMouse(NCarbonEvent				&theEvent,
														const HIPoint			&theMouse,
														MouseTrackingResult		theTrack);


	// Draw the view
	void								DrawBackground(NCarbonEvent &theEvent, const HIRect &theContent);
	void								DrawContent(   NCarbonEvent &theEvent);


	// Get the content bounds
	HIRect								GetContentBounds(void) const;


private:
	void								BuildRows( void);
	void								UpdateRows(void);
	
	void								AddRow(NSourceListItem *theItem, HIRect &theRect, UInt32 theIndent);
	void								RevealRow(UInt32 theIndex);

	UInt32								GetRowAtPoint(  const HIPoint &thePoint);
	UInt32								GetRowUnderDrag(const NDrag   &theDrag);

	void								SelectRow(const HIPoint &thePoint);
	void								SelectRow(SInt32 theDelta);
	
	void								EditBegin(void);
	void								EditEnd(bool cancelEdit);

	void								DrawDragHilite(NCGContext &cgContext, bool isUnderlay);
	void								DrawHeader(    NCGContext &cgContext, const SLRow &theRow);
	void								DrawRow(       NCGContext &cgContext, const SLRow &theRow, bool isSelected, bool isEditing);

	bool								CanRemove(UInt32 theIndex);
	bool								CanRename(UInt32 theIndex);
	bool								CanSelect(UInt32 theIndex);
	
	HIRect								GetIconRect(const SLRow &theRow);
	HIRect								GetTextRect(const SLRow &theRow);

	const NSourceListItem				*GetItemForID(const NString &theID, const NSourceListItemList *theItems) const;

	static void							DeleteView(NHIView *theView);
	

private:
	NSourceListItemList					mItems;
	SLRowList							mRows;

	NEditText							*mEditor;
	bool								mHasFocus;
	UInt32								mSelection;

	NSourceListDragDelegate				*mDragDelegate;
	SourceListDragHilite				mDragHilite;
	UInt32								mDragTarget;

	NCGShading							mShadingActive;
	NCGShading							mShadingInactive;
};





#endif // NSOURCELISTVIEW_HDR


