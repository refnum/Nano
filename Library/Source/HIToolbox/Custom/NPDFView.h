/*	NAME:
		NPDFView.h

	DESCRIPTION:
		PDF view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPDFVIEW_HDR
#define NPDFVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NScrollableView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPDFView : public NScrollableView {
public:
										DECLARE_HIVIEW_SUBCLASS(NPDFView);

										NPDFView(HIWindowRef			theWindow,
													const HIRect		&theRect,
													CGPDFDocumentRef	pdfDoc = NULL);
	virtual								~NPDFView(void);


	// Load a document
	OSStatus							Load(const NFile &theFile);


	// Get/set the document
	CGPDFDocumentRef					GetDocument(void) const;
	void								SetDocument(CGPDFDocumentRef pdfDoc);


	// Get the page count
	//
	// Returns 0 if no document is loaded.
	UInt32								GetPageCount(void) const;
	

	// Get/set the current page
	//
	// Pages are indexed from 1.
	UInt32								GetPage(void) const;
	void								SetPage(UInt32 thePage);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Draw the view
	void								DrawContent(NCarbonEvent &theEvent);


private:
	UInt32								mPage;
	NCFObject							mDocument;
};





#endif // NPDFVIEW_HDR


