/*	NAME:
		NMembraneButton.cpp

	DESCRIPTION:
		Membrane button.
		
		A "membrane button" is a push button with a smooth appearance, found
		in the Finder's Find window.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGContext.h"
#include "NMembraneButton.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Geometry
static const float kMembraneRadius									= 7.5f;


// Colors
static const NColor kFaceBorder										= NColor(0x999999EE);
static const NColor kFacePressed									= NColor(0xCCCCCCFF);
static const NColor kFaceUnderline									= NColor(0xFFFFFF88);

static const NShadingSample kSamplesActiveFace[]					= {	{ 0.0f, NColor(0xFFFFFFFF) },
																		{ 1.0f, NColor(0xDBDBDBFF) } };

static const NShadingSample kSamplesInactiveFace[]					= {	{ 0.0f, NColor(0xFFFFFFFF) },
																		{ 1.0f, NColor(0xF0F0F0FF) } };

static const NShadingSample kSamplesPressedFace[]					= {	{ 0.0f, NColor(0xDBDBDBFF) },
																		{ 1.0f, NColor(0xFFFFFFFF) } };





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NMembraneButton, "com.nano.nmembranebutton", kHIBevelButtonClassID);





//============================================================================
//		NMembraneButton::NMembraneButton : Constructor.
//----------------------------------------------------------------------------
NMembraneButton::NMembraneButton(HIWindowRef theWindow, const HIRect &theRect)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);
}





//============================================================================
//		NMembraneButton::~NMembraneButton : Destructor.
//----------------------------------------------------------------------------
NMembraneButton::~NMembraneButton(void)
{
}





//============================================================================
//		NMembraneButton::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NMembraneButton::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw	},
										{ kEventClassNone,    kEventKindNone	} };



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NMembraneButton::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NMembraneButton::DoEventControlDraw(NCarbonEvent &theEvent)
{	bool						isEnabled, isPressed;
	NCGContext					cgContext(theEvent);
	float						minX, maxX, minY, maxY;
	ThemeTextColor				textColor;
	HIRect						theRect;
	NString						theText;



	// Get the state we need
	theRect = CGRectInset(GetBounds(), 1.0f, 1.0f);
	theText = GetTextValue();

	minX = CGRectGetMinX(theRect);
	maxX = CGRectGetMaxX(theRect);
	minY = CGRectGetMinY(theRect);
	maxY = CGRectGetMaxY(theRect);
	
	isEnabled = IsEnabled();
	isPressed = (GetHilite() == kControlButtonPart);
 
	if (isPressed)
		{
		mShadingFace.SetSamples(kSamplesPressedFace);
		textColor = kThemeTextColorBevelButtonPressed;
		}

	else if (isEnabled)
		{
		mShadingFace.SetSamples(kSamplesActiveFace);
		textColor = kThemeTextColorBevelButtonActive;
		}

	else
		{
		mShadingFace.SetSamples(kSamplesInactiveFace);
		textColor = kThemeTextColorBevelButtonInactive;
		}

	mShadingFace.ApplyLinearShading(kLinearTopBottom, minY, maxY);



	// Draw the underline
	cgContext.SetLineWidth(1.0f);

	cgContext.MoveTo(   minX + kMembraneRadius, maxY + 1.0f);
	cgContext.AddLineTo(maxX - kMembraneRadius, maxY + 1.0f);
	
	cgContext.SetAntiAlias(false);
	cgContext.SetStrokeColor(kFaceUnderline);
	cgContext.StrokePath();
	cgContext.SetAntiAlias(true);



	// Draw the border
	theRect.size.height -= 1.0f;
	maxY                -= 1.0f;

	cgContext.AddRect(theRect, kMembraneRadius);

	cgContext.SetLineWidth(2.0f);
	cgContext.SetStrokeColor(kFaceBorder);
	cgContext.StrokePath();

	cgContext.SetLineWidth(1.0f);



	// Draw the face
	cgContext.SaveState();
	
	cgContext.AddRect(theRect, kMembraneRadius);
	cgContext.ClipToPath();

	cgContext.DrawShading(mShadingFace);
	
	if (isPressed)
		{
		cgContext.MoveTo(   minX + kMembraneRadius - 2.0f, minY + 1.0f);
		cgContext.AddLineTo(maxX - kMembraneRadius + 2.0f, minY + 1.0f);

		cgContext.SetAntiAlias(false);
		cgContext.SetStrokeColor(kFacePressed);
		cgContext.StrokePath();
		cgContext.SetAntiAlias(true);
		}

	cgContext.RestoreState();



	// Draw the label
	if (theText.IsNotEmpty())
		{
		cgContext.SetTextColor(textColor);

		cgContext.DrawThemeText(theText, theRect, kThemeTextLabelCenter);
		}

	return(noErr);
}


