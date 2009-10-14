/*	NAME:
		NPrintDialog.h

	DESCRIPTION:
		Standard print dialog.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPRINTDIALOG_HDR
#define NPRINTDIALOG_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"
#include "NHIWindow.h"
#include "NTimer.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgPrintDialogResult						= 'pres';


// Dialogs
typedef enum {
	kPrintDialogNone,
	kPrintDialogPageSetup,
	kPrintDialogPrint
} PrintDialogKind;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NPrintDialog;


// Message data for kMsgPrintDialogResult
typedef struct {
	const NPrintDialog			*printDialog;
	PrintDialogKind				dialogKind;
	bool						didAccept;
} PrintDialogResult;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPrintDialog : public NMsgBroadcaster {
public:
										NPrintDialog(void);
	virtual								~NPrintDialog(void);


	// Is the dialog open?
	bool								IsOpen(void) const;


	// Cancel the dialog
	void								Cancel(void);


	// Get/set the parent
	//
	// When a parent window is assigned, the dialog will appear as a
	// window-modal sheet rather than an application-modal dialog.
	//
	// If a parent window is assigned, opening the dialog will return
	// immediately. When the dialog is dismissed, the final result will
	// be broadcast in a kMsgPrintDialogResult.
	NHIWindow							*GetParent(void) const;
	void								SetParent(NHIWindow *theParent);


	// Create a print event
	//
	// Creates a kEventControlDrawEvent that can be dispatched to a view to
	// render the view's content for the current page.
	//
	// The event contains a kHIThemeOrientationNormal context, clipped to the
	// printable area of the page, with these parameters.
	//
	//		--> kEventParamCGContextRef (in, typeCGContextRef)
	//				The CGContext into which to draw.
	//
	//		--> kEventNParamIsPrinting (in, typeBoolean)
	//				A flag indicating this is a print context.
	//
	//		--> kEventNParamPageRect   (in, typeHIRect)
	//				The page rectangle for the context.
	//
	// Only available between PMSessionBeginPage/PMSessionEndPage calls.
	NCarbonEvent						CreateEventControlDraw(void) const;


	// Create/destroy the session
	//
	// The print session will be created on demand, but can be created
	// prior to opening the dialog to pre-configure any required options.
	OSStatus							CreateSession( void);
	void								DestroySession(void);


	// Get the session state
	PMPageFormat						GetPageFormat(   void) const;
	PMPrintSession						GetPrintSession( void) const;
	PMPrintSettings						GetPrintSettings(void) const;


	// Get the print areas
	//
	// Only available between PMSessionBeginPage/PMSessionEndPage calls.
	HIRect								GetPaperRect(void) const;
	HIRect								GetPageRect( void) const;


	// Get/set the orientation
	//
	// Only available between CreateSession/DestroySession calls.
	PMOrientation						GetOrientation(void) const;
	void								SetOrientation(PMOrientation theOrientation);


	// Get/set the resolution
	//
	// Only available between CreateSession/DestroySession calls.
	PMResolution						GetResolution(void) const;
	void								SetResolution(PMResolution theResolution);


	// Get/set the scale
	//
	// Only available between CreateSession/DestroySession calls.
	double								GetScale(void) const;
	void								SetScale(double theScale);


	 // Get/set the job name
	//
	// Only available between CreateSession/DestroySession calls.
	 NString							GetJobName(void) const;
	 void								SetJobName(const NString &theName);


	// Get/set the page range
	//
	// The active range is the page range selected by the user; if activeRange
	// is false, the entire page range set by the application is returned.
	//
	// Only available between CreateSession/DestroySession calls.
	void								GetPageRange(UInt32 &firstPage, UInt32 &lastPage, bool activeRange=true) const;
	void								SetPageRange(UInt32  firstPage, UInt32  lastPage);


	// Display the page setup dialog
	OSStatus							DoPageSetup(void);


	// Display the print dialog
	OSStatus							DoPrint(void);


private:
	OSStatus							PrepareSession(PrintDialogKind theKind);
	void								ValidateSession(void);
	
	OSStatus							FinishDialog(bool didAccept);
	void								FinishSheet( bool didAccept);

	static void							SheetDone(PMPrintSession theSession, WindowRef theWindow, Boolean didAccept);


private:
	NHIWindow							*mParent;
	PrintDialogResult					mResult;

	NTimer								mTimer;
	bool								mInsideSheet;
	
	PMPageFormat						mPageFormat;
	PMPrintSession						mPrintSession;
	PMPrintSettings						mPrintSettings;

	static PMSheetDoneUPP				mSheetDone;
};





#endif // NPRINTDIALOG_HDR


