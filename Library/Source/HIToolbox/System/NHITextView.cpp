/*	NAME:
		NHITextView.cpp

	DESCRIPTION:
		Text field view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileUtilities.h"
#include "NHITextView.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NHITextView, kHITextViewClassID, NULL);





//============================================================================
//		NHITextView::NHITextView : Constructor.
//----------------------------------------------------------------------------
NHITextView::NHITextView(HIWindowRef			theWindow,
							const HIRect		&theRect,
							const NString		&theValue,
							OptionBits			theOptions)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = HITextViewCreate(NULL, 0, theOptions, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);



	// Configure the view
	SetTextValue(theValue);
}





//============================================================================
//		NHITextView::~NHITextView : Destructor.
//----------------------------------------------------------------------------
NHITextView::~NHITextView(void)
{
}





//============================================================================
//		NHITextView::GetTXNObject : Get the TXNObject.
//----------------------------------------------------------------------------
TXNObject NHITextView::GetTXNObject(void) const
{


	// Get the TXNObject
	return(HITextViewGetTXNObject(*this));
}





//============================================================================
//		NHITextView::GetBackgroundColor : Get the background color.
//----------------------------------------------------------------------------
NColor NHITextView::GetBackgroundColor(void) const
{	NColor			theValue;
	CGColorRef		cgColor;
	OSStatus		theErr;



	// Get the background color
	theErr = HITextViewCopyBackgroundColor(*this, &cgColor);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr && cgColor != NULL)
		theValue.SetColor(cgColor);
	
	return(theValue);
}





//============================================================================
//		NHITextView::SetBackgroundColor : Set the background color.
//----------------------------------------------------------------------------
void NHITextView::SetBackgroundColor(const NColor &theColor)
{	NCFObject		cgColor;
	OSStatus		theErr;



	// Set the background color
	cgColor.Set(theColor.CopyColor());

	theErr = HITextViewSetBackgroundColor(*this, cgColor);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHITextView::IsReadOnly : Is the text read-only?
//----------------------------------------------------------------------------
bool NHITextView::IsReadOnly(void) const
{	TXNControlData	theData;



	// Get our state
	theData = GetTXNData(kTXNNoUserIOTag);
	
	return((bool) theData.uValue);
}





//============================================================================
//		NHITextView::SetReadOnly : Set the read-only state.
//----------------------------------------------------------------------------
void NHITextView::SetReadOnly(bool readOnly)
{	TXNControlData	theData;



	// Set the state
	theData.uValue = (UInt32) readOnly;
	SetTXNData(kTXNNoUserIOTag, theData);
}





//============================================================================
//		NHITextView::GetTextValue : Get the text value.
//----------------------------------------------------------------------------
//		Note :	We override NHIView's implementation, since HITextView only
//				supports kControlEditTextCFStringTag from Mac OS X 10.4.
//----------------------------------------------------------------------------
NString NHITextView::GetTextValue(void) const
{	NString			theValue;
	const UniChar	*textPtr;
	UInt32			textSize;
	NData			theData;



	// Get the value
	theData = GetTextData(kTXNUnicodeTextData);
	
	if (theData.IsValid())
		{
		textPtr  = (const UniChar *) theData.GetData();
		textSize = theData.GetSize() / sizeof(UniChar);
		
		theValue.Set(CFStringCreateWithCharacters(NULL, textPtr, textSize));
		}

	return(theValue);
}





//============================================================================
//		NHITextView::SetTextValue : Set the text value.
//----------------------------------------------------------------------------
//		Note :	We override NHIView's implementation, since HITextView only
//				supports kControlEditTextCFStringTag from Mac OS X 10.4.
//----------------------------------------------------------------------------
void NHITextView::SetTextValue(const NString &theValue)
{	const UniChar	*textPtr;
	UInt32			textSize;
	NData			theData;



	// Prepare the data
	textPtr  = theValue.GetUTF16();
	textSize = theValue.GetSize() * sizeof(UniChar);

	theData.AppendData(textSize, textPtr);



	// Set the value
	SetTextData(theData, kTXNUnicodeTextData);
}





//============================================================================
//		NHITextView::GetTextData : Get the text data.
//----------------------------------------------------------------------------
NData NHITextView::GetTextData(TXNDataType theFormat) const
{	NData			theData;
	CFDataRef		cfData;
	OSStatus		theErr;



	// Get the data
	//
	// TXNFlattenObjectToCFDataRef does not follow the CF model of Get/Copy/Create,
	// and we are assumed to take ownership of the returned string. 
	cfData = NULL;
	theErr = TXNFlattenObjectToCFDataRef(GetTXNObject(), theFormat, &cfData);
	NN_ASSERT_NOERR(theErr);
	
	if (cfData != NULL)
		theData.Set(cfData);
	
	return(theData);
}





//============================================================================
//		NHITextView::SetTextData : Set the text data.
//----------------------------------------------------------------------------
void NHITextView::SetTextData(const NData &theData, TXNDataType theFormat)
{	NFile		theFile;
	OSStatus	theErr;



	// Set RTF data
	//
	// TXNSetData does not support kTXNRichTextFormatData data (rdar://4512691),
	// and so for this format we have to write the data out to a temporary file.
	if (theFormat == kTXNRichTextFormatData)
		{
		theFile = NFileUtilities::GetTempFile("NHITextView.rtf");
		theErr  = NFileUtilities::SetFileData(theFile, theData);
		NN_ASSERT_NOERR(theErr);
			
		if (theErr == noErr)
			{
			theErr = LoadTextFile(theFile);
			NN_ASSERT_NOERR(theErr);
			
			theFile.Delete();
			}
		}
	
	
	// Set other data
	else
		{
		theErr = TXNSetData(GetTXNObject(), theFormat, theData.GetData(), theData.GetSize(), kTXNStartOffset, kTXNEndOffset);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NHITextView::LoadTextFile : Load a text file.
//----------------------------------------------------------------------------
OSStatus NHITextView::LoadTextFile(const NFile &theFile)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theFile.Exists());



	// Load the file
	theErr = TXNSetDataFromCFURLRef(GetTXNObject(), theFile.GetURL(), kTXNStartOffset, kTXNEndOffset);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NHITextView::SaveTextFile : Save a text file.
//----------------------------------------------------------------------------
OSStatus NHITextView::SaveTextFile(const NFile &theFile)
{	FSSpec		theFSSpec;
	NFile		openFile;
	OSStatus	theErr;



	// Save an RTF file
	//
	// Although TXNWriteRangeToCFURL can write out RTF data, it requires 10.4
	// and so for now we write these files ourselves from their raw data.
	if (theFile.GetUTI().EqualTo(kUTNTypeRTF))
		{
		theErr = NFileUtilities::SetFileData(theFile, GetTextData());
		NN_ASSERT_NOERR(theErr);
		}



	// Save a text file
	else
		{
		// Open the file
		openFile = theFile;
		theErr   = openFile.OpenFile(fsWrPerm, true);

		if (theErr != noErr)
			return(theErr);


		// Save the data
		theErr = TXNSave(GetTXNObject(), kTXNUnicodeTextFile,
							kTXNSingleStylePerTextDocumentResType,
							kTXNUnicodeEncoding, &theFSSpec,
							openFile.GetForkRef(), kInvalidForkRef);

		NN_ASSERT_NOERR(theErr);
		}

	return(theErr);
}





//============================================================================
//		NHITextView::GetTXNData : Get a TXNControlData value.
//----------------------------------------------------------------------------
TXNControlData NHITextView::GetTXNData(TXNControlTag theTag) const
{	TXNControlData	theData = { 0 };
	OSStatus		theErr;



	// Get the data
	theErr = TXNGetTXNObjectControls(GetTXNObject(), 1, &theTag, &theData);
	NN_ASSERT_NOERR(theErr);
	
	return(theData);
}





//============================================================================
//		NHITextView::SetTXNData : Set a TXNControlData value.
//----------------------------------------------------------------------------
void NHITextView::SetTXNData(TXNControlTag theTag, const TXNControlData &theData)
{	OSStatus	theErr;



	// Set the data
	theErr = TXNSetTXNObjectControls(GetTXNObject(), false, 1, &theTag, &theData);
	NN_ASSERT_NOERR(theErr);
}



