/*	NAME:
		NBevelButton.cpp

	DESCRIPTION:
		Bevel button control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NSystemUtilities.h"
#include "NCFBundle.h"
#include "NBevelButton.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagIcon										= 'icon';
static const OSType kTagImage										= 'imag';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NBevelButton, kHIBevelButtonClassID, NULL);





//============================================================================
//		NBevelButton::NBevelButton : Constructor.
//----------------------------------------------------------------------------
NBevelButton::NBevelButton(HIWindowRef							theWindow,
							const HIRect						&theRect,
							const NString						&theTitle,
							ControlBevelThickness				theThickness,
							ControlBevelButtonBehavior			theBehaviour,
							const ControlButtonContentInfo		*theContent,
							ControlBevelButtonMenuBehavior		menuBehaviour,
							ControlBevelButtonMenuPlacement		menuPlacement)
{	ControlButtonContentInfo	tmpContent = kControlContentNone;
	HIViewRef					theView;
	OSStatus					theErr;



	// Prepare the content
	if (theContent != NULL)
		tmpContent = *theContent;



	// Create the view
	theErr = CreateBevelButtonControl(NULL, &kQDRectZero, theTitle, theThickness, theBehaviour,
										&tmpContent, 0, menuBehaviour, menuPlacement, &theView);
	NN_ASSERT_NOERR(theErr);

	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NBevelButton::~NBevelButton : Destructor.
//----------------------------------------------------------------------------
NBevelButton::~NBevelButton(void)
{
}





//============================================================================
//		NBevelButton::GetKind : Get the kind.
//----------------------------------------------------------------------------
ThemeButtonKind NBevelButton::GetKind(void) const
{


	// Get the kind
	return(GetControlUInt16(kControlEntireControl, kControlBevelButtonKindTag));
}





//============================================================================
//		NBevelButton::SetKind : Set the kind.
//----------------------------------------------------------------------------
void NBevelButton::SetKind(ThemeButtonKind theKind)
{


	// Set the kind
	SetControlUInt16(kControlEntireControl, kControlBevelButtonKindTag, theKind);
}





//============================================================================
//		NBevelButton::GetIcon : Get the icon.
//----------------------------------------------------------------------------
NIcon NBevelButton::GetIcon(void) const
{


	// Get the icon
	return(GetContentIcon(kControlEntireControl, kControlBevelButtonContentTag));
}





//============================================================================
//		NBevelButton::SetIcon : Set the icon.
//----------------------------------------------------------------------------
void NBevelButton::SetIcon(const NIcon &theIcon)
{


	// Set the icon
	SetContentIcon(kControlEntireControl, kControlBevelButtonContentTag, theIcon);
}





//============================================================================
//		NBevelButton::GetImage : Get the image.
//----------------------------------------------------------------------------
NCGImage NBevelButton::GetImage(void) const
{


	// Validate our state
	NN_ASSERT(NSystemUtilities::SystemIsAtLeast(kSystemTenFour));



	// Get the image
	return(GetContentImage(kControlEntireControl, kControlBevelButtonContentTag));
}





//============================================================================
//		NBevelButton::SetImage : Set the image.
//----------------------------------------------------------------------------
void NBevelButton::SetImage(const NCGImage &theImage)
{


	// Validate our state
	NN_ASSERT(NSystemUtilities::SystemIsAtLeast(kSystemTenFour));



	// Set the image
	SetContentImage(kControlEntireControl, kControlBevelButtonContentTag, theImage);
}





//============================================================================
//		NBevelButton::GetTransform : Get the transform.
//----------------------------------------------------------------------------
IconTransformType NBevelButton::GetTransform(void) const
{


	// Get the transform
	return(GetControlUInt16(kControlEntireControl, kControlBevelButtonTransformTag));
}





//============================================================================
//		NBevelButton::SetTransform : Set the transform.
//----------------------------------------------------------------------------
void NBevelButton::SetTransform(IconTransformType theTransform)
{


	// Set the transform
	SetControlUInt16(kControlEntireControl, kControlBevelButtonTransformTag, theTransform);
}





//============================================================================
//		NBevelButton::GetMenu : Get the menu.
//----------------------------------------------------------------------------
MenuRef NBevelButton::GetMenu(void) const
{	MenuRef		theMenu;



	// Get the menu
	GetControlData(kControlEntireControl, kControlBevelButtonMenuRefTag, sizeof(theMenu), &theMenu);
	
	return(theMenu);
}





//============================================================================
//		NBevelButton::SetMenu : Set the menu.
//----------------------------------------------------------------------------
void NBevelButton::SetMenu(MenuRef theMenu)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theMenu != NULL);



	// Retain the menu
	//
	// The caller should be able to release their reference, so we retain the menu
	// for the control and transfer ownership with kControlBevelButtonOwnedMenuRefTag.
	theErr = RetainMenu(theMenu);
	NN_ASSERT_NOERR(theErr);



	// Set the menu
	SetControlData(kControlEntireControl, kControlBevelButtonOwnedMenuRefTag, sizeof(theMenu), &theMenu);
}





//============================================================================
//		NBevelButton::GetMenuValue : Get the menu value.
//----------------------------------------------------------------------------
MenuItemIndex NBevelButton::GetMenuValue(bool getLast) const
{	MenuItemIndex	theIndex;



	// Get the menu value
	if (getLast)
		theIndex = GetControlUInt16(kControlEntireControl, kControlBevelButtonLastMenuTag);
	else
		theIndex = GetControlUInt16(kControlEntireControl, kControlBevelButtonMenuValueTag);
	
	return(theIndex);
}





//============================================================================
//		NBevelButton::SetMenuValue : Set the menu value.
//----------------------------------------------------------------------------
void NBevelButton::SetMenuValue(MenuItemIndex theIndex)
{


	// Set the menu value
	SetControlUInt16(kControlEntireControl, kControlBevelButtonMenuValueTag, theIndex);
}





//============================================================================
//		NBevelButton::GetMenuDelay : Get the menu delay.
//----------------------------------------------------------------------------
EventTime NBevelButton::GetMenuDelay(void) const
{	SInt32		numTicks;
	EventTime	theDelay;



	// Get the delay
	numTicks = GetControlUInt32(kControlEntireControl, kControlBevelButtonMenuDelayTag);
	theDelay = TicksToEventTime(numTicks);
	
	return(theDelay);
}





//============================================================================
//		NBevelButton::SetMenuDelay : Set the menu delay.
//----------------------------------------------------------------------------
void NBevelButton::SetMenuDelay(EventTime theDelay)
{	SInt32		numTicks;



	// Get the delay
	numTicks = EventTimeToTicks(theDelay);
	SetControlUInt32(kControlEntireControl, kControlBevelButtonMenuDelayTag, numTicks);
}





//============================================================================
//		NBevelButton::IsMenuGlyphCentered : Is the menu glyph centered?
//----------------------------------------------------------------------------
bool NBevelButton::IsMenuGlyphCentered(void) const
{


	// Get the flag
	return(GetControlBoolean(kControlEntireControl, kControlBevelButtonCenterPopupGlyphTag));
}





//============================================================================
//		NBevelButton::SetMenuGlyphCentered : Set the menu glyph center state.
//----------------------------------------------------------------------------
void NBevelButton::SetMenuGlyphCentered(bool isCentered)
{


	// Set the flag
	SetControlBoolean(kControlEntireControl, kControlBevelButtonCenterPopupGlyphTag, isCentered);
}





//============================================================================
//		NBevelButton::IsMenuMultiValue : Can the menu have multiple values?
//----------------------------------------------------------------------------
bool NBevelButton::IsMenuMultiValue(void) const
{


	// Get the flag
	return(GetControlBoolean(kControlEntireControl, kControlBevelButtonIsMultiValueMenuTag));
}





//============================================================================
//		NBevelButton::SetMenuMultiValue : Set the menu multi-value state.
//----------------------------------------------------------------------------
void NBevelButton::SetMenuMultiValue(bool multiValue)
{


	// Set the flag
	SetControlBoolean(kControlEntireControl, kControlBevelButtonIsMultiValueMenuTag, multiValue);
}





//============================================================================
//		NBevelButton::GetTextPlacement : Get the text placement.
//----------------------------------------------------------------------------
ControlButtonTextPlacement NBevelButton::GetTextPlacement(void) const
{


	// Get the placement
	return(GetControlUInt16(kControlEntireControl, kControlBevelButtonTextPlaceTag));
}





//============================================================================
//		NBevelButton::SetTextPlacement : Set the text placement.
//----------------------------------------------------------------------------
void NBevelButton::SetTextPlacement(ControlButtonTextPlacement thePlacement)
{


	// Set the placement
	SetControlUInt16(kControlEntireControl, kControlBevelButtonTextPlaceTag, thePlacement);
}





//============================================================================
//		NBevelButton::GetTextAlignment : Get the text alignment.
//----------------------------------------------------------------------------
ControlButtonTextAlignment NBevelButton::GetTextAlignment(void) const
{


	// Get the alignment
	return(GetControlUInt16(kControlEntireControl, kControlBevelButtonTextAlignTag));
}





//============================================================================
//		NBevelButton::SetTextAlignment : Set the text alignment.
//----------------------------------------------------------------------------
void NBevelButton::SetTextAlignment(ControlButtonTextAlignment theAlignment)
{


	// Set the alignment
	SetControlUInt16(kControlEntireControl, kControlBevelButtonTextAlignTag, theAlignment);
}





//============================================================================
//		NBevelButton::GetTextOffset : Get the text offset.
//----------------------------------------------------------------------------
SInt16 NBevelButton::GetTextOffset(void) const
{


	// Get the offset
	return(GetControlUInt16(kControlEntireControl, kControlBevelButtonTextOffsetTag));
}





//============================================================================
//		NBevelButton::SetTextOffset : Set the text offset.
//----------------------------------------------------------------------------
void NBevelButton::SetTextOffset(SInt16 theOffset)
{


	// Set the offset
	SetControlUInt16(kControlEntireControl, kControlBevelButtonTextOffsetTag, theOffset);
}





//============================================================================
//		NBevelButton::GetGraphicAlignment : Get the graphic alignment.
//----------------------------------------------------------------------------
ControlButtonGraphicAlignment NBevelButton::GetGraphicAlignment(void) const
{


	// Get the alignment
	return(GetControlUInt16(kControlEntireControl, kControlBevelButtonGraphicAlignTag));
}





//============================================================================
//		NBevelButton::SetGraphicAlignment : Set the graphic alignment.
//----------------------------------------------------------------------------
void NBevelButton::SetGraphicAlignment(ControlButtonGraphicAlignment theAlignment)
{


	// Set the alignment
	SetControlUInt16(kControlEntireControl, kControlBevelButtonGraphicAlignTag, theAlignment);
}





//============================================================================
//		NBevelButton::GetGraphicOffset : Get the graphic offset.
//----------------------------------------------------------------------------
HIPoint NBevelButton::GetGraphicOffset(void) const
{	HIPoint		theOffset;
	Point		qdPoint;



	// Get the offset
	GetControlData(kControlEntireControl, kControlBevelButtonGraphicOffsetTag, sizeof(qdPoint), &qdPoint);
	NGeometryUtilities::ConvertPoint(qdPoint, theOffset);
	
	return(theOffset);
}





//============================================================================
//		NBevelButton::SetGraphicOffset : Set the graphic offset.
//----------------------------------------------------------------------------
void NBevelButton::SetGraphicOffset(const HIPoint &theOffset)
{	Point	qdPoint;



	// Set the offset
	NGeometryUtilities::ConvertPoint(theOffset, qdPoint);
	SetControlData(kControlEntireControl, kControlBevelButtonGraphicOffsetTag, sizeof(qdPoint), &qdPoint);
}





//============================================================================
//		NBevelButton::CanScaleGraphic : Can the graphic be scaled?
//----------------------------------------------------------------------------
bool NBevelButton::CanScaleGraphic(void) const
{


	// Get the flag
	return(GetControlBoolean(kControlEntireControl, kControlBevelButtonScaleIconTag));
}





//============================================================================
//		NBevelButton::SetScaleGraphic : Set the graphic scale state.
//----------------------------------------------------------------------------
void NBevelButton::SetScaleGraphic(bool canScale)
{


	// Set the flag
	SetControlBoolean(kControlEntireControl, kControlBevelButtonScaleIconTag, canScale);
}





//============================================================================
//		NBevelButton::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NBevelButton::InitializeView(void)
{


	// Apply our properties
	if (HasProperty(                              kPropertyNano, kTagIcon))
		SetIcon(NBundleResource(GetPropertyString(kPropertyNano, kTagIcon)));

	if (HasProperty(                               kPropertyNano, kTagImage))
		SetImage(NBundleResource(GetPropertyString(kPropertyNano, kTagImage)));
}



