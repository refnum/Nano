/*	NAME:
		CExampleView.cpp

	DESCRIPTION:
		Example view.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NCGContext.h"

#include "CExampleView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Properties
//
// These values may be archived to disk, and can never change.
static const NString kEVColor										= "EVColor";


// Tags
static const OSType kTagColor										= 'colr';


// Misc
static const float   kShapeInset									= 5.0f;
static const float   kShapeCorner									= 10.0f;
static const NString kShapeLabel									= "Your Document Content Here...";





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(CExampleView, "com.mycompany.cexampleview", kHIViewClassID);





//============================================================================
//		CExampleView::CExampleView : Constructor.
//----------------------------------------------------------------------------
CExampleView::CExampleView(HIWindowRef		theWindow,
							const HIRect	&theRect,
							const NColor	&theColor)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	SetColor(theColor);
}





//============================================================================
//		CExampleView::~CExampleView : Destructor.
//----------------------------------------------------------------------------
CExampleView::~CExampleView(void)
{
}





//============================================================================
//		CExampleView::GetColor : Get the color.
//----------------------------------------------------------------------------
NColor CExampleView::GetColor(void) const
{


	// Get the color
	return(mColor);
}





//============================================================================
//		CExampleView::SetColor : Set the color.
//----------------------------------------------------------------------------
void CExampleView::SetColor(const NColor &theColor)
{


	// Set the color
	mColor = theColor;
	
	SetNeedsDisplay();
}





//============================================================================
//		CExampleView::EncodeProperties : Encode the properties.
//----------------------------------------------------------------------------
OSStatus CExampleView::EncodeProperties(NDictionary &theProperties)
{	OSStatus	theErr;



	// Encode the base class
	theErr = NHIView::EncodeProperties(theProperties);
	if (theErr != noErr)
		return(theErr);



	// Encode the properties
	theProperties.SetValueColor(kEVColor, mColor);
	
	return(theErr);
}





//============================================================================
//		CExampleView::DecodeProperties : Decode the properties.
//----------------------------------------------------------------------------
OSStatus CExampleView::DecodeProperties(const NDictionary &theProperties)
{	NDictionary		theDictionary;
	OSStatus		theErr;



	// Decode the base class
	theErr = NHIView::DecodeProperties(theProperties);
	if (theErr != noErr)
		return(theErr);



	// Decode the properties
	SetColor(theProperties.GetValueColor(kEVColor));

	return(theErr);
}





//============================================================================
//		CExampleView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void CExampleView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw },
										{ kEventClassNone,    kEventKindNone    } };



	// Register for events
	AddEvents(theEvents);



	// Initialize ourselves
	SetColor(kColorRed);



	// Apply our properties
	if (HasProperty(               kPropertyNano, kTagColor))
		SetColor(GetPropertyString(kPropertyNano, kTagColor));
}





//============================================================================
//		CExampleView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus CExampleView::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext		cgContext(theEvent);
	HIRect			theRect;



	// Draw the shape
	theRect = CGRectInset(GetBounds(), kShapeInset, kShapeInset);

	cgContext.SetLineWidth(1.0f);
	cgContext.SetStrokeColor(kColorBlack);
	cgContext.SetFillColor(mColor);

	cgContext.AddRect(theRect, kShapeCorner);
	cgContext.DrawPath();



	// Draw the text
	theRect = CGRectInset(theRect, kShapeInset, kShapeInset);

	cgContext.SetFillColor(kColorBlack);
	cgContext.DrawThemeText(kShapeLabel, theRect);

	return(noErr);
}
