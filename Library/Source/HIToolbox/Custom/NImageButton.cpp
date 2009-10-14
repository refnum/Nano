/*	NAME:
		NImageButton.cpp

	DESCRIPTION:
		Image button.
		
		An "image button" is a button whose content is determined by a set of
		externally supplied images.
		
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
#include "NImageButton.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagButtonImageActive							= 'actb';
static const OSType kTagButtonImageInactive							= 'inab';
static const OSType kTagButtonImagePressed							= 'preb';


// Geometry
static const float kButtonRadius									= 4.0f;





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NImageButton, "com.nano.nimagebutton", kHIBevelButtonClassID);





//============================================================================
//		NImageButton::NImageButton : Constructor.
//----------------------------------------------------------------------------
NImageButton::NImageButton(HIWindowRef theWindow, const HIRect &theRect)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);
}





//============================================================================
//		NImageButton::~NImageButton : Destructor.
//----------------------------------------------------------------------------
NImageButton::~NImageButton(void)
{
}





//============================================================================
//		NImageButton::GetActiveImage : Get the active image.
//----------------------------------------------------------------------------
NCGImage NImageButton::GetActiveImage(void) const
{


	// Get the image
	return(mImageActive);
}





//============================================================================
//		NImageButton::SetActiveImage : Set the active image.
//----------------------------------------------------------------------------
void NImageButton::SetActiveImage(const NCGImage &theImage)
{


	// Set the image
	mImageActive = theImage;
}





//============================================================================
//		NImageButton::GetInactiveImage : Get the inactive image.
//----------------------------------------------------------------------------
NCGImage NImageButton::GetInactiveImage(void) const
{


	// Get the image
	return(mImageInactive);
}





//============================================================================
//		NImageButton::SetInactiveImage : Set the inactive image.
//----------------------------------------------------------------------------
void NImageButton::SetInactiveImage(const NCGImage &theImage)
{


	// Set the image
	mImageInactive = theImage;
}





//============================================================================
//		NImageButton::GetPressedImage : Get the pressed image.
//----------------------------------------------------------------------------
NCGImage NImageButton::GetPressedImage(void) const
{


	// Get the image
	return(mImagePressed);
}





//============================================================================
//		NImageButton::SetPressedImage : Set the pressed image.
//----------------------------------------------------------------------------
void NImageButton::SetPressedImage(const NCGImage &theImage)
{


	// Set the image
	mImagePressed = theImage;
}





//============================================================================
//		NImageButton::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NImageButton::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw	},
										{ kEventClassNone,    kEventKindNone	} };





	// Apply our properties
	if (HasProperty(                              kPropertyNano, kTagButtonImageActive))
		SetActiveImage(NCGImage(GetPropertyString(kPropertyNano, kTagButtonImageActive)));

	if (HasProperty(                                kPropertyNano, kTagButtonImageInactive))
		SetInactiveImage(NCGImage(GetPropertyString(kPropertyNano, kTagButtonImageInactive)));

	if (HasProperty(                               kPropertyNano, kTagButtonImagePressed))
		SetPressedImage(NCGImage(GetPropertyString(kPropertyNano, kTagButtonImagePressed)));



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NImageButton::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NImageButton::DoEventControlDraw(NCarbonEvent &theEvent)
{	NColor				fillColor, strokeColor;
	bool				isEnabled, isPressed;
	NCGContext			cgContext(theEvent);
	const NCGImage		*theImage;
	ThemeTextColor		textColor;
	NString				theText;
	HIRect				theRect;



	// Get the state we need
	theRect = GetBounds();
	theText = GetTextValue();

	isEnabled = IsEnabled();
	isPressed = (GetHilite() == kControlButtonPart);

	if (isPressed)
		{
		theImage  = &mImagePressed;
		textColor = kThemeTextColorBevelButtonPressed;

		if (!theImage->IsValid())
			{
			fillColor   = kColorBlack;
			strokeColor = kColorWhite;
			textColor   = kThemeTextColorWhite;
			}
		}

	else if (isEnabled)
		{
		theImage  = &mImageActive;
		textColor = kThemeTextColorBevelButtonActive;

		if (!theImage->IsValid())
			{
			fillColor   = kColorWhite;
			strokeColor = kColorBlack;
			}
		}
	
	else
		{
		theImage  = &mImagePressed;
		textColor = kThemeTextColorBevelButtonInactive;

		if (!theImage->IsValid())
			{
			fillColor   = kColorGrayLight;
			strokeColor = kColorGrayDark;
			textColor   = kThemeTextColorWhite;
			}
		}



	// Draw the button
	if (theImage->IsValid())
		cgContext.DrawImage(*theImage, theRect);
	else
		{
		cgContext.SetFillColor(    fillColor);
		cgContext.SetStrokeColor(strokeColor);
		cgContext.AddRect(theRect, kButtonRadius);
		cgContext.DrawPath();
		}



	// Draw the label
	if (theText.IsNotEmpty())
		{
		cgContext.SetTextColor(textColor);
		cgContext.DrawThemeText(theText, theRect, kThemeTextButtonCenter);
		}

	return(noErr);
}


