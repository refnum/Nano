/*	NAME:
		NHIView.cpp

	DESCRIPTION:
		HIViewRef object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIViewUtilities.h"
#include "NSystemUtilities.h"
#include "NHelpUtilities.h"
#include "NMathUtilities.h"
#include "NPasteboard.h"
#include "NHIWindow.h"
#include "NCursor.h"
#include "NHIView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagValueMinimum								= 'min ';
static const OSType kTagValueMaximum								= 'max ';
static const OSType kTagValueCurrent								= 'valu';
static const OSType kTagValueText									= 'text';





//============================================================================
//		NHIView::NHIView : Constructor.
//----------------------------------------------------------------------------
NHIView::NHIView(void)
{


	// Initialize ourselves
	InitializeSelf();
}





//============================================================================
//		NHIView::~NHIView : Destructor.
//----------------------------------------------------------------------------
NHIView::~NHIView(void)
{
}





//============================================================================
//		NHIView::GetBounds : Get the bounds.
//----------------------------------------------------------------------------
HIRect NHIView::GetBounds(void) const
{	HIRect		theRect;
	OSStatus	theErr;



	// Get the bounds
	theRect = kHIRectZero;
	theErr  = HIViewGetBounds(*this, &theRect);
	NN_ASSERT_NOERR(theErr);
	
	return(theRect);
}





//============================================================================
//		NHIView::GetFrame : Get the frame.
//----------------------------------------------------------------------------
HIRect NHIView::GetFrame(void) const
{	HIRect		theRect;
	OSStatus	theErr;



	// Get the frame
	theRect = kHIRectZero;
	theErr  = HIViewGetFrame(*this, &theRect);
	NN_ASSERT_NOERR(theErr);
	
	return(theRect);
}





//============================================================================
//		NHIView::SetFrame : Set the frame.
//----------------------------------------------------------------------------
void NHIView::SetFrame(const HIRect &theRect)
{	OSStatus	theErr;



	// Set the frame
	theErr = HIViewSetFrame(*this, &theRect);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::GetIdealSize : Get the ideal size.
//----------------------------------------------------------------------------
HIRect NHIView::GetIdealSize(void) const
{	SInt16		baseOffset;
	HIRect		theRect;
	Rect		qdRect;
	OSStatus	theErr;



	// Get the ideal bounds
	//
	// HIViewGetOptimalBounds is 10.4 only, so we use the older API.
	qdRect = kQDRectZero;
	theErr = GetBestControlRect(*this, &qdRect, &baseOffset);

	NGeometryUtilities::ConvertRect(qdRect, theRect);
	
	return(theRect);
}





//============================================================================
//		NHIView::GetSizeConstraints : Get the size constraints.
//----------------------------------------------------------------------------
void NHIView::GetSizeConstraints(HISize *minSize, HISize *maxSize) const
{	OSStatus	theErr;



	// Get the size constraints
	//
	// We use HIViewGetSizeConstraints rather than returning our size fields,
	// since we want to trigger a kEventControlGetSizeConstraints event that
	// can be forwarded to system controls.
	//
	// The base NHIView class provides size fields for custom views, however
	// wrappers around system views need to forward the event to return the
	// system view's size constraints.
	theErr = HIViewGetSizeConstraints(*this, minSize, maxSize);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::SetSizeConstraints : Set the size constraints.
//----------------------------------------------------------------------------
void NHIView::SetSizeConstraints(const HISize *minSize, const HISize *maxSize)
{


	// Update our state
	if (minSize != NULL)
		mSizeMin = *minSize;

	if (maxSize != NULL)
		mSizeMax = *maxSize;
}





//============================================================================
//		NHIView::IsVisible : Is the view visible?
//----------------------------------------------------------------------------
bool NHIView::IsVisible(void) const
{


	// Get the state
	return(HIViewIsVisible(*this));
}





//============================================================================
//		NHIView::SetVisible : Set the visibility.
//----------------------------------------------------------------------------
void NHIView::SetVisible(bool isVisible)
{	OSStatus	theErr;



	// Set the state
	theErr = HIViewSetVisible(*this, isVisible);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::IsEnabled : Is the view enabled?
//----------------------------------------------------------------------------
bool NHIView::IsEnabled(void) const
{


	// Get the state
	return(IsControlEnabled(*this));
}





//============================================================================
//		NHIView::SetEnabled : Set the enabled state.
//----------------------------------------------------------------------------
void NHIView::SetEnabled(bool isEnabled)
{	OSStatus	theErr;



	// Set the state
	if (isEnabled)
		theErr = EnableControl(*this);
	else
		theErr = DisableControl(*this);

	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::IsActive : Is the view active?
//----------------------------------------------------------------------------
bool NHIView::IsActive(void) const
{


	// Get the state
	return(HIViewIsActive(*this, NULL));
}





//============================================================================
//		NHIView::SetActive : Set the active state.
//----------------------------------------------------------------------------
void NHIView::SetActive(bool isActive)
{	OSStatus	theErr;



	// Set the state
	theErr = HIViewSetActivated(*this, isActive);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::GetHilite : Get the hilite state.
//----------------------------------------------------------------------------
HIViewPartCode NHIView::GetHilite(void) const
{


	// Get the state
	return((HIViewPartCode) GetControlHilite(*this));
}





//============================================================================
//		NHIView::SetHilite : Set the hilite state.
//----------------------------------------------------------------------------
void NHIView::SetHilite(HIViewPartCode thePart)
{	OSStatus	theErr;



	// Set the state
	theErr = HIViewSetHilite(*this, thePart);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::GetViewID : Get the view ID.
//----------------------------------------------------------------------------
HIViewID NHIView::GetViewID(void) const
{	OSStatus	theErr;
	HIViewID	theID;



	// Get the view ID
	theID  = kHIViewIDNone;
	theErr = GetControlID(*this, &theID);
	NN_ASSERT_NOERR(theErr);
	
	return(theID);
}





//============================================================================
//		NHIView::SetViewID : Set the view ID.
//----------------------------------------------------------------------------
void NHIView::SetViewID(const HIViewID &theID)
{	OSStatus	theErr;



	// Set the view ID
	theErr = SetControlID(*this, &theID);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::GetCommandID : Get the command ID.
//----------------------------------------------------------------------------
UInt32 NHIView::GetCommandID(void) const
{	UInt32		theCmd;
	OSStatus	theErr;



	// Get the command ID
	theCmd = kHICommandNone;
	theErr = GetControlCommandID(*this, &theCmd);
	NN_ASSERT_NOERR(theErr);
	
	return(theCmd);
}





//============================================================================
//		NHIView::SetCommandID : Set the command ID.
//----------------------------------------------------------------------------
void NHIView::SetCommandID(UInt32 theCmd)
{	OSStatus	theErr;



	// Set the command ID
	theErr = SetControlCommandID(*this, theCmd);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//      NHIView::SendCommand : Send the command.
//----------------------------------------------------------------------------
OSStatus NHIView::SendCommand(void) const
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Validate our state
	NN_ASSERT(GetCommandID() != kHICommandNone);



	// Build the command info
	memset(&hiCmd, 0x00, sizeof(hiCmd));
	
	hiCmd.attributes     = kHICommandFromControl;
	hiCmd.commandID      = GetCommandID();
	hiCmd.source.control = *this;



	// Send the command
	theErr = ProcessHICommand((HICommand *) &hiCmd);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NHIView::GetMinimumValue : Get the minimum value.
//----------------------------------------------------------------------------
SInt32 NHIView::GetMinimumValue(void) const
{


	// Get the minimum value
	return(GetControl32BitMinimum(*this));
}





//============================================================================
//		NHIView::GetMaximumValue : Get the maximum value.
//----------------------------------------------------------------------------
SInt32 NHIView::GetMaximumValue(void) const
{


	// Get the maximum value
	return(GetControl32BitMaximum(*this));
}





//============================================================================
//		NHIView::GetCurrentValue : Get the current value.
//----------------------------------------------------------------------------
SInt32 NHIView::GetCurrentValue(void) const
{


	// Get the current value
	return(GetControl32BitValue(*this));
}





//============================================================================
//		NHIView::GetPercentValue : Get the percentage value.
//----------------------------------------------------------------------------
float NHIView::GetPercentValue(void) const
{	float		theRange, theOffset, theValue;



	// Get the percentage
	theRange  = GetMaximumValue() - GetMinimumValue();
	theOffset = GetCurrentValue() - GetMinimumValue();
	theValue  = (theOffset / theRange);

	return(theValue);
}





//============================================================================
//		NHIView::SetMinimumValue : Set the minimum value.
//----------------------------------------------------------------------------
void NHIView::SetMinimumValue(SInt32 theValue)
{


	// Set the minimum value
	SetControl32BitMinimum(*this, theValue);
}





//============================================================================
//		NHIView::SetMaximumValue : Set the maximum value.
//----------------------------------------------------------------------------
void NHIView::SetMaximumValue(SInt32 theValue)
{


	// Set the maximum value
	SetControl32BitMaximum(*this, theValue);
}





//============================================================================
//		NHIView::SetCurrentValue : Set the current value.
//----------------------------------------------------------------------------
void NHIView::SetCurrentValue(SInt32 theValue)
{


	// Validate our parameters
	NN_ASSERT_MSG(theValue >= GetMinimumValue() && theValue <= GetMaximumValue(), "SetCurrentValue will be clamped");



	// Set the current value
	SetControl32BitValue(*this, theValue);
}





//============================================================================
//		NHIView::SetPercentValue : Set the percentage value.
//----------------------------------------------------------------------------
void NHIView::SetPercentValue(float theValue)
{	float	theRange;
	


	// Set the percentage
	theRange = GetMaximumValue() - GetMinimumValue();
	theValue = GetMinimumValue() + (theValue * theRange);
	
	SetCurrentValue((SInt32) theValue);
}





//============================================================================
//		NHIView::GetTextValue : Get the text value.
//----------------------------------------------------------------------------
NString NHIView::GetTextValue(void) const
{	CFStringRef		cfString;
	NString			theValue;
	OSStatus		theErr;



	// Get the text value
	//
	// Most views with a text element support the kControlEditTextCFStringTag
	// tag either on the view or on kControlEditTextPart, and so we try that
	//first. If that fails, we fall back to the title.
	//
	// This is the same approach as HIViewCopyText, but supports 10.3.
	//
	// Note that GetControlData does not follow the CF model of Get/Copy/Create,
	// and so in both cases we take ownership of the returned string.
	GetControlData(kControlEntireControl, kControlEditTextCFStringTag, sizeof(cfString), &cfString);

	if (cfString == NULL)
		GetControlData(kControlEntireControl, kControlEditTextPart, sizeof(cfString), &cfString);

	if (cfString == NULL)
		theErr = CopyControlTitleAsCFString(*this, &cfString);

	if (cfString != NULL)
		theValue.Set(cfString);

	return(theValue);
}





//============================================================================
//		NHIView::SetTextValue : Set the text value.
//----------------------------------------------------------------------------
void NHIView::SetTextValue(const NString &theValue)
{	CFStringRef		cfString;
	OSStatus		theErr;



	// Set the text value
	//
	// Most views with a text element support the kControlEditTextCFStringTag
	// tag, and so we try that first - falling back to the title on failure.
	//
	// This is the same approach as HIViewSetText, but supports 10.3.
	cfString = theValue;
	theErr   = ::SetControlData(*this, kControlEntireControl, kControlEditTextCFStringTag, sizeof(cfString), &cfString);

	if (theErr != noErr)
		theErr = ::SetControlData(*this, kControlEntireControl, kControlEditTextPart, sizeof(cfString), &cfString);

	if (theErr != noErr)
		theErr = SetControlTitleWithCFString(*this, cfString);
		
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::GetControlSize : Get the control size.
//----------------------------------------------------------------------------
ControlSize NHIView::GetControlSize(void) const
{	ControlSize		theSize;



	// Get the control size
	theSize = (ControlSize) GetControlUInt16(kControlNoPart, kControlSizeTag);
	
	return(theSize);
}





//============================================================================
//		NHIView::SetControlSize : Set the control size.
//----------------------------------------------------------------------------
void NHIView::SetControlSize(ControlSize theSize)
{	ControlFontStyleRec		theStyle;



	// Set the control size
	SetControlUInt16(kControlNoPart, kControlSizeTag, theSize);



	// Adjust the font
	//
	// The control size and font size can be adjusted independently, however
	// as a convenience we keep them both in sync when changing the size.
	memset(&theStyle, 0x00, sizeof(theStyle));
	theStyle.flags = kControlUseThemeFontIDMask;

	switch (theSize) {
		case kControlSizeLarge:
			theStyle.font = kThemeSystemFont;
			break;

		case kControlSizeNormal:
			theStyle.font = kThemeSystemFont;
			break;

		case kControlSizeSmall:
			theStyle.font = kThemeSmallSystemFont;
			break;

		case kControlSizeMini:
			theStyle.font = kThemeMiniSystemFont;
			break;

		default:
			NN_LOG("Unknown control size: %d", theSize);
			break;
		}
	
	SetFontStyle(theStyle);
}





//============================================================================
//		NHIView::GetLayoutInfo : Get the layout info.
//----------------------------------------------------------------------------
HILayoutInfo NHIView::GetLayoutInfo(void) const
{	HILayoutInfo	theInfo;
	OSStatus		theErr;



	// Get the layout info
	theInfo = kHILayoutNone;
	theErr  = HIViewGetLayoutInfo(*this, &theInfo);
	NN_ASSERT_NOERR(theErr);
	
	return(theInfo);
}





//============================================================================
//		NHIView::SetLayoutInfo : Set the layout info.
//----------------------------------------------------------------------------
void NHIView::SetLayoutInfo(const HILayoutInfo &theInfo)
{	OSStatus	theErr;



	// Set the layout info
	theErr = HIViewSetLayoutInfo(*this, &theInfo);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::GetFeatures : Get the features.
//----------------------------------------------------------------------------
HIViewFeatures NHIView::GetFeatures(void) const
{	HIViewFeatures		theFeatures;
	OSStatus			theErr;



	// Get the features
	theFeatures = kHIViewFeatureNone;
	theErr      = HIViewGetFeatures(*this, &theFeatures);
	NN_ASSERT_NOERR(theErr);
	
	return(theFeatures);
}





//============================================================================
//		NHIView::SetFeatures : Set the features.
//----------------------------------------------------------------------------
void NHIView::SetFeatures(HIViewFeatures setThese, HIViewFeatures clearThese)
{	OSStatus	theErr;



	// Set the features
	theErr = HIViewChangeFeatures(*this, setThese, clearThese);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::GetAttributes : Get the attributes.
//----------------------------------------------------------------------------
OptionBits NHIView::GetAttributes(void) const
{	OptionBits		theAttributes;
	OSStatus		theErr;



	// Get the attributes
	theAttributes = 0;
	theErr        = HIViewGetAttributes(*this, &theAttributes);
	NN_ASSERT_NOERR(theErr);
	
	return(theAttributes);
}





//============================================================================
//		NHIView::SetAttributes : Set the attributes.
//----------------------------------------------------------------------------
void NHIView::SetAttributes(OptionBits setThese, OptionBits clearThese)
{	OSStatus	theErr;



	// Set the attributes
	theErr = HIViewChangeAttributes(*this, setThese, clearThese);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::GetOptions : Get the options.
//----------------------------------------------------------------------------
OptionBits NHIView::GetOptions(void) const
{


	// Get the options
	return(mOptions);
}





//============================================================================
//		NHIView::SetOptions : Set the options.
//----------------------------------------------------------------------------
void NHIView::SetOptions(OptionBits setThese, OptionBits clearThese)
{


	// Set the options
	mOptions |=    setThese;
	mOptions &= ~clearThese;
}





//============================================================================
//		NHIView::GetHelpTag : Get the help tag.
//----------------------------------------------------------------------------
NString NHIView::GetHelpTag(bool isExtended) const
{	HMHelpContentRec	helpTag;
	OSStatus			theErr;



	// Get the help tag
	helpTag = NHelpUtilities::GetEmptyHelp();
	theErr  = HMGetControlHelpContent(*this, &helpTag);
	NN_ASSERT(theErr == noErr || theErr == controlPropertyNotFoundErr);

	return(NHelpUtilities::GetHelpString(helpTag, isExtended));
}





//============================================================================
//		NHIView::SetHelpTag : Set the help tag.
//----------------------------------------------------------------------------
void NHIView::SetHelpTag(const NString &theValue, bool isExtended)
{	HMHelpContentRec	helpTag;
	OSStatus			theErr;



	// Get the current value
	helpTag = NHelpUtilities::GetEmptyHelp();
	theErr  = HMGetControlHelpContent(*this, &helpTag);
	NN_ASSERT(theErr == noErr || theErr == controlPropertyNotFoundErr);



	// Set the help tag
	NHelpUtilities::SetHelpString(helpTag, theValue, isExtended);

	theErr = HMSetControlHelpContent(*this, &helpTag);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::GetHIWindow : Get the window.
//----------------------------------------------------------------------------
HIWindowRef NHIView::GetHIWindow(void) const
{


	// Get the window
	return(HIViewGetWindow(*this));
}





//============================================================================
//		NHIView::GetSuperView : Get the super-view.
//----------------------------------------------------------------------------
NHIView *NHIView::GetSuperView(void) const
{	NHIView		*superView;
	HIViewRef	hiView;



	// Get the super-view
	hiView    = HIViewGetSuperview(*this);
	superView = NHIViewUtilities::GetView(hiView);

	return(superView);
}





//============================================================================
//		NHIView::GetSubViewCount : Get the number of sub-views.
//----------------------------------------------------------------------------
UInt32 NHIView::GetSubViewCount(void) const
{	UInt32		numViews;
	HIViewRef	subView;



	// Get the number of sub-views
	//
	// HIViewCountSubviews requires Mac OS X 10.4 or later.
	if (HIViewCountSubviews != NULL)
		numViews = HIViewCountSubviews(*this);
	else
		{
		numViews = 0;
		subView  = HIViewGetFirstSubview(*this);

		while (subView != NULL)
			{
			numViews++;
			subView = HIViewGetNextView(subView);
			}
		}
	
	return(numViews);
}





//============================================================================
//		NHIView::GetSubView : Get a sub-view.
//----------------------------------------------------------------------------
NHIView *NHIView::GetSubView(UInt32 theIndex) const
{	NHIView		*theView;
	HIViewRef	hiView;
	OSStatus	theErr;
	UInt32		n;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSubViewCount());



	// Get the sub-view
	//
	// HIViewGetIndexedSubview requires Mac OS X 10.4 or later
	hiView = NULL;
	
	if (HIViewGetIndexedSubview != NULL)
		{
		theErr = HIViewGetIndexedSubview(*this, theIndex + 1, &hiView);
		NN_ASSERT_NOERR(theErr);
		}
	else
		{
		hiView = HIViewGetFirstSubview(*this);
		
		for (n = 0; n < theIndex; n++)
			hiView = HIViewGetNextView(hiView);
		}



	// Get the wrapper
	theView = NHIViewUtilities::GetView(hiView);
	
	return(theView);
}





//============================================================================
//		NHIView::AddSubView : Add a sub-view.
//----------------------------------------------------------------------------
void NHIView::AddSubView(NHIView *theView)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theView != NULL);



	// Add the sub-view
	theErr = HIViewAddSubview(*this, *theView);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::RemoveSubView : Remove a sub-view.
//----------------------------------------------------------------------------
void NHIView::RemoveSubView(NHIView *theView, bool andDelete)
{


	// Validate our parameters
	NN_ASSERT(theView != NULL);



	// Remove the sub-view
	theView->RemoveFromSuperView();
	
	if (andDelete)
		delete theView;
}





//============================================================================
//		NHIView::RemoveFromSuperView : Remove this view from its super-view.
//----------------------------------------------------------------------------
void NHIView::RemoveFromSuperView(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(GetSuperView() != NULL);



	// Remove ourselves from our super-view
	theErr = HIViewRemoveFromSuperview(*this);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::RemoveSubViews : Remove all sub-views from this view.
//----------------------------------------------------------------------------
void NHIView::RemoveSubViews(void)
{


	// Remove the sub-views
	while (GetSubViewCount() != 0)
		RemoveSubView(GetSubView(0), true);
}





//============================================================================
//		NHIView::SetNeedsDisplay : Set the display state.
//----------------------------------------------------------------------------
void NHIView::SetNeedsDisplay(bool needsDisplay, const HIRect *theRect)
{	OSStatus	theErr;



	// Set the display state
	if (theRect != NULL)
		theErr = HIViewSetNeedsDisplayInRect(*this, theRect, needsDisplay);
	else
		theErr = HIViewSetNeedsDisplay(      *this,          needsDisplay);
	
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::GetFontStyle : Get the font style.
//----------------------------------------------------------------------------
ControlFontStyleRec NHIView::GetFontStyle(void) const
{	ControlFontStyleRec		theStyle;



	// Get the font style
	//
	// The font style is not supported by all controls.
	GetControlData(kControlEntireControl, kControlFontStyleTag, sizeof(theStyle), &theStyle);

	return(theStyle);
}





//============================================================================
//		NHIView::SetFontStyle : Set the font style.
//----------------------------------------------------------------------------
void NHIView::SetFontStyle(const ControlFontStyleRec &theStyle)
{	OSStatus	theErr;



	// Set the font style
	//
	// The font style is not supported by all controls.
	theErr = SetControlFontStyle(*this, &theStyle);
	if (theErr != unimpErr && theErr != errDataNotSupported)
		NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::SetZOrder : Set the Z-order.
//----------------------------------------------------------------------------
void NHIView::SetZOrder(HIViewZOrderOp zOrder, NHIView *relativeTo)
{	HIViewRef	hiView;
	OSStatus	theErr;



	// Set the Z-order
	hiView = (relativeTo == NULL) ? NULL : (HIViewRef) *relativeTo;
	theErr = HIViewSetZOrder(*this, zOrder, hiView);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::Render : Render the view.
//----------------------------------------------------------------------------
void NHIView::Render(EventTime waitFor)
{	OSStatus	theErr;



	// Render the view
	SetNeedsDisplay();
	
	theErr = HIViewRender(*this);
	NN_ASSERT_NOERR(theErr);



	// Block as required
	if (!NMathUtilities::IsZero(waitFor))
		{
		theErr = HIWindowFlush(GetHIWindow());
		NN_ASSERT_NOERR(theErr);

		usleep((useconds_t) (waitFor / kEventDurationMicrosecond));
		}
}





//============================================================================
//      NHIView::SimulateClick : Simulate a mouse click.
//----------------------------------------------------------------------------
HIViewPartCode NHIView::SimulateClick(HIViewPartCode partCode, UInt32 modKeys)
{	HIViewPartCode	hitPart;
	OSStatus		theErr;



	// Interpret meta-parts
	//
	// Although a generic "clickable part" code exists, HIViewSimulateClick does
	// not use this to find an appropriate part code for simulating clicks on a
	// view (rdar://4836048).
	//
	// To allow SimulateClick to be called without having to explicitly specify
	// a part code, we use this meta-part as our default value for partCode and
	// try and map it onto something that makes sense for this view.
	if (partCode == kHIViewClickableMetaPart)
		partCode = NHIViewUtilities::GetClickablePart(this);



	// Simulate the click
	hitPart = kHIViewNoPart;
	theErr  = HIViewSimulateClick(*this, partCode, modKeys, &hitPart);
	NN_ASSERT_NOERR(theErr);
	
	return(hitPart);
}





//============================================================================
//		NHIView::HasProperty : Does a property exist?
//----------------------------------------------------------------------------
bool NHIView::HasProperty(OSType theCreator, OSType theTag) const
{	UInt32		theSize;
	OSStatus	theErr;



	// Check the size
	theSize = 0;
	theErr  = GetControlPropertySize(*this, theCreator, theTag, &theSize);
	
	return(theErr == noErr && theSize != 0);
}





//============================================================================
//		NHIView::GetPropertyBoolean : Get a boolean property.
//----------------------------------------------------------------------------
bool NHIView::GetPropertyBoolean(OSType theCreator, OSType theTag) const
{	Boolean		theValue;
	OSStatus	theErr;



	// Get the property
	theErr = GetControlProperty(*this, theCreator, theTag, sizeof(theValue), NULL, &theValue);
	if (theErr != noErr)
		theValue = false;
	
	return((bool) theValue);
}





//============================================================================
//		NHIView::GetPropertyUInt16 : Get a UInt16 property.
//----------------------------------------------------------------------------
UInt16 NHIView::GetPropertyUInt16(OSType theCreator, OSType theTag) const
{	UInt16		theValue;
	OSStatus	theErr;



	// Get the property
	theErr = GetControlProperty(*this, theCreator, theTag, sizeof(theValue), NULL, &theValue);
	if (theErr != noErr)
		theValue = 0;
	
	return(theValue);
}





//============================================================================
//		NHIView::GetPropertyUInt32 : Get a UInt32 property.
//----------------------------------------------------------------------------
UInt32 NHIView::GetPropertyUInt32(OSType theCreator, OSType theTag) const
{	UInt32		theValue;
	OSStatus	theErr;



	// Get the property
	theErr = GetControlProperty(*this, theCreator, theTag, sizeof(theValue), NULL, &theValue);
	if (theErr != noErr)
		theValue = 0;
	
	return(theValue);
}





//============================================================================
//		NHIView::GetPropertyUInt64 : Get a UInt64 property.
//----------------------------------------------------------------------------
UInt64 NHIView::GetPropertyUInt64(OSType theCreator, OSType theTag) const
{	UInt64		theValue;
	OSStatus	theErr;



	// Get the property
	theErr = GetControlProperty(*this, theCreator, theTag, sizeof(theValue), NULL, &theValue);
	if (theErr != noErr)
		theValue = 0;
	
	return(theValue);
}





//============================================================================
//		NHIView::GetPropertyFloat32 : Get a Float32 property.
//----------------------------------------------------------------------------
Float32 NHIView::GetPropertyFloat32(OSType theCreator, OSType theTag) const
{	Float32		theValue;
	OSStatus	theErr;



	// Get the property
	theErr = GetControlProperty(*this, theCreator, theTag, sizeof(theValue), NULL, &theValue);
	if (theErr != noErr)
		theValue = 0;
	
	return(theValue);
}





//============================================================================
//		NHIView::GetPropertyFloat64 : Get a Float64 property.
//----------------------------------------------------------------------------
Float64 NHIView::GetPropertyFloat64(OSType theCreator, OSType theTag) const
{	Float64		theValue;
	OSStatus	theErr;



	// Get the property
	theErr = GetControlProperty(*this, theCreator, theTag, sizeof(theValue), NULL, &theValue);
	if (theErr != noErr)
		theValue = 0;
	
	return(theValue);
}





//============================================================================
//		NHIView::GetPropertyString : Get a string property.
//----------------------------------------------------------------------------
NString NHIView::GetPropertyString(OSType theCreator, OSType theTag) const
{	NString					theValue;
	std::vector<UInt8>		theData;
	UInt32					theSize;
	OSStatus				theErr;



	// Get the property size
	theErr = GetControlPropertySize(*this, theCreator, theTag, &theSize);
	if (theErr != noErr || theSize == 0)
		return(theValue);
	
	theData.resize(theSize);



	// Get the property
	//
	// Property strings are simply an array of UniChars.
	theErr = GetControlProperty(*this, theCreator, theTag, theSize, NULL, &theData[0]);
	if (theErr == noErr)
		theValue = NString((UniChar *) &theData[0], theSize / sizeof(UniChar));
	
	return(theValue);
}





//============================================================================
//		NHIView::GetPropertyOSType : Get a OSType property.
//----------------------------------------------------------------------------
OSType NHIView::GetPropertyOSType(OSType theCreator, OSType theTag) const
{	OSType		theValue;
	OSStatus	theErr;



	// Get the property
	theErr = GetControlProperty(*this, theCreator, theTag, sizeof(theValue), NULL, &theValue);
	if (theErr != noErr)
		theValue = 0;
	
	return(theValue);
}





//============================================================================
//		NHIView::GetPropertyPointer : Get a pointer property.
//----------------------------------------------------------------------------
void *NHIView::GetPropertyPointer(OSType theCreator, OSType theTag) const
{	void		*theValue;
	OSStatus	theErr;



	// Get the property
	theErr = GetControlProperty(*this, theCreator, theTag, sizeof(theValue), NULL, &theValue);
	if (theErr != noErr)
		theValue = NULL;
	
	return(theValue);
}





//============================================================================
//		NHIView::SetPropertyBoolean : Set a boolean property.
//----------------------------------------------------------------------------
void NHIView::SetPropertyBoolean(OSType theCreator, OSType theTag, bool theValue)
{	Boolean		tmpValue;
	OSStatus	theErr;



	// Set the property
	tmpValue = (Boolean) theValue;
	theErr   = SetControlProperty(*this, theCreator, theTag, sizeof(tmpValue), &tmpValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::SetPropertyUInt16 : Set a UInt16 property.
//----------------------------------------------------------------------------
void NHIView::SetPropertyUInt16(OSType theCreator, OSType theTag, UInt16 theValue)
{	OSStatus	theErr;



	// Get the property
	theErr = SetControlProperty(*this, theCreator, theTag, sizeof(theValue), &theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::SetPropertyUInt32 : Set a UInt32 property.
//----------------------------------------------------------------------------
void NHIView::SetPropertyUInt32(OSType theCreator, OSType theTag, UInt32 theValue)
{	OSStatus	theErr;



	// Get the property
	theErr = SetControlProperty(*this, theCreator, theTag, sizeof(theValue), &theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::SetPropertyUInt64 : Set a UInt64 property.
//----------------------------------------------------------------------------
void NHIView::SetPropertyUInt64(OSType theCreator, OSType theTag, UInt64 theValue)
{	OSStatus	theErr;



	// Get the property
	theErr = SetControlProperty(*this, theCreator, theTag, sizeof(theValue), &theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::SetPropertyFloat32 : Set a Float32 property.
//----------------------------------------------------------------------------
void NHIView::SetPropertyFloat32(OSType theCreator, OSType theTag, Float32 theValue)
{	OSStatus	theErr;



	// Get the property
	theErr = SetControlProperty(*this, theCreator, theTag, sizeof(theValue), &theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::SetPropertyFloat64 : Set a Float64 property.
//----------------------------------------------------------------------------
void NHIView::SetPropertyFloat64(OSType theCreator, OSType theTag, Float64 theValue)
{	OSStatus	theErr;



	// Get the property
	theErr = SetControlProperty(*this, theCreator, theTag, sizeof(theValue), &theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::SetPropertyString : Set a string property.
//----------------------------------------------------------------------------
void NHIView::SetPropertyString(OSType theCreator, OSType theTag, const NString &theValue)
{	const UniChar	*theData;
	UInt32			theSize;
	OSStatus		theErr;



	// Set the property
	//
	// Property strings are simply an array of UniChars.
	theData = theValue.GetUTF16();
	theSize = theValue.GetSize() * sizeof(UniChar);
	
	theErr = SetControlProperty(*this, theCreator, theTag, theSize, theData);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::SetPropertyOSType : Set a OSType property.
//----------------------------------------------------------------------------
void NHIView::SetPropertyOSType(OSType theCreator, OSType theTag, OSType theValue)
{	OSStatus	theErr;



	// Get the property
	theErr = SetControlProperty(*this, theCreator, theTag, sizeof(theValue), &theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::SetPropertyPointer : Set a pointer property.
//----------------------------------------------------------------------------
void NHIView::SetPropertyPointer(OSType theCreator, OSType theTag, void *theValue)
{	OSStatus	theErr;



	// Set the property
	theErr = SetControlProperty(*this, theCreator, theTag, sizeof(theValue), &theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::GetControlData : Get some control data.
//----------------------------------------------------------------------------
void NHIView::GetControlData(ControlPartCode thePart, OSType theTag, UInt32 theSize, void *theData) const
{	OSStatus	theErr;



	// Get the data
	theErr = ::GetControlData(*this, thePart, theTag, theSize, theData, NULL);
	if (theErr != noErr)
		memset(theData, 0x00, theSize);
}





//============================================================================
//		NHIView::GetControlBoolean : Get boolean data.
//----------------------------------------------------------------------------
bool NHIView::GetControlBoolean(ControlPartCode thePart, OSType theTag) const
{	Boolean		theValue;



	// Get the data
	GetControlData(thePart, theTag, sizeof(theValue), &theValue);
	
	return((bool) theValue);
}





//============================================================================
//		NHIView::GetControlUInt16 : Get UInt16 data.
//----------------------------------------------------------------------------
UInt16 NHIView::GetControlUInt16(ControlPartCode thePart, OSType theTag) const
{	UInt16		theValue;



	// Get the data
	GetControlData(thePart, theTag, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NHIView::GetControlUInt32 : Get UInt32 data.
//----------------------------------------------------------------------------
UInt32 NHIView::GetControlUInt32(ControlPartCode thePart, OSType theTag) const
{	UInt32		theValue;



	// Get the data
	GetControlData(thePart, theTag, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NHIView::GetControlUInt64 : Get UInt64 data.
//----------------------------------------------------------------------------
UInt64 NHIView::GetControlUInt64(ControlPartCode thePart, OSType theTag) const
{	UInt64		theValue;



	// Get the data
	GetControlData(thePart, theTag, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NHIView::GetControlFloat32 : Get Float32 data.
//----------------------------------------------------------------------------
Float32 NHIView::GetControlFloat32(ControlPartCode thePart, OSType theTag) const
{	Float32		theValue;



	// Get the data
	GetControlData(thePart, theTag, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NHIView::GetControlFloat64 : Get Float64 data.
//----------------------------------------------------------------------------
Float64 NHIView::GetControlFloat64(ControlPartCode thePart, OSType theTag) const
{	Float64		theValue;



	// Get the data
	GetControlData(thePart, theTag, sizeof(theValue), &theValue);
	
	return(theValue);
}





//============================================================================
//		NHIView::GetControlString : Get string data.
//----------------------------------------------------------------------------
NString NHIView::GetControlString(ControlPartCode thePart, OSType theTag) const
{	CFStringRef		cfString;
	NString			theValue;



	// Get the data
	//
	// GetControlData does not follow the CF model of Get/Copy/Create, and
	// we are assumed to take ownership of the returned string. 
	GetControlData(thePart, theTag, sizeof(cfString), &cfString);
	if (cfString != NULL)
		theValue.Set(cfString);
	
	return(theValue);
}





//============================================================================
//		NHIView::SetControlData : Set control data.
//----------------------------------------------------------------------------
void NHIView::SetControlData(ControlPartCode thePart, OSType theTag, UInt32 theSize, const void *theData)
{	OSStatus	theErr;



	// Set the data
	theErr = ::SetControlData(*this, thePart, theTag, theSize, theData);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::SetControlBoolean : Set boolean data.
//----------------------------------------------------------------------------
void NHIView::SetControlBoolean(ControlPartCode thePart, OSType theTag, bool theValue)
{	Boolean		tmpValue;



	// Set the data
	tmpValue = theValue;
	SetControlData(thePart, theTag, sizeof(tmpValue), &tmpValue);
}





//============================================================================
//		NHIView::SetControlUInt16 : Set UInt16 data.
//----------------------------------------------------------------------------
void NHIView::SetControlUInt16(ControlPartCode thePart, OSType theTag, UInt16 theValue)
{


	// Set the data
	SetControlData(thePart, theTag, sizeof(theValue), &theValue);
}





//============================================================================
//		NHIView::SetControlUInt32 : Set UInt32 data.
//----------------------------------------------------------------------------
void NHIView::SetControlUInt32(ControlPartCode thePart, OSType theTag, UInt32 theValue)
{


	// Set the data
	SetControlData(thePart, theTag, sizeof(theValue), &theValue);
}





//============================================================================
//		NHIView::SetControlUInt64 : Set UInt64 data.
//----------------------------------------------------------------------------
void NHIView::SetControlUInt64(ControlPartCode thePart, OSType theTag, UInt64 theValue)
{


	// Set the data
	SetControlData(thePart, theTag, sizeof(theValue), &theValue);
}





//============================================================================
//		NHIView::SetControlFloat32 : Set Float32 data.
//----------------------------------------------------------------------------
void NHIView::SetControlFloat32(ControlPartCode thePart, OSType theTag, Float32 theValue)
{


	// Set the data
	SetControlData(thePart, theTag, sizeof(theValue), &theValue);
}





//============================================================================
//		NHIView::SetControlFloat64 : Set Float64 data.
//----------------------------------------------------------------------------
void NHIView::SetControlFloat64(ControlPartCode thePart, OSType theTag, Float64 theValue)
{


	// Set the data
	SetControlData(thePart, theTag, sizeof(theValue), &theValue);
}





//============================================================================
//		NHIView::SetControlString : Set string data.
//----------------------------------------------------------------------------
void NHIView::SetControlString(ControlPartCode thePart, OSType theTag, const NString &theValue)
{	CFStringRef		cfString;



	// Set the data
	cfString = theValue;
	SetControlData(thePart, theTag, sizeof(cfString), &cfString);
}





//============================================================================
//		NHIView::GetContentIcon : Get the content icon.
//----------------------------------------------------------------------------
NIcon NHIView::GetContentIcon(ControlPartCode thePart, OSType theTag) const
{	ControlButtonContentInfo	theContent;
	NIcon						theIcon;



	// Get the icon
	GetControlData(thePart, theTag, sizeof(theContent), &theContent);
	
	if (theContent.contentType == kControlContentIconRef);
		theIcon = theContent.u.iconRef;
	
	return(theIcon);
}





//============================================================================
//		NHIView::GetContentImage : Get the content image.
//----------------------------------------------------------------------------
NCGImage NHIView::GetContentImage(ControlPartCode thePart, OSType theTag) const
{	ControlButtonContentInfo	theContent;
	NCGImage					theImage;



	// Get the image
	GetControlData(thePart, theTag, sizeof(theContent), &theContent);
	
	if (theContent.contentType == kControlContentCGImageRef);
		theImage = theContent.u.imageRef;
	
	return(theImage);
}





//============================================================================
//		NHIView::SetContentIcon : Set the content icon.
//----------------------------------------------------------------------------
void NHIView::SetContentIcon(ControlPartCode thePart, OSType theTag, const NIcon &theIcon)
{	ControlButtonContentInfo	theContent;



	// Validate our parameters
	NN_ASSERT(theIcon.IsValid());



	// Set the icon
	theContent.contentType = kControlContentIconRef;
	theContent.u.iconRef   = theIcon;

	SetControlData(thePart, theTag, sizeof(theContent), &theContent);
}





//============================================================================
//		NHIView::SetContentImage : Set the content image.
//----------------------------------------------------------------------------
void NHIView::SetContentImage(ControlPartCode thePart, OSType theTag, const NCGImage &theImage)
{	ControlButtonContentInfo	theContent;



	// Validate our parameters
	NN_ASSERT(theImage.IsValid());



	// Set the image
	theContent.contentType = kControlContentCGImageRef;
	theContent.u.imageRef  = theImage;

	SetControlData(thePart, theTag, sizeof(theContent), &theContent);
}





//============================================================================
//		NHIView::GetViewImage : Get the view image.
//----------------------------------------------------------------------------
NCGImage NHIView::GetViewImage(void) const
{	NCGImage		theImage;
	CGImageRef		cgImage;
	OSStatus		theErr;



	// Create the image
	theErr = HIViewCreateOffscreenImage(*this, 0, NULL, &cgImage);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		theImage = cgImage;
	
	CFSafeRelease(cgImage);

	return(theImage);
}





//============================================================================
//		NHIView::GetDraggedTypes : Get the dragged types we accept.
//----------------------------------------------------------------------------
NArray NHIView::GetDraggedTypes(void) const
{


	// Get our state
	return(mDraggedTypes);
}





//============================================================================
//		NHIView::SetDraggedTypes : Set the dragged types we accept.
//----------------------------------------------------------------------------
void NHIView::SetDraggedTypes(const NArray &theTypes)
{	OSStatus	theErr;



	// Set our state
	mDraggedTypes = theTypes;



	// Enable drag and drop support
	theErr = SetControlDragTrackingEnabled(*this, true);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIView::operator HIViewRef : HIViewRef-cast operator.
//----------------------------------------------------------------------------
NHIView::operator HIViewRef(void) const
{


	// Get the view
	return((HIViewRef) GetHIObject());
}





//============================================================================
//		NHIView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
//		Note : Sub-classes should override to initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NHIView::InitializeView(void)
{
}





//============================================================================
//		NHIView::DoEventHIObjectInitialize : Handle kEventHIObjectInitialize.
//----------------------------------------------------------------------------
//		Note : This event is only received by sub-classes of HIView.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventHIObjectInitialize(NCarbonEvent &theEvent)
{	const EventTypeSpec	theEvents[] = { { kEventClassNControl, kEventNControlInitialized },
										{ kEventClassNone,     kEventKindNone            } };
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(GetHIObject() != NULL);



	// Initialize the object
	theErr = CallNextHandler(theEvent);
	if (theErr != noErr)
		return(theErr);



	// Install the handler
	//
	// This event is received as part of the initial set of events requested
	// by NHIObjectFactory when the subclass was registered.
	//
	// Those events are attached to the class, not this object, and so we need
	// to install a handler on our object in order to receive further events.
	theErr = InstallHandler(GetHIObject(), theEvents);
	NN_ASSERT_NOERR(theErr);
	
	return(noErr);
}





//============================================================================
//		NHIView::DoEventControlActivate : Handle kEventControlActivate.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlActivate(NCarbonEvent &/*theEvent*/)
{


	// Update our state
	if (mOptions & kHIViewOptionInvalidateOnActivate)
		SetNeedsDisplay();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIView::DoEventControlDeactivate : Handle kEventControlDeactivate.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlDeactivate(NCarbonEvent &/*theEvent*/)
{


	// Update our state
	if (mOptions & kHIViewOptionInvalidateOnActivate)
		SetNeedsDisplay();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIView::DoEventControlDragEnter : Handle kEventControlDragEnter.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlDragEnter(NCarbonEvent &theEvent)
{	NPasteboard		thePasteboard(theEvent);
	UInt32			n, numFlavors;
	bool			canAccept;



	// Check our state
	//
	// Disabled controls, or those with no types, can't receive drags.
	numFlavors = mDraggedTypes.GetSize();
	canAccept  = IsEnabled() && numFlavors != 0;



	// Look for an acceptable type
	if (canAccept)
		{
		canAccept = false;

		for (n = 0; n < numFlavors && !canAccept; n++)
			canAccept = thePasteboard.HasFlavor(mDraggedTypes.GetValueString(n));
		}



	// Update the event
	theEvent.SetParameterBoolean(kEventParamControlWouldAcceptDrop, canAccept);
	
	return(noErr);
}





//============================================================================
//		NHIView::DoEventControlDragWithin : Handle kEventControlDragWithin.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlDragWithin(NCarbonEvent &theEvent)
{	NCursor		theCursor;
	DragRef		theDrag;



	// Update the drag
	theDrag = theEvent.GetParameterDrag(kEventParamDragRef);
	theCursor.SetCursor(DoDragUpdate(theDrag, true));
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIView::DoEventControlDragLeave : Handle kEventControlDragLeave.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlDragLeave(NCarbonEvent &theEvent)
{	NCursor		theCursor;
	DragRef		theDrag;



	// Update the drag
	theDrag = theEvent.GetParameterDrag(kEventParamDragRef);
	(void) DoDragUpdate(theDrag, false);



	// Reset the cursor
	theCursor.SetCursor(kThemeArrowCursor);
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIView::DoEventControlDragReceive : Handle kEventControlDragReceive.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlDragReceive(NCarbonEvent &theEvent)
{	DragActions		theAction;
	DragRef			theDrag;
	OSStatus		theErr;



	// Receive the drag
	theDrag   = theEvent.GetParameterDrag(kEventParamDragRef);
	theAction = DoDragReceive(theDrag);



	// Update the source of the drag
	theErr = SetDragDropAction(theDrag, theAction);
	NN_ASSERT_NOERR(theErr);

	return(theErr);
}





//============================================================================
//		NHIView::DoEventControlEnabledStateChanged : Handle kEventControlEnabledStateChanged.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlEnabledStateChanged(NCarbonEvent &/*theEvent*/)
{


	// Update our state
	if (mOptions & kHIViewOptionInvalidateOnEnable)
		SetNeedsDisplay();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIView::DoEventControlGetSizeConstraints : Handle kEventControlGetSizeConstraints.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlGetSizeConstraints(NCarbonEvent &theEvent)
{	HISize		sizeMin, sizeMax, sizeIdeal;
	OSStatus	theErr;



	// Ask the base class
	//
	// We ask the base class first, so that wrappers around system classes can
	// inherit the size constraints of their base class.
	theErr = CallNextHandler(theEvent);
	NN_ASSERT(theErr == noErr || theErr == eventNotHandledErr);

	sizeMin = theEvent.GetParameterHISize(kEventParamMinimumSize);
	sizeMax = theEvent.GetParameterHISize(kEventParamMaximumSize);



	// Override the default size
	//
	// If a size has been assigned to this view, it overrides the sizes from
	// the base class (if any - the base HIView class has no size constraints).
	if (mSizeMin != kHISizeZero)
		sizeMin = mSizeMin;

	if (mSizeMax != kHISizeZero)
		sizeMax = mSizeMax;



	// Provide a fallback
	//
	// If no size has been assigned to this view, and the base class doesn't
	// provide one ither, we fall back to the "ideal size" for the view.
	//
	// if the view doesn't have an ideal size, this equates to the bounds of
	// the view (which ensures that every view has a size constraint, which
	// is a requirement for embedding within a toolbar).
	if (sizeMin == kHISizeZero || sizeMax == kHISizeZero)
		{
		sizeIdeal = GetIdealSize().size;
		
		if (NMathUtilities::IsZero(sizeMin.width) && NMathUtilities::IsZero(sizeMin.height))
			sizeMin = sizeIdeal;

		if (NMathUtilities::IsZero(sizeMax.width) && NMathUtilities::IsZero(sizeMax.height))
			sizeMax = sizeIdeal;
		}



	// Update the event
	theErr  = theEvent.SetParameterHISize(kEventParamMinimumSize, sizeMin);
	theErr |= theEvent.SetParameterHISize(kEventParamMaximumSize, sizeMax);
	
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NHIView::DoEventControlHiliteChanged : Handle kEventControlHiliteChanged.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlHiliteChanged(NCarbonEvent &/*theEvent*/)
{


	// Update our state
	if (mOptions & kHIViewOptionInvalidateOnHilite)
		SetNeedsDisplay();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIView::DoEventControlHitTest : Handle kEventControlHitTest.
//----------------------------------------------------------------------------
//		Note :	The base class does not register for this event, but supplies
//				a default implementation for views that wish to do so.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlHitTest(NCarbonEvent &theEvent)
{


	// Update the event
	//
	// The base class assumes the view contains a single part.
	theEvent.SetViewPartCode(kControlIndicatorPart);
	
	return(noErr);
}





//============================================================================
//		NHIView::DoEventControlTitleChanged : Handle kEventControlTitleChanged.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlTitleChanged(NCarbonEvent &/*theEvent*/)
{


	// Update our state
	if (mOptions & kHIViewOptionInvalidateOnTitle)
		SetNeedsDisplay();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIView::DoEventControlTrack : Handle kEventControlTrack.
//----------------------------------------------------------------------------
//		Note :	The base class does not register for this event, but supplies
//				a default implementation for views that wish to do so.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlTrack(NCarbonEvent &theEvent)
{	NCarbonEvent			hitTest(kEventClassControl, kEventControlHitTest);
	HIPoint					theMouse, lastMouse;
	UInt32					keyModifiers;
	MouseTrackingResult		trackResult;
	HIPoint					theDelta;
	Point					qdPoint;
	OSStatus				theErr;



	// Get the state we need
	theMouse  = theEvent.GetMouseLocation();
	lastMouse = theMouse;

	theErr      = noErr;
	trackResult = kMouseTrackingMouseDown;



	// Track the mouse
	//
	// The base class provides a default mouse tracking loop - sub-classes
	// should implement DoTrackMouse to update their state during the track.
	SetTrackingMouse(true);
	
	while (DoTrackMouse(theEvent, theMouse, trackResult))
		{
		// Block until the mouse moves
		theErr = TrackMouseLocationWithOptions((GrafPtr) -1, 0, kEventDurationForever, &qdPoint, &keyModifiers, &trackResult);
		NN_ASSERT_NOERR(theErr);

		theMouse = CGPointMake(qdPoint.h, qdPoint.v);
		theMouse = NGeometryUtilities::Transform(theMouse, kHICoordSpace72DPIGlobal, NULL, kHICoordSpaceView, *this);



		// Update the event
		theDelta.x = theMouse.x - lastMouse.x;
		theDelta.y = theMouse.y - lastMouse.y;
		lastMouse  = theMouse;
		
		theEvent.SetMouseDelta(  theDelta);
		theEvent.SetKeyModifiers(keyModifiers);
		}
	
	SetTrackingMouse(false);



	// Return the part
	//
	// If the user didn't cancel, we hit-test the final mouse location and return
	// the hit part (if any) so that the Control Manager will dispatch our command.
	if (trackResult == kMouseTrackingMouseUp && CGRectContainsPoint(GetBounds(), theMouse))
		{
		hitTest.SetHIView(*this);
		hitTest.SetMouseLocation(theMouse);
		
		theErr = DispatchEvent(hitTest);
		if (theErr == noErr)
			theEvent.SetViewPartCode(hitTest.GetViewPartCode());
		}

	return(theErr);
}





//============================================================================
//		NHIView::DoEventControlValueFieldChanged : Handle kEventControlValueFieldChanged.
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventControlValueFieldChanged(NCarbonEvent &/*theEvent*/)
{


	// Update our state
	if (mOptions & kHIViewOptionInvalidateOnValue)
		SetNeedsDisplay();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIView::DoEventNControlInitialized : Handle kEventNControlInitialized.
//----------------------------------------------------------------------------
//		Note :	This event is only received by sub-classes of HIView when they
//				are instantiated via a window created with NIBUtilities .
//----------------------------------------------------------------------------
OSStatus NHIView::DoEventNControlInitialized(NCarbonEvent &/*theEvent*/)
{


	// Initialize the view
	//
	// Unfortunately we can not initialize the view from kEventHIObjectInitialize
	// or kEventControlInitialize, since those events are sent before the view
	// properties are applied.
	//
	// Ideally there would be an event dispatched once the IB runtime has applied
	// the view properties (rdar://4499560), but to work around this our window
	// creation bottleneck (NIBUtilities::CreateWindow) dispatches this event to
	// every view in the window once the window has been created.
	//
	// At that time the properties have been applied, so we can initialize the view.
	InitializeSubView();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIView::DoDragUpdate : Update a drag.
//----------------------------------------------------------------------------
ThemeCursor NHIView::DoDragUpdate(DragRef /*theDrag*/, bool isWithin)
{


	// Update our state
	//
	// Sub-classes with transparent areas may wish to override this method to
	// perform their own hit-testing during a drag, since the default behaviour
	// is to treat the entire view as a drag target.
	//
	// Sub-classes which wish to change the cursor during a drag should override
	// and return an appropriate cursor type.
	SetDragTarget(isWithin);
	
	return(kThemeCopyArrowCursor);
}





//============================================================================
//		NHIView::DoDragReceive : Receive a drag.
//----------------------------------------------------------------------------
DragActions NHIView::DoDragReceive(DragRef /*theDrag*/)
{


	// Receive the drag
	//
	// Sub-classes should override to receive a drag, and return the action
	// with which the drag data was handled.
	NN_LOG("NHIView::DoDragReceive should be overriden");

	return(kDragActionNothing);
}





//============================================================================
//		NHIView::DoTrackMouse : Track the mouse.
//----------------------------------------------------------------------------
bool NHIView::DoTrackMouse(NCarbonEvent &/*theEvent*/, const HIPoint &/*theMouse*/, MouseTrackingResult theTrack)
{	bool	keepTracking;



	// Track the mouse
	//
	// Sub-classes should override this method to implement mouse tracking,
	// since it will be invoked by the base class if the view registers for
	// kEventControlTrack events.
	//
	// We provide a default implementation that ends the track loop when the
	// user releases the mouse or cancels, and sub-classes would typically
	// call through to pick up the default behaviour.
	keepTracking = true;
	
	switch (theTrack) {
		case kMouseTrackingMouseUp:
		case kMouseTrackingUserCancelled:
			// Stop the track
			keepTracking = false;
			break;
		}
	
	return(keepTracking);
}





//============================================================================
//		NHIView::ConstructView : Construct the view.
//----------------------------------------------------------------------------
void NHIView::ConstructView(HIViewRef hiView, HIWindowRef hiWindow, const HIRect *theFrame, bool isVisible)
{	HIViewRef		contentView;
	OSStatus		theErr;



	// Validate our parameters
	NN_ASSERT(hiView != NULL);



	// Initialize the view
	//
	// We are invoked from the constructor of a sub-class, when they have created an
	// HIViewRef and wish to assign it to themselves.
	//
	// If the view was a sub-class instantiated with HIObjectCreate, we will already
	// be associated with the HIObjectRef and we just need to invoke InitializeSubView
	// (since this is normally invoked via kEventNControlInitialized when a window is
	// created).
	//
	// If the view is a wrapper around a system HIViewRef, we need to assign the view
	// to ourselves (which will invoke InitializeSubView for us).
	if (GetHIObject() == NULL)
		SetHIObject((HIObjectRef) hiView);
	else
		InitializeSubView();



	// Add the view to the window
	if (hiWindow != NULL)
		{
		contentView = NHIWindow(hiWindow).GetHIView(kHIViewWindowContentID);
		NN_ASSERT(contentView != NULL);
		
		theErr = HIViewAddSubview(contentView, *this);
		NN_ASSERT_NOERR(theErr);
		}



	// Set the frame
	if (theFrame != NULL)
		SetFrame(*theFrame);



	// Set the visibility
	SetVisible(isVisible);
}





//============================================================================
//		NHIView::SetHIObject : Set the HIObjectRef.
//----------------------------------------------------------------------------
//		Note : This method is only invoked for wrapper classes.
//----------------------------------------------------------------------------
void NHIView::SetHIObject(HIObjectRef hiObject)
{


	// Validate our parameters and state
	NN_ASSERT(hiObject      != NULL);
	NN_ASSERT(GetHIObject() == NULL);



	// Initialize the object
	NHIObject::SetHIObject(hiObject);



	// Initialize the view
	InitializeSubView();
}





//============================================================================
//		NHIView::IsDragTarget : Are we the target for a drag?
//----------------------------------------------------------------------------
bool NHIView::IsDragTarget(void) const
{


	// Get our state
	return(mIsDragTarget);
}





//============================================================================
//		NHIView::SetDragTarget : Set if we are the target for a drag.
//----------------------------------------------------------------------------
void NHIView::SetDragTarget(bool isTarget)
{	bool	wasTarget;



	// Set our state
	wasTarget     = mIsDragTarget;
	mIsDragTarget = isTarget;



	// Redraw when our state changes
	if (wasTarget != mIsDragTarget)
		SetNeedsDisplay();
}





//============================================================================
//		NHIView::IsTrackingMouse : Are we tracking the mouse?
//----------------------------------------------------------------------------
bool NHIView::IsTrackingMouse(void) const
{


	// Get our state
	return(mIsTrackingMouse);
}





//============================================================================
//		NHIView::SetTrackingMouse : Set if we are tracking the mouse.
//----------------------------------------------------------------------------
void NHIView::SetTrackingMouse(bool isTracking)
{


	// Update our state
	mIsTrackingMouse = isTracking;
}





//============================================================================
//		NHIView::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NHIView::InitializeSelf(void)
{	const EventTypeSpec	theEvents[] = {	{ kEventClassControl, kEventControlActivate				},
										{ kEventClassControl, kEventControlDeactivate			},
										{ kEventClassControl, kEventControlDragEnter			},
										{ kEventClassControl, kEventControlDragLeave			},
										{ kEventClassControl, kEventControlDragReceive			},
										{ kEventClassControl, kEventControlDragWithin			},
										{ kEventClassControl, kEventControlEnabledStateChanged	},
										{ kEventClassControl, kEventControlGetSizeConstraints	},
										{ kEventClassControl, kEventControlHiliteChanged		},
										{ kEventClassControl, kEventControlTitleChanged			},
										{ kEventClassControl, kEventControlValueFieldChanged	},
										{ kEventClassNone,    kEventKindNone					} };



	// Initialize ourselves
	mSizeMin = kHISizeZero;
	mSizeMax = kHISizeZero;
	mOptions = kHIViewOptionsNone;

	mIsDragTarget    = false;
	mIsTrackingMouse = false;



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NHIView::InitializeSubView : Initialize the sub-class view.
//----------------------------------------------------------------------------
void NHIView::InitializeSubView(void)
{


	// Apply our properties
	if (HasProperty(                               kPropertyNano, kTagValueMinimum))
		SetMinimumValue((SInt32) GetPropertyUInt32(kPropertyNano, kTagValueMinimum));

	if (HasProperty(                               kPropertyNano, kTagValueMaximum))
		SetMaximumValue((SInt32) GetPropertyUInt32(kPropertyNano, kTagValueMaximum));

	if (HasProperty(                               kPropertyNano, kTagValueCurrent))
		SetCurrentValue((SInt32) GetPropertyUInt32(kPropertyNano, kTagValueCurrent));

	if (HasProperty(                   kPropertyNano, kTagValueText))
		SetTextValue(GetPropertyString(kPropertyNano, kTagValueText));



	// Enable auto-invalidation
	//
	// System views automatically invalidate when their state is changed,
	// but custom views must perform this step manually (rdar://4771894).
	//
	// We maintain a set of option flags that can provide this behaviour,
	// and turn them on by default for custom views.
	if (!GetClassID().StartsWith("com.apple", kCFCompareCaseInsensitive))
		{
		mOptions |= kHIViewOptionInvalidateOnActivate;
		mOptions |= kHIViewOptionInvalidateOnHilite;
		mOptions |= kHIViewOptionInvalidateOnEnable;
		mOptions |= kHIViewOptionInvalidateOnValue;
		mOptions |= kHIViewOptionInvalidateOnTitle;
		}



	// Initialize the view
	InitializeView();
}


