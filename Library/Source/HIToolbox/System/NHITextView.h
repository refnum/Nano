/*	NAME:
		NHITextView.h

	DESCRIPTION:
		Text field view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHITEXTVIEW_HDR
#define NHITEXTVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"
#include "NColor.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const TXNFrameOptions kHITextViewOptionsDefault			= kTXNWantHScrollBarMask |
																  kTXNWantVScrollBarMask;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHITextView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NHITextView);

										NHITextView(HIWindowRef				theWindow,
														const HIRect		&theRect,
														const NString		&theValue  = "",
														TXNFrameOptions		theOptions = kHITextViewOptionsDefault);
	virtual								~NHITextView(void);


	// Get the TXNObject
	TXNObject							GetTXNObject(void) const;


	// Get/set the background color
	NColor								GetBackgroundColor(void) const;
	void								SetBackgroundColor(const NColor &theColor);


	// Get/set the read only state
	bool								IsReadOnly(void) const;
	void								SetReadOnly(bool readOnly);
	

	// Get/set the text value
	NString								GetTextValue(void) const;
	void								SetTextValue(const NString &theValue);


	// Get/set the text data
	NData								GetTextData(                      TXNDataType theFormat=kTXNRichTextFormatData) const;
	void								SetTextData(const NData &theData, TXNDataType theFormat=kTXNRichTextFormatData);


	// Load/save a file
	//
	// Text can be saved as a .rtf or a Unicode .txt file.
	OSStatus							LoadTextFile(const NFile &theFile);
	OSStatus							SaveTextFile(const NFile &theFile);


	// Get/set a TXNControlData value
	TXNControlData						GetTXNData(TXNControlTag theTag) const;
	void								SetTXNData(TXNControlTag theTag, const TXNControlData &theData);


private:


};





#endif // NHITextView_HDR


