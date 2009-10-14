/*	NAME:
		NPDFView.cpp

	DESCRIPTION:
		PDF view.
		
		Supplied by Cyril Murzin <mcyril@gmail.com>.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGContext.h"
#include "NCFBundle.h"
#include "NPDFView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagDocument									= 'pdf ';
static const OSType kTagDocumentPage								= 'page';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NPDFView, "com.nano.npdfview", kHIViewClassID);





//============================================================================
//		NPDFView::NPDFView : Constructor.
//----------------------------------------------------------------------------
NPDFView::NPDFView(HIWindowRef theWindow, const HIRect &theRect, CGPDFDocumentRef pdfDoc)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	if (pdfDoc != NULL)
		SetDocument(pdfDoc);
}





//============================================================================
//		NPDFView::~NPDFView : Destructor.
//----------------------------------------------------------------------------
NPDFView::~NPDFView(void)
{
}





//============================================================================
//		NPDFView::Load : Load a document.
//----------------------------------------------------------------------------
OSStatus NPDFView::Load(const NFile &theFile)
{	CGPDFDocumentRef	pdfDoc;
	OSStatus			theErr;



	// Load the document
	pdfDoc = CGPDFDocumentCreateWithURL(theFile.GetURL());
	theErr = (pdfDoc == NULL) ? (OSStatus) badFormat : (OSStatus) noErr;
	
	if (theErr == noErr)
		SetDocument(pdfDoc);
	
	return(theErr);
}





//============================================================================
//		NPDFView::GetDocument : Get the document.
//----------------------------------------------------------------------------
CGPDFDocumentRef NPDFView::GetDocument(void) const
{


	// Get the document
	return(mDocument);
}





//============================================================================
//		NPDFView::SetDocument : Set the document.
//----------------------------------------------------------------------------
void NPDFView::SetDocument(CGPDFDocumentRef pdfDoc)
{


	// Set the document
	mDocument.Set(pdfDoc, false);



	// Update our state
	SetPage(mDocument.IsValid() ? 1 : 0);
	SetNeedsDisplay();
}





//============================================================================
//		NPDFView::GetPageCount : Get the page count.
//----------------------------------------------------------------------------
UInt32 NPDFView::GetPageCount(void) const
{	UInt32	numPages;



	// Get the page count
	numPages = 0;
	
	if (mDocument.IsValid())
		numPages = CGPDFDocumentGetNumberOfPages(mDocument);
	
	return(numPages);
}





//============================================================================
//		NPDFView::GetPage : Get the current page.
//----------------------------------------------------------------------------
UInt32 NPDFView::GetPage(void) const
{


	// Get the page
	return(mPage);
}





//============================================================================
//		NPDFView::SetPage : Set the current page.
//----------------------------------------------------------------------------
void NPDFView::SetPage(UInt32 thePage)
{	CGPDFPageRef	pdfPage;
	HISize			theSize;
	UInt32			oldPage;



	// Validate our parameters
	if (mDocument.IsValid())
		NN_ASSERT(thePage >= 1 && thePage <= GetPageCount());
	else
		NN_ASSERT(thePage == 0);



	// Set the page
	oldPage = mPage;
	mPage   = thePage;



	// Update our state
	if (oldPage != mPage)
		{
		// Get the page bounds
		theSize = kHISizeZero;

		if (mDocument.IsValid())
			{
			pdfPage = CGPDFDocumentGetPage(mDocument, thePage);
			if (pdfPage != NULL)
				theSize = CGPDFPageGetBoxRect(pdfPage, kCGPDFMediaBox).size;
			}


		// Update our state
		SetImageSize(theSize);
		SetNeedsDisplay();
		}
}





//============================================================================
//		NPDFView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NPDFView::InitializeView(void)
{


	// Initialize the base class
	NScrollableView::InitializeView();



	// Initialize ourselves
	mPage = 0;



	// Apply our properties
	if (HasProperty(                           kPropertyNano, kTagDocument))
		Load(NBundleResource(GetPropertyString(kPropertyNano, kTagDocument)));

	if (HasProperty(              kPropertyNano, kTagDocumentPage))
		SetPage(GetPropertyUInt32(kPropertyNano, kTagDocumentPage));
}





//============================================================================
//		NPDFView::DrawContent : Draw the view.
//----------------------------------------------------------------------------
void NPDFView::DrawContent(NCarbonEvent &theEvent)
{	NCGContext		cgContext(theEvent);
	CGPDFPageRef	pdfPage;
	HISize			theSize;



	// Get the state we need
	theSize = GetImageSize();
	pdfPage = CGPDFDocumentGetPage(mDocument, mPage);



	// Draw the page
	//
	// Since CGContextDrawPDFPage draws to the CG coordinate system, we
	// flip the transform prior to drawing to draw the right way up.
	if (pdfPage != NULL)
		{
		cgContext.TranslateCTM(0.0f, theSize.height);
		cgContext.ScaleCTM(    1.0f, -1.0f);

		CGContextDrawPDFPage(cgContext, pdfPage);
		}
}



