/*	NAME:
		NPasteboard.h

	DESCRIPTION:
		Pasteboard object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPASTEBOARD_HDR
#define NPASTEBOARD_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAutoReleasePool.h"
#include "NCarbonEvent.h"
#include "NCFObject.h"
#include "NCGImage.h"
#include "NString.h"
#include "NArray.h"
#include "NColor.h"
#include "NData.h"
#include "NFile.h"
#include "NUTI.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Pasteboard items
static const PasteboardItemID kPasteboardItemFirst						= NULL;
static const PasteboardItemID kPasteboardItemUnique						= NULL;
static const PasteboardItemID kPasteboardItemNone						= NULL;





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NPasteboard;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPasteboardPromiseKeeper {
public:
										 NPasteboardPromiseKeeper(void) { };
	virtual								~NPasteboardPromiseKeeper(void) { };


	// Provide the data for a promise
	//
	// Objects which wish to add promises to the clipboard should derive from
	// NPasteboardPromiseKeeper, assign themselves to the pasteboard prior to
	// adding the promise, and implement this method to honour the promise.
	virtual OSStatus					DoPasteboardPromise(const NPasteboard	*thePasteboard,
															PasteboardItemID	theItem,
															const NUTI			&theUTI) = 0;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPasteboard : public NCFObject {
public:
										NPasteboard(const NString &theName=kPasteboardClipboard);
										NPasteboard(NCarbonEvent  &dragEvent);
										NPasteboard(DragRef       theDrag);

	virtual								~NPasteboard(void);


	// Clear the pasteboard
	void								Clear(void);


	// Get/set the paste location
	NFile								GetPasteLocation(void) const;
	void								SetPasteLocation(const NFile &theFile);


	// Get/set the promise keeper
	NPasteboardPromiseKeeper			*GetPromiseKeeper(void) const;
	void								 SetPromiseKeeper(NPasteboardPromiseKeeper *promiseKeeper);


	// Does the pasteboard contain an item with the specified flavor?
	bool								HasFlavor(const NString &theFlavor) const;


	// Get the number of items
	UInt32								GetItemCount(void) const;


	// Get an item
	//
	// When getting an item by flavor, the first item that contains the flavor
	// is returned (or kPasteboardItemNone if no item contains the flavor).
	PasteboardItemID					GetItem(UInt32          theIndex) const;
	PasteboardItemID					GetItem(const NString &theFlavor) const;


	// Get the state of an item
	NArray								GetItemFlavors(PasteboardItemID theItem) const;
	PasteboardFlavorFlags				GetItemFlags(  PasteboardItemID theItem, const NString &theFlavor) const;


	// Get/add data for an item
	NData								GetItemData(PasteboardItemID theItem, const NString &theFlavor) const;
	void								AddItemData(PasteboardItemID theItem, const NString &theFlavor, const NData &theData, PasteboardFlavorFlags theFlags=kPasteboardFlavorNoFlags);


	// Get/add a value to the pasteboard
	//
	// When adding a value, multiple flavors of data will be added as appropriate.
	// The data will be added to the specified item, or to a new item if theItem
	// is kPasteboardItemUnique.
	//
	// When getting a value, the highest-fidelity flavor will be used to obtain the
	// value from the specified item (or the first item, as appropriate). If the
	// item does not contain suitable flavors, a 0'd result will be returned.
	NString								GetValueText( PasteboardItemID theItem=kPasteboardItemFirst) const;
	NColor								GetValueColor(PasteboardItemID theItem=kPasteboardItemFirst) const;
	NFile								GetValueFile( PasteboardItemID theItem=kPasteboardItemFirst) const;
	NCGImage							GetValueImage(PasteboardItemID theItem=kPasteboardItemFirst) const;

	void								AddValueText( const NString  &theValue, PasteboardItemID theItem=kPasteboardItemUnique, bool clearFirst=true);
	void								AddValueColor(const NColor   &theValue, PasteboardItemID theItem=kPasteboardItemUnique, bool clearFirst=true);
	void								AddValueFile( const NFile    &theValue, PasteboardItemID theItem=kPasteboardItemUnique, bool clearFirst=true);
	void								AddValueImage(const NCGImage &theValue, PasteboardItemID theItem=kPasteboardItemUnique, bool clearFirst=true);


private:
	void								InitializeSelf(PasteboardRef thePasteboard);

	PasteboardItemID					PrepareToGet(PasteboardItemID theItem) const;
	PasteboardItemID					PrepareToAdd(PasteboardItemID theItem, bool clearFirst);
	
	static OSStatus						PromiseKeeperCallback(PasteboardRef			thePasteboard,
																PasteboardItemID	theItem,
																CFStringRef			cfUTI,
																void				*theContext);



private:
	NPasteboardPromiseKeeper			*mPromiseKeeper;
	NAutoReleasePool					mPool;
};





#endif // NPASTEBOARD_HDR


