/*	NAME:
		NStaticText.cpp

	DESCRIPTION:
		Static text control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStaticText.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagShadowColor									= 'scol';
static const OSType kTagShadowBlur									= 'blur';
static const OSType kTagShadowOffsetX								= 'offx';
static const OSType kTagShadowOffsetY								= 'offy';


// Appearance
static const CGShadowInfo kDefaultShadow							= { kColorClear, 2.0f, CGSizeMake(0.0f, -1.0f) };





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NStaticText, kHIStaticTextViewClassID, NULL);





//============================================================================
//		NStaticText::NStaticText : Constructor.
//----------------------------------------------------------------------------
NStaticText::NStaticText(HIWindowRef					theWindow,
							const HIRect				&theRect,
							const NString				&theValue,
							const ControlFontStyleRec	*theStyle)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateStaticTextControl(NULL, &kQDRectZero, theValue, theStyle, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NStaticText::~NStaticText : Destructor.
//----------------------------------------------------------------------------
NStaticText::~NStaticText(void)
{
}





//============================================================================
//		NStaticText::GetSize : Get the size.
//----------------------------------------------------------------------------
UInt32 NStaticText::GetSize(void) const
{


	// Get the size
	return(GetTextValue().GetSize());
}





//============================================================================
//		NStaticText::GetTextHeight : Get the text height.
//----------------------------------------------------------------------------
float NStaticText::GetTextHeight(void) const
{


	// Get the height
	return((float) GetControlUInt16(kControlEntireControl, kControlStaticTextTextHeightTag));
}





//============================================================================
//		NStaticText::GetTruncation : Get the truncation mode.
//----------------------------------------------------------------------------
TruncCode NStaticText::GetTruncation(void) const
{


	// Get the mode
	return((TruncCode) GetControlUInt16(kControlEntireControl, kControlStaticTextTruncTag));
}





//============================================================================
//		NStaticText::SetTruncation : Set the truncation mode.
//----------------------------------------------------------------------------
void NStaticText::SetTruncation(TruncCode truncMode)
{


	// Set the mode
	SetControlUInt16(kControlEntireControl, kControlStaticTextTruncTag, truncMode);
}





//============================================================================
//		NStaticText::IsMultiLine : Is this a mult-line text field?
//----------------------------------------------------------------------------
bool NStaticText::IsMultiLine(void) const
{


	// Get the flag
	return(GetControlBoolean(kControlEntireControl, kControlStaticTextIsMultilineTag));
}





//============================================================================
//		NStaticText::SetMultiLine : Set the multi-line state.
//----------------------------------------------------------------------------
void NStaticText::SetMultiLine(bool multiLine)
{


	// Set the flag
	SetControlBoolean(kControlEntireControl, kControlStaticTextIsMultilineTag, multiLine);
}





//============================================================================
//		NStaticText::GetShadow : Get the shadow.
//----------------------------------------------------------------------------
CGShadowInfo NStaticText::GetShadow(void) const
{


	// Get the shadow
	return(mShadow);
}





//============================================================================
//		NStaticText::SetShadow : Set the shadow.
//----------------------------------------------------------------------------
void NStaticText::SetShadow(const CGShadowInfo &theShadow)
{


	// Set the shadow
	mShadow = theShadow;
	
	SetNeedsDisplay();
}





//============================================================================
//		NStaticText::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NStaticText::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw },
										{ kEventClassNone,    kEventKindNone	} };



	// Initialize ourselves
	mShadow = kDefaultShadow;



	// Register for events
	AddEvents(theEvents);



	// Apply our properties
	if (HasProperty(                             kPropertyNano, kTagShadowColor))
		mShadow.color = NColor(GetPropertyString(kPropertyNano, kTagShadowColor));

	if (HasProperty(                      kPropertyNano, kTagShadowBlur))
		mShadow.blur = GetPropertyFloat32(kPropertyNano, kTagShadowBlur);

	if (HasProperty(                              kPropertyNano, kTagShadowOffsetX))
		mShadow.offset.width = GetPropertyFloat32(kPropertyNano, kTagShadowOffsetX);

	if (HasProperty(                               kPropertyNano, kTagShadowOffsetY))
		mShadow.offset.height = GetPropertyFloat32(kPropertyNano, kTagShadowOffsetY);
}





//============================================================================
//		NStaticText::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NStaticText::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext			cgContext(theEvent);
	StContextState		cgState(cgContext);
	bool				doShadow;
	OSStatus			theErr;



	// Get the state we need
	doShadow = NMathUtilities::NotZero(mShadow.color.GetAlpha());



	// Draw the text
	if (doShadow)
		{
		cgContext.SaveState();
		cgContext.SetShadow(mShadow);
		}
	
	theErr = CallNextHandler(theEvent);
	NN_ASSERT_NOERR(theErr);

	if (doShadow)
		cgContext.RestoreState();

	return(theErr);
}


