/*	NAME:
		NDocument.h

	DESCRIPTION:
		Document object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDOCUMENT_HDR
#define NDOCUMENT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <list>

#include "NWindowController.h"
#include "NCFPreferences.h"
#include "NPrintDialog.h"
#include "NFileDialog.h"
#include "NDictionary.h"
#include "NAlias.h"
#include "NFile.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Properties
static const NString kDocNibFileKey									= "DocNibFile";
static const NString kDocNibWindowKey								= "DocNibWindow";
static const NString kDocWindowToolbarUnified						= "DocWindowToolbarUnified";
static const NString kDocRecentItemsGroupKey						= "DocRecentItemsGroup";
static const NString kDocFileIconKey								= "DocFileIcon";
static const NString kDocFileUTIKey									= "DocFileUTI";
static const NString kDocFormatUTIsKey								= "DocFormatUTIs";





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Lists
class NDocument;

typedef std::list<NDocument*>										NDocumentList;
typedef NDocumentList::iterator										NDocumentListIterator;
typedef NDocumentList::const_iterator								NDocumentListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDocument : public NWindowController {
public:
										NDocument(void);
	virtual								~NDocument(void);


	// Initialize the document
	OSStatus							InitializeDocument(const NFile *theFile=NULL);


	// Does the document have a file?
	bool								HasFile(void) const;


	// Get/set the modified state
	//
	// Typically, documents should record changes to their content via the NUndoManager
	// associated with that document. NDocument will automatically track changes made
	// to this object, and will adjust the modified state as required.
	//
	// SetModified should only be used if a non-undable change has been made to the
	// document content. Since this change happened outside the normal undo mechanism,
	// this will also clear the undo stacks.
	bool								IsModified(void) const;
	void								SetModified(bool isModified=true);


	// Get/set the file
	NFile								GetFile(void) const;
	void								SetFile(const NFile &theFile);


	// Get the properties dictionary
	NDictionary							*GetProperties(void);


	// Get the document name
	NString								GetDocumentName(void);


	// Get the documents
	static NDocumentList				GetDocuments(bool onlyModified=false);


	// Get the "current" document
	//
	// The current document is the document window closest to the front.
	static NDocument					*GetCurrentDocument(void);
	

protected:
	// Initialize ourselves
	//
	// Sub-classes should call through to initialize the base class.
	virtual OSStatus					InitializeProperties(const NDictionary *appProperties);
	virtual OSStatus					InitializeSelf(void);


	// Handle messages
	virtual void						DoMessage(BroadcastMsg theMsg, void *msgData);
	

	// Handle events
	virtual OSStatus					DoEventCommandProcess( NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppActivated(   NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowActivated(NCarbonEvent &theEvent);


	// Get the status of a command
	virtual CommandStatus				GetCommandStatus(const HICommandExtended &hiCmd);


	// Handle commands
	virtual OSStatus					CommandCancel(void);
	virtual OSStatus					CommandClose(void);
	virtual OSStatus					CommandSave(void);
	virtual OSStatus					CommandSaveAs(void);
	virtual OSStatus					CommandRevert(void);
	virtual OSStatus					CommandPageSetup(void);
	virtual OSStatus					CommandPrint(void);


	// Handle preference changes
	virtual void						DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey);


	// Load/save/print the document
	virtual OSStatus					LoadSelf(void);
	virtual OSStatus					SaveSelf(void);
	virtual OSStatus					PrintSelf(const NPrintDialog *theDialog, UInt32 pageNum);


	// Encode/decode the document state
	//
	// Sub-classes should call through to allow the base class to archive its state.
	virtual OSStatus					EncodeState(      NDictionary &theState) const;
	virtual OSStatus					DecodeState(const NDictionary &theState);


	// Update the window appearance
	//
	// Sub-classes should override to customize the window appearance.
	virtual void						UpdateWindowTitle(void);
	virtual void						UpdateWindowProxy(void);
	virtual void						UpdateWindowModified(void);


	// Update the print dialog
	//
	// Sub-classes should override to customize the print job.
	virtual OSStatus					UpdatePrintDialog(PrintDialogKind theKind, NPrintDialog *theDialog);


	// Get the print view
	//
	// The base class provides a default UpdatePrintDialog and PrintSelf
	// implementation which prints a single page job with a single view
	// centered and scaled to fit on that page.
	//
	// Sub-classes can override GetPrintView to provide the view to print
	// if this model is sufficient, or overridate UpdatePrintDialog and
	// PrintSelf as appropriate.
	virtual NHIView						*GetPrintView(void);
	
	
private:
	OSStatus							LoadDocument(void);
	OSStatus							SaveDocument(void);
	OSStatus							PrintDocument(void);
	
	void								UpdateWindow(void);
	
	void								CompleteFileDialog( const FileDialogResult  &theResult);
	void								CompletePrintDialog(const PrintDialogResult &theResult);
	

private:
	NDictionary							mProperties;
	NFileDialog							mFileDialog;
	NPrintDialog						mPrintDialog;

	NAlias								mAlias;
	NString								mUntitledName;

	bool								mIsModified;
	bool								mCloseOnSave;
	bool								mContinueOnClose;

	static NDocumentList				mDocuments;
	static NDocument					*mCurrentDocument;
	static UInt32						mUntitledCount;
};





#endif // NDOCUMENT_HDR


