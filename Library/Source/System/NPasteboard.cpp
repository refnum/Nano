/*	NAME:
		NPasteboard.cpp

	DESCRIPTION:
		Pasteboard object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NFileUtilities.h"
#include "NPasteboard.h"





//============================================================================
//		NPasteboard::NPasteboard : Constructor.
//----------------------------------------------------------------------------
NPasteboard::NPasteboard(const NString &theName)
{	PasteboardRef	thePasteboard;
	OSStatus		theErr;



	// Create the pasteboard
	thePasteboard = NULL;
	
	if (theName.IsEmpty())
		theErr = PasteboardCreate(NULL,    &thePasteboard);
	else
		theErr = PasteboardCreate(theName, &thePasteboard);
	
	NN_ASSERT_NOERR(theErr);



	// Initialize ourselves
	//
	// We do not release the object when we initialize ourselves, so
	// we need to release it here to avoid leaving an extra reference.
	InitializeSelf(thePasteboard);
	
	CFSafeRelease(thePasteboard);
}





//============================================================================
//		NPasteboard::NPasteboard : Constructor.
//----------------------------------------------------------------------------
NPasteboard::NPasteboard(NCarbonEvent &theEvent)
{	PasteboardRef	thePasteboard;
	DragRef			theDrag;
	OSStatus		theErr;



	// Get the pasteboard
	thePasteboard = NULL;
	
	theDrag = theEvent.GetParameterDrag(kEventParamDragRef);
	theErr  = GetDragPasteboard(theDrag, &thePasteboard);
	NN_ASSERT_NOERR(theErr);



	// Initialize the pasteboard
	InitializeSelf(thePasteboard);
}





//============================================================================
//		NPasteboard::NPasteboard : Constructor.
//----------------------------------------------------------------------------
NPasteboard::NPasteboard(DragRef theDrag)
{	PasteboardRef	thePasteboard;
	OSStatus		theErr;



	// Get the pasteboard
	thePasteboard = NULL;

	theErr = GetDragPasteboard(theDrag, &thePasteboard);
	NN_ASSERT_NOERR(theErr);



	// Initialize the pasteboard
	InitializeSelf(thePasteboard);
}





//============================================================================
//		NPasteboard::~NPasteboard : Destructor.
//----------------------------------------------------------------------------
NPasteboard::~NPasteboard(void)
{
}





//============================================================================
//		NPasteboard::Clear : Clear the pasteboard.
//----------------------------------------------------------------------------
void NPasteboard::Clear(void)
{	OSStatus	theErr;



	// Clear the pasteboard
	theErr = PasteboardClear(*this);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NPasteboard::GetPasteLocation : Get the paste location.
//----------------------------------------------------------------------------
NFile NPasteboard::GetPasteLocation(void) const
{	NCFURL		theURL;
	OSStatus	theErr;
	CFURLRef	cfURL;



	// Get the paste location
	cfURL  = NULL;
	theErr = PasteboardCopyPasteLocation(*this, &cfURL);
	NN_ASSERT_NOERR(theErr);
	
	if (cfURL != NULL)
		theURL.Set(cfURL);
	
	return(theURL.GetFile());
}





//============================================================================
//		NPasteboard::SetPasteLocation : Set the paste location.
//----------------------------------------------------------------------------
void NPasteboard::SetPasteLocation(const NFile &theFile)
{	OSStatus	theErr;



	// Set the paste location
	theErr = PasteboardSetPasteLocation(*this, theFile.GetURL());
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NPasteboard::GetPromiseKeeper : Get the promise keeper.
//----------------------------------------------------------------------------
NPasteboardPromiseKeeper *NPasteboard::GetPromiseKeeper(void) const
{


	// Get the promise keeper
	return(mPromiseKeeper);
}





//============================================================================
//		NPasteboard::SetPromiseKeeper : Set the promise keeper.
//----------------------------------------------------------------------------
void NPasteboard::SetPromiseKeeper(NPasteboardPromiseKeeper *promiseKeeper)
{	OSStatus	theErr;



	// Set the promise keeper
	mPromiseKeeper = promiseKeeper;



	// Update the pasteboard
	theErr = PasteboardSetPromiseKeeper(*this, PromiseKeeperCallback, this);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NPasteboard::HasFlavor : Does the pasteboard contain a flavor?
//----------------------------------------------------------------------------
bool NPasteboard::HasFlavor(const NString &theFlavor) const
{	PasteboardItemID	theItem;



	// Find an item with the flavor
	theItem = GetItem(theFlavor);
	
	return(theItem != kPasteboardItemNone);
}





//============================================================================
//		NPasteboard::GetItemCount : Get the number of items on the pasteboard.
//----------------------------------------------------------------------------
UInt32 NPasteboard::GetItemCount(void) const
{	ItemCount	theValue;
	OSStatus	theErr;



	// Synchronise the pasteboard
	PasteboardSynchronize(*this);



	// Get the item count
	theValue = 0;
	theErr   = PasteboardGetItemCount(*this, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NPasteboard::GetItem : Get an item from the pasteboard.
//----------------------------------------------------------------------------
PasteboardItemID NPasteboard::GetItem(UInt32 theIndex) const
{	PasteboardItemID	theItem;
	OSStatus			theErr;



	// Validate our parameters
	NN_ASSERT(theIndex < GetItemCount());



	// Synchronise the pasteboard
	PasteboardSynchronize(*this);



	// Get the item
	theItem = NULL;
	theErr  = PasteboardGetItemIdentifier(*this, theIndex + 1, &theItem);
	NN_ASSERT_NOERR(theErr);
	
	return(theItem);
}





//============================================================================
//		NPasteboard::GetItem : Get an item from the pasteboard.
//----------------------------------------------------------------------------
PasteboardItemID NPasteboard::GetItem(const NString &theFlavor) const
{	UInt32				n, m, numItems, numFlavors;
	NUTI				itemFlavor;
	NArray				theFlavors;
	PasteboardItemID	theItem;



	// Find the item
	//
	// Any time on the pasteboard that is equal to, or conforms to, the flavor
	// is assumed to be an aceptable match (we may want to provide a "strict
	// match" here, and on HasFlavor, where only exact matches are accepted).
	numItems = GetItemCount();
	
	for (n = 0; n < numItems; n++)
		{
		// Get the state we need
		theItem    = GetItem(n);
		theFlavors = GetItemFlavors(theItem);
		numFlavors = theFlavors.GetSize();
		
		
		
		// Check the flavors
		for (m = 0; m < numFlavors; m++)
			{
			itemFlavor = theFlavors.GetValueString(m);
			
			if (itemFlavor.ConformsTo(theFlavor))
				return(theItem);
			}
		}
	
	return(kPasteboardItemNone);
}





//============================================================================
//		NPasteboard::GetItemFlavors : Get an item's flavors.
//----------------------------------------------------------------------------
NArray NPasteboard::GetItemFlavors(PasteboardItemID theItem) const
{	NArray			theArray;
	CFArrayRef		cfArray;
	OSStatus		theErr;



	// Validate our parameters
	NN_ASSERT(theItem != NULL);
	


	// Synchronise the pasteboard
	PasteboardSynchronize(*this);



	// Get the item's flavors
	cfArray = NULL;
	theErr  = PasteboardCopyItemFlavors(*this, theItem, &cfArray);
	NN_ASSERT_NOERR(theErr);
	
	if (cfArray != NULL)
		theArray.Set(cfArray);
	
	return(theArray);
}





//============================================================================
//		NPasteboard::GetItemFlags : Get an item's flavor flags.
//----------------------------------------------------------------------------
PasteboardFlavorFlags NPasteboard::GetItemFlags(PasteboardItemID theItem, const NString &theFlavor) const
{	PasteboardFlavorFlags	theFlags;
	OSStatus				theErr;



	// Validate our parameters
	NN_ASSERT(theItem != NULL);
	NN_ASSERT(theFlavor.IsNotEmpty());
	


	// Synchronise the pasteboard
	PasteboardSynchronize(*this);



	// Get the item's flavor flags
	theFlags = kPasteboardFlavorNoFlags;
	theErr   = PasteboardGetItemFlavorFlags(*this, theItem, theFlavor, &theFlags);
	NN_ASSERT_NOERR(theErr);
	
	return(theFlags);
}





//============================================================================
//		NPasteboard::GetItemData : Get data from an item.
//----------------------------------------------------------------------------
NData NPasteboard::GetItemData(PasteboardItemID theItem, const NString &theFlavor) const
{	NData			theData;
	CFDataRef		cfData;
	OSStatus		theErr;



	// Validate our parameters
	NN_ASSERT(theItem != NULL);
	NN_ASSERT(theFlavor.IsNotEmpty());



	// Synchronise the pasteboard
	PasteboardSynchronize(*this);



	// Resolve any promises
	theErr = PasteboardResolvePromises(*this);
	NN_ASSERT_NOERR(theErr);



	// Get the data
	cfData = NULL;
	theErr = PasteboardCopyItemFlavorData(*this, theItem, theFlavor, &cfData);
	NN_ASSERT_NOERR(theErr);
	
	if (cfData != NULL)
		theData.Set(cfData);

	return(theData);
}





//============================================================================
//		NPasteboard::AddItemData : Add data to an item.
//----------------------------------------------------------------------------
void NPasteboard::AddItemData(PasteboardItemID			theItem,
								const NString			&theFlavor,
								const NData				&theData,
								PasteboardFlavorFlags	theFlags)
{	CFDataRef	cfData;
	OSStatus	theErr;



	// Validate our parameters and state
	NN_ASSERT(theItem != NULL);
	NN_ASSERT(theFlavor.IsNotEmpty());
	
	if (theFlags & kPasteboardFlavorPromised)
		NN_ASSERT(mPromiseKeeper != NULL);



	// Prepare the data
	cfData = NULL;
	
	if (theData.GetSize() != 0)
		cfData = theData;



	// Synchronise the pasteboard
	PasteboardSynchronize(*this);



	// Add the data
	theErr = PasteboardPutItemFlavor(*this, theItem, theFlavor, cfData, theFlags);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NPasteboard::GetValueColor : Get a color from the pasteboard.
//----------------------------------------------------------------------------
NColor NPasteboard::GetValueColor(PasteboardItemID theItem) const
{	UInt32			n, numFlavors;
	NArray			theFlavors;
	NUTI			theFlavor;
	NColor			theColor;
	NSColor			*nsColor;
	NData			theData;



	// Get the state we need
	theItem = PrepareToGet(theItem);
	nsColor = NULL;



	// Find the most suitable flavor
	theFlavors = GetItemFlavors(theItem);
	numFlavors = theFlavors.GetSize();

	for (n = 0; n < numFlavors && nsColor == NULL; n++)
		{
		theFlavor = theFlavors.GetValueString(n);

		if (theFlavor.EqualTo(kUTNTypeColor))
			{
			theData  = GetItemData(theItem, theFlavor);
			nsColor  = (NSColor *) [NSKeyedUnarchiver unarchiveObjectWithData:(NSData *) ((CFDataRef) theData)];

			if (nsColor != NULL)
				{
				theColor.SetRed(  [nsColor   redComponent]);
				theColor.SetGreen([nsColor greenComponent]);
				theColor.SetBlue( [nsColor  blueComponent]);
				theColor.SetAlpha([nsColor alphaComponent]);
				}
			}
		}
	
	return(theColor);
}





//============================================================================
//		NPasteboard::GetValueText : Get a string from the pasteboard.
//----------------------------------------------------------------------------
NString NPasteboard::GetValueText(PasteboardItemID theItem) const
{	UInt32				n, numFlavors;
	CFStringEncoding	theEncoding;
	NArray				theFlavors;
	NUTI				theFlavor;
	NString				theValue;
	NData				theData;



	// Get the state we need
	theItem     = PrepareToGet(theItem);
	theEncoding = kCFStringEncodingInvalidId;



	// Find the most suitable flavor
	theFlavors = GetItemFlavors(theItem);
	numFlavors = theFlavors.GetSize();

	for (n = 0; n < numFlavors && theEncoding == kCFStringEncodingInvalidId; n++)
		{
		theFlavor = theFlavors.GetValueString(n);
		
		if (theFlavor.ConformsTo(kUTNTypeUTF16PlainText))
			{
			theData     = GetItemData(theItem, theFlavor);
			theEncoding = kCFStringEncodingUTF16;
			}

		else if (theFlavor.ConformsTo(kUTNTypeUTF8PlainText))
			{
			theData     = GetItemData(theItem, theFlavor);
			theEncoding = kCFStringEncodingUTF8;
			}
		}



	// Get the value
	if (theEncoding != kCFStringEncodingInvalidId)
		theValue = NString((const char *) theData.GetData(), theData.GetSize(), theEncoding);
	
	return(theValue);
}





//============================================================================
//		NPasteboard::GetValueFile : Get a file from the pasteboard.
//----------------------------------------------------------------------------
NFile NPasteboard::GetValueFile(PasteboardItemID theItem) const
{	UInt32				n, numFlavors;
	NArray				theFlavors;
	NUTI				theFlavor;
	NFile				theFile;
	NData				theData;
	NCFURL				theURL;



	// Get the state we need
	theItem = PrepareToGet(theItem);



	// Find the most suitable flavor
	theFlavors = GetItemFlavors(theItem);
	numFlavors = theFlavors.GetSize();

	for (n = 0; n < numFlavors && !theFile.IsValid(); n++)
		{
		theFlavor = theFlavors.GetValueString(n);
		
		if (theFlavor.ConformsTo(kUTNTypeFileURL))
			{
			theData = GetItemData(theItem, theFlavor);
			theURL  = theData;
			
			theFile.SetURL(theURL);
			}
		}

	return(theFile);
}





//============================================================================
//		NPasteboard::GetValueImage : Get an image from the pasteboard.
//----------------------------------------------------------------------------
NCGImage NPasteboard::GetValueImage(PasteboardItemID theItem) const
{	UInt32			n, numFlavors;
	NArray			theFlavors;
	NUTI			theFlavor;
	NCGImage		theImage;
	NData			theData;



	// Get the state we need
	theItem = PrepareToGet(theItem);



	// Find the most suitable flavor
	theFlavors = GetItemFlavors(theItem);
	numFlavors = theFlavors.GetSize();

	for (n = 0; n < numFlavors && !theImage.IsValid(); n++)
		{
		theFlavor = theFlavors.GetValueString(n);
		
		if (theFlavor.ConformsTo(kUTNTypeImage))
			{
			theData  = GetItemData(theItem, theFlavor);
			theImage = NCoreGraphicsUtilities::CreateImageFromData(theData, theFlavor);
			}
		}
	
	return(theImage);
}





//============================================================================
//		NPasteboard::AddValueColor : Add a color to the pasteboard.
//----------------------------------------------------------------------------
void NPasteboard::AddValueColor(const NColor &theValue, PasteboardItemID theItem, bool clearFirst)
{	NSData		*nsData;
	NData		theData;



	// Prepare the value
	nsData = [NSKeyedArchiver archivedDataWithRootObject:[NSColor colorWithCalibratedRed:theValue.GetRed()
																				   green:theValue.GetGreen()
																					blue:theValue.GetBlue()
																				   alpha:theValue.GetAlpha()]];



	// Add the value
	theItem = PrepareToAdd(theItem, clearFirst);
	
	if (theData.SetData([nsData length], [nsData bytes]))
		AddItemData(theItem, kUTNTypeColor, theData);
}





//============================================================================
//		NPasteboard::AddValueText : Add a string to the pasteboard.
//----------------------------------------------------------------------------
void NPasteboard::AddValueText(const NString &theValue, PasteboardItemID theItem, bool clearFirst)
{	NData	textUTF16, textUTF8;



	// Prepare the values
	if (!textUTF16.SetData(theValue.GetSize(), theValue.GetUTF16()) ||
		! textUTF8.SetData(theValue.GetSize(), theValue.GetUTF8()))
		return;



	// Add the values
	theItem = PrepareToAdd(theItem, clearFirst);
	
	AddItemData(theItem, kUTNTypeUTF16PlainText, textUTF16);
	AddItemData(theItem, kUTNTypeUTF8PlainText,  textUTF8);
}





//============================================================================
//		NPasteboard::AddValueFile : Add a file to the pasteboard.
//----------------------------------------------------------------------------
void NPasteboard::AddValueFile(const NFile &theValue, PasteboardItemID theItem, bool clearFirst)
{	NString		theText;
	NData		theData;



	// Prepare the values
	theText = theValue.GetURL().GetString();
	if (!theData.SetData(theText.GetSize(), theText.GetUTF8()))
		return;



	// Add the values
	theItem = PrepareToAdd(theItem, clearFirst);

	AddItemData(theItem, kUTNTypeFileURL, theData);
	AddValueText(theValue.GetFileName(), theItem, false);
}





//============================================================================
//		NPasteboard::AddValueImage : Add an image to the pasteboard.
//----------------------------------------------------------------------------
void NPasteboard::AddValueImage(const NCGImage &theValue, PasteboardItemID theItem, bool clearFirst)
{	NData	imagePNG, imagePICT;



	// Prepare the values
	//
	// Typically PICT data is placed on the clipboard without the PICT file header.
	imagePNG  = NCoreGraphicsUtilities::CreateDataFromImage(theValue, kUTNTypePNG);
	imagePICT = NCoreGraphicsUtilities::CreateDataFromImage(theValue, kUTNTypePICT);

	imagePICT.ReplaceData(CFRangeMake(0, kFileHeaderSizePICT), 0, NULL);



	// Add the values
	theItem = PrepareToAdd(theItem, clearFirst);
	
	AddItemData(theItem, kUTNTypePNG,   imagePNG);
	AddItemData(theItem, kUTNTypePICT, imagePICT);
}





//============================================================================
//		NPasteboard::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NPasteboard::InitializeSelf(PasteboardRef thePasteboard)
{


	// Validate our parameters
	NN_ASSERT(thePasteboard != NULL);



	// Initialize ourselves
	mPromiseKeeper = NULL;

	Set(thePasteboard, false);
}





//============================================================================
//		NPasteboard::PrepareToGet : Prepare to get an item.
//----------------------------------------------------------------------------
PasteboardItemID NPasteboard::PrepareToGet(PasteboardItemID theItem) const
{


	// Update the item ID
	if (theItem == kPasteboardItemFirst)
		{
		NN_ASSERT(GetItemCount() >= 1);

		theItem = GetItem(0);
		}

	return(theItem);
}





//============================================================================
//		NPasteboard::PrepareToAdd : Prepare to add an item.
//----------------------------------------------------------------------------
PasteboardItemID NPasteboard::PrepareToAdd(PasteboardItemID theItem, bool clearFirst)
{	static UInt32		sNextItemID = 1;



	// Update the item ID
	if (theItem == kPasteboardItemUnique)
		theItem = (PasteboardItemID) sNextItemID++;



	// Clear the pasteboard
	if (clearFirst)
		Clear();
	
	return(theItem);
}





//============================================================================
//		NPasteboard::PromiseKeeperCallback : Promise keeper callback.
//----------------------------------------------------------------------------
OSStatus NPasteboard::PromiseKeeperCallback(PasteboardRef		/*thePasteboard*/,
											PasteboardItemID	theItem,
											CFStringRef			cfUTI,
											void				*theContext)
{	NPasteboard		*thisPtr = (NPasteboard *) theContext;
	OSStatus		theErr   = paramErr;



	// Resolve the promise
	if (thisPtr->mPromiseKeeper != NULL)
		theErr = thisPtr->mPromiseKeeper->DoPasteboardPromise(thisPtr, theItem, cfUTI);

	return(theErr);
}




