/*	NAME:
		NFlatButton.cpp

	DESCRIPTION:
		Flat button.
		
		A "flat button" is equivalent to Cocoa's "small square" button.
		
		We inherit standard button behaviour by subclassing a bevel button,
		and override kEventControlDraw to customise the appearance.

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
#include "NFlatButton.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
typedef struct {
	NColor		frameTopOuter;
	NColor		frameTopInner;
	NColor		frameBottomOuter;
	NColor		frameBottomInner;
	NColor		frameSide;
	NColor		faceTop;
	NColor		faceBottom;
} FlatButtonStyle;





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Geometry
static const float kButtonFaceSplit									= 0.6f;


// Colors
static const FlatButtonStyle kFlatButtonStyleActive					= { NColor(0xD8D8D8FF),
																		NColor(0x7D7D7DFF),
																		NColor(0xF5F5F5FF),
																		NColor(0x979797FF),
																		NColor(0x979797FF),
																		NColor(0xFBFBFBFF),
																		NColor(0xEDEDEDFF) };

static const FlatButtonStyle kFlatButtonStyleInactive				= kFlatButtonStyleActive;

static const FlatButtonStyle kFlatButtonStylePressed				= { NColor(0xDFDFDFFF),
																		NColor(0x545454FF),
																		NColor(0xC4C4C4FF),
																		NColor(0x646464FF),
																		NColor(0x696969FF),
																		NColor(0x969696FF),
																		NColor(0x8E8E8EFF) };





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NFlatButton, "com.nano.nflatbutton", kHIBevelButtonClassID);





//============================================================================
//		NFlatButton::NFlatButton : Constructor.
//----------------------------------------------------------------------------
NFlatButton::NFlatButton(HIWindowRef theWindow, const HIRect &theRect)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);
}





//============================================================================
//		NFlatButton::~NFlatButton : Destructor.
//----------------------------------------------------------------------------
NFlatButton::~NFlatButton(void)
{
}





//============================================================================
//		NFlatButton::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NFlatButton::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw	},
										{ kEventClassNone,    kEventKindNone	} };



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NFlatButton::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NFlatButton::DoEventControlDraw(NCarbonEvent &theEvent)
{	float						minX, maxX, minY, maxY, theSplit;
	bool						isEnabled, isPressed;
	NCGContext					cgContext(theEvent);
	const FlatButtonStyle		*theStyle;
	ThemeTextColor				textColor;
	HIRect						theRect;
	NString						theText;



	// Get the state we need
	theRect = GetBounds();
	theText = GetTextValue();
	
	minX = CGRectGetMinX(theRect);
	maxX = CGRectGetMaxX(theRect);
	minY = CGRectGetMinY(theRect);
	maxY = CGRectGetMaxY(theRect);
	
	isEnabled = IsEnabled();
	isPressed = (GetHilite() == kControlButtonPart);
 
	if (isPressed)
		{
		theStyle  = &kFlatButtonStylePressed;
		textColor = kThemeTextColorBevelButtonPressed;
		}

	else if (isEnabled)
		{
		theStyle  = &kFlatButtonStyleActive;
		textColor = kThemeTextColorBevelButtonActive;
		}

	else
		{
		theStyle  = &kFlatButtonStyleInactive;
		textColor = kThemeTextColorBevelButtonInactive;
		}



	// Draw the top edge
	cgContext.SetAntiAlias(false);

	cgContext.MoveTo(   minX, minY + 1.0f);
	cgContext.AddLineTo(maxX, minY + 1.0f);

	cgContext.SetStrokeColor(theStyle->frameTopOuter);
	cgContext.StrokePath();

	cgContext.MoveTo(   minX, minY + 2.0f);
	cgContext.AddLineTo(maxX, minY + 2.0f);

	cgContext.SetStrokeColor(theStyle->frameTopInner);
	cgContext.StrokePath();



	// Draw the bottom edge
	cgContext.MoveTo(   minX, maxY);
	cgContext.AddLineTo(maxX, maxY);

	cgContext.SetStrokeColor(theStyle->frameBottomOuter);
	cgContext.StrokePath();

	cgContext.MoveTo(   minX, maxY - 1.0f);
	cgContext.AddLineTo(maxX, maxY - 1.0f);

	cgContext.SetStrokeColor(theStyle->frameBottomInner);
	cgContext.StrokePath();



	// Draw the sides
	cgContext.MoveTo(   minX, minY + 3.0f);
	cgContext.AddLineTo(minX, maxY - 2.0f);

	cgContext.MoveTo(   maxX - 1.0f, minY + 3.0f);
	cgContext.AddLineTo(maxX - 1.0f, maxY - 2.0f);

	cgContext.SetStrokeColor(theStyle->frameSide);
	cgContext.StrokePath();



	// Draw the face
	theRect = CGRectInset(theRect, 1.0f, 2.0f);
	
	cgContext.SetFillColor(theStyle->faceTop);
	cgContext.FillRect(theRect);

	theSplit = CGRectGetHeight(theRect) * kButtonFaceSplit;
	theRect.origin.y    += theSplit;
	theRect.size.height -= theSplit;

	cgContext.SetFillColor(theStyle->faceBottom);
	cgContext.FillRect(theRect);



	// Draw the label
	if (theText.IsNotEmpty())
		{
		cgContext.SetAntiAlias(true);
		cgContext.SetTextColor(textColor);

		cgContext.DrawThemeText(theText, GetBounds(), kThemeTextButtonCenter);
		}

	return(noErr);
}


