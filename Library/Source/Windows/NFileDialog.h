/*	NAME:
		NFileDialog.h

	DESCRIPTION:
		Standard file dialog.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NFILEDIALOG_HDR
#define NFILEDIALOG_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"
#include "NHIWindow.h"
#include "NArray.h"
#include "NFile.h"
#include "NUTI.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgFileDialogFilter						= 'filt';
static const BroadcastMsg kMsgFileDialogEvent						= 'fevt';
static const BroadcastMsg kMsgFileDialogResult						= 'fres';


// Dialogs
typedef enum {
	kNavNone,
	kNavReviewDocuments,
	kNavSaveChanges,
	kNavDiscardChanges,
	kNavChooseSave,
	kNavChooseFile,
	kNavChooseFolder,
	kNavChooseVolume
} FileDialogKind;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NFileDialog;


// Message data for kMsgFileDialogFilter
typedef struct {
	const NFileDialog			*fileDialog;
	NavFilterModes				filterMode;
	NFile						theFile;
	bool						showFile;
} FileDialogFilter;


// Message data for kMsgFileDialogEvent
typedef struct {
	const NFileDialog			*fileDialog;
	NavEventCallbackMessage		navMsg;
	NavCBRecPtr					navData;
} FileDialogEvent;


// Message data for kMsgFileDialogResult
typedef struct {
	const NFileDialog			*fileDialog;
	NUTI						formatUTI;
	NavReplyRecord				*navReply;
	NavUserAction				navAction;
	NFileList					theFiles;
} FileDialogResult;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NFileDialog : public NMsgBroadcaster {
public:
										NFileDialog(void);
	virtual								~NFileDialog(void);


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
	// immediately with kNavUserActionNone. When the dialog is dismissed,
	// the final result will be broadcast in a kMsgFileDialogResult.
	NHIWindow							*GetParent(void) const;
	void								SetParent(NHIWindow *theParent);


	// Prompt to review modified documents
	NavUserAction						AskReviewDocuments(UInt32 numDocs);


	// Prompt to save changes
	NavUserAction						AskSaveChanges(NavAskSaveChangesAction theCause);


	// Prompt to discard changes
	NavUserAction						AskDiscardChanges(const NString &fileName);


	// Choose a file for saving
	//
	// The result is optional if the dialog is attached as a sheet.
	//
	// A list of UTIs the file can be saved as should be provided if known; if
	// the fileUTI value is not contained in this list, it will be adjusted to
	// reflect the first item in the list.
	NavUserAction						ChooseSave(const NString		&textDialog,
												   const NString		&textButton,
												   const NString		&fileName,
												   const NUTI			&fileUTI,
												   FileDialogResult		*theResult  = NULL,
												   const NArray			&formatUTIs = NArray());


	// Choose a file
	//
	// A list of UTIs can be provided to filter files.
	NavUserAction						ChooseFile(const NString		&textDialog,
												   const NString		&textButton,
												   FileDialogResult		&theResult,
												   const NArray			&formatUTIs = NArray());


	// Choose a folder
	NavUserAction						ChooseFolder(const NString			&textDialog,
												 	 const NString			&textButton,
													 FileDialogResult		&theResult);


	// Choose a volume
	NavUserAction						ChooseVolume(const NString			&textDialog,
												 	 const NString			&textButton,
													 FileDialogResult		&theResult);


private:
	NavDialogCreationOptions			GetOptions(void);
	CFStringRef							GetString(const NString &theString);
	
	NavUserAction						DoDialog(FileDialogKind			theKind,
													FileDialogResult	*theResult  = NULL,
													const NUTI			*fileUTI    = NULL,
													const NArray		*formatUTIs = NULL);

	NArray								GetFormatList(const NArray &formatUTIs); 
	bool								FilterUTI(const NFile &theFile, bool &showFile);

	void								DoPopupMenuSelect(const NavMenuItemSpec *navMenu);
	void								DoUserAction(     const NavCBRec        *navInfo);

	static Boolean						NavFilter(AEDesc *theItem, void *info, void *callBackUD, NavFilterModes filterMode);
	static void							NavEvent( NavEventCallbackMessage callBackSelector, NavCBRecPtr callBackParms, void *callBackUD);


private:
	NHIWindow							*mParent;
	NavDialogRef						mDialog;
	FileDialogKind						mKind;
	FileDialogResult					mResult;
	
	NUTI								mFormatUTI;
	NArray								mFormatUTIs;

	static NavObjectFilterUPP			mNavFilter;
	static NavEventUPP					mNavEvent;
};





#endif // NFILEDIALOG_HDR


