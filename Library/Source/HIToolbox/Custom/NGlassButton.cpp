/*	NAME:
		NGlassButton.cpp

	DESCRIPTION:
		Glass button.
		
		A "glass button" is equivalent to Cocoa's "round textured" button.
		
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
#include "NGlassButton.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
typedef struct {
	NColor			frameTop;
	NColor			frameBottom;
	NColor			frameSurround;
} GlassButtonStyle;





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Geometry
static const float kFrameWidth									=  1.9f;
static const float kFrameRadius									=  4.0f;
static const float kFrameInset									=  3.0f;
static const float kFaceRadius									= 12.0f;


// Colors
static const GlassButtonStyle kGlassButtonStyleActive			= { NColor(0x949394FF),
																	NColor(0xC9C9C9FF),
																	NColor(0x666666FF) };

static const GlassButtonStyle kGlassButtonStyleInactive			= kGlassButtonStyleActive;

static const GlassButtonStyle kGlassButtonStylePressed			= kGlassButtonStyleActive;

static const NColor kSwoopActiveTopLight						= NColor(0xE8E8E8FF);
static const NColor kSwoopActiveTopDark							= NColor(0xACACACFF);
static const NColor kSwoopActiveMiddle							= NColor(0xD7D7D7FF);
static const NColor kSwoopActiveBottom							= NColor(0xF9F9F9FF);

static const NColor kSwoopInactiveTopLight						= kSwoopActiveTopLight;
static const NColor kSwoopInactiveTopDark						= kSwoopActiveTopDark;
static const NColor kSwoopInactiveMiddle						= kSwoopActiveMiddle;
static const NColor kSwoopInactiveBottom						= kSwoopActiveBottom;

static const NColor kSwoopPressedTopLight						= NColor(0xB3B3B3FF);
static const NColor kSwoopPressedTopDark						= NColor(0x858585FF);
static const NColor kSwoopPressedMiddle							= NColor(0xAFAFAFFF);
static const NColor kSwoopPressedBottom							= NColor(0xD1D1D1FF);





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NGlassButton, "com.nano.nglassbutton", kHIBevelButtonClassID);





//============================================================================
//		NGlassButton::NGlassButton : Constructor.
//----------------------------------------------------------------------------
NGlassButton::NGlassButton(HIWindowRef theWindow, const HIRect &theRect)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);
}





//============================================================================
//		NGlassButton::~NGlassButton : Destructor.
//----------------------------------------------------------------------------
NGlassButton::~NGlassButton(void)
{
}





//============================================================================
//		NGlassButton::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NGlassButton::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw	},
										{ kEventClassNone,    kEventKindNone	} };



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NGlassButton::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NGlassButton::DoEventControlDraw(NCarbonEvent &theEvent)
{	NShadingSample				samplesBottom[] = { { 0.0f, kColorGreen }, { 0.5f, kColorYellow }, { 1.0f, kColorBlue } };
	NShadingSample				samplesTop[]    = { { 0.0f, kColorRed   }, { 1.0f, kColorBlue   } };
	float						minX, maxX, midX, minY, maxY, midY;
	bool						isEnabled, isPressed;
	NCGContext					cgContext(theEvent);
	const GlassButtonStyle		*theStyle;
	ThemeTextColor				textColor;
	NString						theText;
	HIRect						theRect;



	// Get the state we need
	theRect = GetBounds();
	theText = GetTextValue();

	minX = CGRectGetMinX(theRect);
	maxX = CGRectGetMaxX(theRect);
	midX = minX + floor(CGRectGetWidth(theRect) / 2.0f) + 1.0f;

	minY = CGRectGetMinY(theRect);
	maxY = CGRectGetMaxY(theRect);
	midY = minY + floor(CGRectGetHeight(theRect) / 2.0f) + 1.0f;

	isEnabled = IsEnabled();
	isPressed = (GetHilite() == kControlButtonPart);
 
	if (isPressed)
		{
		theStyle  = &kGlassButtonStylePressed;
		textColor = kThemeTextColorBevelButtonPressed;
		
		samplesTop[0].theColor = kSwoopPressedTopLight;
		samplesTop[1].theColor = kSwoopPressedBottom;

		samplesBottom[0].theColor = kSwoopPressedTopDark;
		samplesBottom[1].theColor = kSwoopPressedMiddle;
		samplesBottom[2].theColor = kSwoopPressedBottom;
		}

	else if (isEnabled)
		{
		theStyle  = &kGlassButtonStyleActive;
		textColor = kThemeTextColorBevelButtonActive;

		samplesTop[0].theColor = kSwoopActiveTopLight;
		samplesTop[1].theColor = kSwoopActiveBottom;

		samplesBottom[0].theColor = kSwoopActiveTopDark;
		samplesBottom[1].theColor = kSwoopActiveMiddle;
		samplesBottom[2].theColor = kSwoopActiveBottom;
		}

	else
		{
		theStyle  = &kGlassButtonStyleInactive;
		textColor = kThemeTextColorBevelButtonInactive;

		samplesTop[0].theColor = kSwoopInactiveTopLight;
		samplesTop[1].theColor = kSwoopInactiveBottom;

		samplesBottom[0].theColor = kSwoopInactiveTopDark;
		samplesBottom[1].theColor = kSwoopInactiveMiddle;
		samplesBottom[2].theColor = kSwoopInactiveBottom;
		}

	mShadingTop.SetSamples(   samplesTop);
	mShadingBottom.SetSamples(samplesBottom);

	mShadingTop.ApplyLinearShading(   kLinearTopBottom, minY, maxY);
	mShadingBottom.ApplyLinearShading(kLinearTopBottom, minY, maxY);



	// Draw the frame
	cgContext.SetLineWidth(kFrameWidth);
	cgContext.SetStrokeColor(theStyle->frameSurround);
	cgContext.AddRect(CGRectInset(theRect, 1, 1), kFrameRadius);
	cgContext.StrokePath();

	cgContext.SetStrokeColor(theStyle->frameTop);
	cgContext.MoveTo(   minX + kFrameInset, minY);
	cgContext.AddLineTo(maxX - kFrameInset, minY);
	cgContext.StrokePath();

	cgContext.SetStrokeColor(theStyle->frameBottom);
	cgContext.MoveTo(   minX + kFrameInset, maxY);
	cgContext.AddLineTo(maxX - kFrameInset, maxY);
	cgContext.StrokePath();



	// Draw the face
	cgContext.SaveState();

		cgContext.AddRect(CGRectInset(theRect, 1, 2), kFrameRadius);
		cgContext.ClipToPath();
		cgContext.DrawShading(mShadingTop);

		cgContext.MoveTo(minX, maxY);
		cgContext.AddArcToPoint(CGPointMake(minX, midY), CGPointMake(minX + kFaceRadius, midY), kFaceRadius);
		cgContext.AddLineTo(maxX - kFaceRadius, midY);
		cgContext.AddArcToPoint(CGPointMake(maxX - 1.0f, midY), CGPointMake(maxX - 1.0f, minY), kFaceRadius);
		cgContext.AddLineTo(maxX, maxY);
		cgContext.AddLineTo(minX, maxY);
		cgContext.ClipToPath();
		cgContext.DrawShading(mShadingBottom);

	cgContext.RestoreState();



	// Draw the label
	if (theText.IsNotEmpty())
		{
		cgContext.SetTextColor(textColor);
		cgContext.DrawThemeText(theText, GetBounds(), kThemeTextButtonCenter);
		}

	return(noErr);
}


