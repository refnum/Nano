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
#include "NMsgBroadcaster.h"
#include "NHIView.h"
#include "NTimer.h"
#include "NColor.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgTextViewChanged					= 'tcha';


// Misc
static const TXNFrameOptions kHITextViewOptionsDefault			= kTXNWantHScrollBarMask |
																  kTXNWantVScrollBarMask;


// Margins
typedef enum {
	kMarginTop,
	kMarginLeft,
	kMarginBottom,
	kMarginRight
} TextMargin;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHITextView : public NHIView, public NMsgBroadcaster {
public:
										DECLARE_HIVIEW_SUBCLASS(NHITextView);

										NHITextView(HIWindowRef				theWindow,
														const HIRect		&theRect,
														const NString		&theValue  = "",
														TXNFrameOptions		theOptions = kHITextViewOptionsDefault);
	virtual								~NHITextView(void);


	// Has the content been modified?
	bool								IsModified(void) const;


	// Clear the undo state
	void								ClearUndo(void);


	// Can the last action be undone/redone?
	bool								CanUndo(void) const;
	bool								CanRedo(void) const;
	

	// Get/set the background color
	NColor								GetBackgroundColor(void) const;
	void								SetBackgroundColor(const NColor &theColor);
	

	// Get/set the read-only state
	bool								IsReadOnly(void) const;
	void								SetReadOnly(bool readOnly);


	// Get/set the spelling state
	bool								IsSpellingEnabled(void) const;
	void								SetSpellingEnabled(bool theFlag);


	// Get/set the selection
	CFRange								GetSelection(void) const;
	void								SetSelection(const CFRange &theSelection=kCFRangeAll);


	// Get/set the text value
	NString								GetTextValue(void) const;
	void								SetTextValue(const NString &theValue);


	// Get/set the text data
	NData								GetTextData(                      TXNDataType theFormat=kTXNRichTextFormatData) const;
	void								SetTextData(const NData &theData, TXNDataType theFormat=kTXNRichTextFormatData);


	// Load/save the text to a file
	//
	// Text will be saved as .rtf, or a Unicode .txt file, based on the file UTI.
	OSStatus							Load(const NFile &theFile);
	OSStatus							Save(const NFile &theFile);


	// Get/clear the action count
	UInt32								GetActionCount(  CFStringRef cfAction=kTXNActionCountOfAllChanges) const;
	void								ClearActionCount(CFStringRef cfAction=kTXNActionCountOfAllChanges);


	// Get/set the command options
	TXNCommandEventSupportOptions		GetCommandOptions(void) const;
	void								SetCommandOptions(TXNCommandEventSupportOptions setThese, TXNCommandEventSupportOptions clearThese=0);


	// Get/set the auto-scroll mode
	TXNAutoScrollBehavior				GetAutoScroll(void) const;
	void								SetAutoScroll(TXNAutoScrollBehavior theMode);


	// Get/set a margin
	float								GetMargin(TextMargin theMargin) const;
	void								SetMargin(TextMargin theMargin, float theValue);


	// Get/set a TXNControlData value
	void								GetTXNData(TXNControlTag theTag,       TXNControlData &theData) const;
	void								SetTXNData(TXNControlTag theTag, const TXNControlData &theData);


	// Operators
	operator							TXNObject(void) const;


protected:
	// Initialize the view
	void								InitializeView(void);


	// Get the status of a command
	CommandStatus						GetCommandStatus(const HICommandExtended &hiCmd);


private:
	void								UpdateModified(void);


private:
	NTimer								mTimer;
	bool								mModified;
};





#endif // NHITextView_HDR


