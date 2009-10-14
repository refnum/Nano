/*	NAME:
		NHISegmentedView.cpp

	DESCRIPTION:
		Segmented view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFBundle.h"
#include "NMathUtilities.h"
#include "NHISegmentedView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagInitialSegment								= 'segm';
static const OSType kTagSegment0Image								= 'img0';
static const OSType kTagSegment1Image								= 'img1';
static const OSType kTagSegment2Image								= 'img2';
static const OSType kTagSegment3Image								= 'img3';
static const OSType kTagSegment4Image								= 'img4';
static const OSType kTagSegment5Image								= 'img5';
static const OSType kTagSegment6Image								= 'img6';
static const OSType kTagSegment7Image								= 'img7';
static const OSType kTagSegment8Image								= 'img8';
static const OSType kTagSegment9Image								= 'img9';
static const OSType kTagSegment0Label								= 'txt0';
static const OSType kTagSegment1Label								= 'txt1';
static const OSType kTagSegment2Label								= 'txt2';
static const OSType kTagSegment3Label								= 'txt3';
static const OSType kTagSegment4Label								= 'txt4';
static const OSType kTagSegment5Label								= 'txt5';
static const OSType kTagSegment6Label								= 'txt6';
static const OSType kTagSegment7Label								= 'txt7';
static const OSType kTagSegment8Label								= 'txt8';
static const OSType kTagSegment9Label								= 'txt9';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NHISegmentedView, kHISegmentedViewClassID, NULL);





//============================================================================
//		NHISegmentedView::NHISegmentedView : Constructor.
//----------------------------------------------------------------------------
NHISegmentedView::NHISegmentedView(HIWindowRef theWindow, const HIRect &theRect, UInt32 numItems)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = HISegmentedViewCreate(NULL, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);



	// Configure the view
	SetSegmentCount(numItems);
}





//============================================================================
//		NHISegmentedView::~NHISegmentedView : Destructor.
//----------------------------------------------------------------------------
NHISegmentedView::~NHISegmentedView(void)
{
}





//============================================================================
//		NHISegmentedView::GetSegmentCount : Get the number of segments.
//----------------------------------------------------------------------------
UInt32 NHISegmentedView::GetSegmentCount(void) const
{


	// Get the number of segments
	return(HISegmentedViewGetSegmentCount(*this));
}





//============================================================================
//		NHISegmentedView::SetSegmentCount : Set the number of segments.
//----------------------------------------------------------------------------
void NHISegmentedView::SetSegmentCount(UInt32 theValue)
{	OSStatus	theErr;



	// Set the number of segments
	theErr = HISegmentedViewSetSegmentCount(*this, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHISegmentedView::IsSegmentEnabled : Is a segment enabled?
//----------------------------------------------------------------------------
bool NHISegmentedView::IsSegmentEnabled(UInt32 theIndex) const
{


	// Get the segment state
	return(HISegmentedViewIsSegmentEnabled(*this, theIndex+1));
}





//============================================================================
//		NHISegmentedView::SetSegmentEnabled : Set a segment's enabled state.
//----------------------------------------------------------------------------
void NHISegmentedView::SetSegmentEnabled(UInt32 theIndex, bool isEnabled)
{	UInt32		n, numItems;
	OSStatus	theErr;



	// Update all segments
	if (theIndex == kAllSegments)
		{
		numItems = GetSegmentCount();
		for (n = 0; n < numItems; n++)
			SetSegmentEnabled(n, isEnabled);
		}
	
	
	// Update a single segment
	else
		{
		theErr = HISegmentedViewSetSegmentEnabled(*this, theIndex+1, isEnabled);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NHISegmentedView::GetSegmentBehaviour : Get a segment's behaviour.
//----------------------------------------------------------------------------
HISegmentBehavior NHISegmentedView::GetSegmentBehaviour(UInt32 theIndex) const
{


	// Get the segment state
	return(HISegmentedViewGetSegmentBehavior(*this, theIndex+1));
}





//============================================================================
//		NHISegmentedView::SetSegmentBehaviour : Set a segment's behaviour.
//----------------------------------------------------------------------------
void NHISegmentedView::SetSegmentBehaviour(UInt32 theIndex, HISegmentBehavior theBehaviour)
{	UInt32		n, numItems;
	OSStatus	theErr;



	// Update all segments
	if (theIndex == kAllSegments)
		{
		numItems = GetSegmentCount();
		for (n = 0; n < numItems; n++)
			SetSegmentBehaviour(n, theBehaviour);
		}
	
	
	// Update a single segment
	else
		{
		theErr = HISegmentedViewSetSegmentBehavior(*this, theIndex+1, theBehaviour);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NHISegmentedView::GetSegmentAttributes : Get a segment's attributes.
//----------------------------------------------------------------------------
OptionBits NHISegmentedView::GetSegmentAttributes(UInt32 theIndex) const
{


	// Get the segment state
	return(HISegmentedViewGetSegmentAttributes(*this, theIndex+1));
}





//============================================================================
//		NHISegmentedView::SetSegmentAttributes : Set a segment's attributes.
//----------------------------------------------------------------------------
void NHISegmentedView::SetSegmentAttributes(UInt32 theIndex, OptionBits setThese, OptionBits clearThese)
{	UInt32		n, numItems;
	OSStatus	theErr;



	// Update all segments
	if (theIndex == kAllSegments)
		{
		numItems = GetSegmentCount();
		for (n = 0; n < numItems; n++)
			SetSegmentAttributes(n, setThese, clearThese);
		}
	
	
	// Update a single segment
	else
		{
		theErr = HISegmentedViewChangeSegmentAttributes(*this, theIndex+1, setThese, clearThese);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NHISegmentedView::GetSegmentValue : Get a segment's value.
//----------------------------------------------------------------------------
SInt32 NHISegmentedView::GetSegmentValue(UInt32 theIndex) const
{


	// Get the segment state
	return(HISegmentedViewGetSegmentValue(*this, theIndex+1));
}





//============================================================================
//		NHISegmentedView::SetSegmentValue : Set a segment's value.
//----------------------------------------------------------------------------
void NHISegmentedView::SetSegmentValue(UInt32 theIndex, SInt32 theValue)
{	UInt32		n, numItems;
	OSStatus	theErr;



	// Update all segments
	if (theIndex == kAllSegments)
		{
		numItems = GetSegmentCount();
		for (n = 0; n < numItems; n++)
			SetSegmentValue(n, theValue);
		}
	
	
	// Update a single segment
	else
		{
		theErr = HISegmentedViewSetSegmentValue(*this, theIndex+1, theValue);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NHISegmentedView::GetSegmentCommand : Get a segment's command.
//----------------------------------------------------------------------------
UInt32 NHISegmentedView::GetSegmentCommand(UInt32 theIndex) const
{


	// Get the segment state
	return(HISegmentedViewGetSegmentCommand(*this, theIndex+1));
}





//============================================================================
//		NHISegmentedView::SetSegmentCommand : Set a segment's command.
//----------------------------------------------------------------------------
void NHISegmentedView::SetSegmentCommand(UInt32 theIndex, UInt32 theCmd)
{	UInt32		n, numItems;
	OSStatus	theErr;



	// Update all segments
	if (theIndex == kAllSegments)
		{
		numItems = GetSegmentCount();
		for (n = 0; n < numItems; n++)
			SetSegmentCommand(n, theCmd);
		}
	
	
	// Update a single segment
	else
		{
		theErr = HISegmentedViewSetSegmentCommand(*this, theIndex+1, theCmd);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NHISegmentedView::GetSegmentLabel : Get a segment's label.
//----------------------------------------------------------------------------
NString NHISegmentedView::GetSegmentLabel(UInt32 theIndex) const
{	NString			theValue;
	CFStringRef		cfString;
	OSStatus		theErr;



	// Get the segment state
	cfString = NULL;
	theErr   = HISegmentedViewCopySegmentLabel(*this, theIndex+1, &cfString);
	NN_ASSERT_NOERR(theErr);
	
	if (cfString != NULL)
		theValue.Set(cfString);

	return(theValue);
}





//============================================================================
//		NHISegmentedView::SetSegmentLabel : Set a segment's label.
//----------------------------------------------------------------------------
void NHISegmentedView::SetSegmentLabel(UInt32 theIndex, const NString &theValue)
{	UInt32		n, numItems;
	OSStatus	theErr;



	// Update all segments
	if (theIndex == kAllSegments)
		{
		numItems = GetSegmentCount();
		for (n = 0; n < numItems; n++)
			SetSegmentLabel(n, theValue);
		}
	
	
	// Update a single segment
	else
		{
		theErr = HISegmentedViewSetSegmentLabel(*this, theIndex+1, theValue);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NHISegmentedView::GetSegmentWidth : Get a segment's width.
//----------------------------------------------------------------------------
float NHISegmentedView::GetSegmentWidth(UInt32 theIndex) const
{


	// Get the segment state
	return(HISegmentedViewGetSegmentContentWidth(*this, theIndex+1, NULL));
}





//============================================================================
//		NHISegmentedView::SetSegmentWidth : Set a segment's width.
//----------------------------------------------------------------------------
void NHISegmentedView::SetSegmentWidth(UInt32 theIndex, float theWidth)
{	UInt32		n, numItems;
	OSStatus	theErr;



	// Update all segments
	if (theIndex == kAllSegments)
		{
		numItems = GetSegmentCount();
		for (n = 0; n < numItems; n++)
			SetSegmentWidth(n, theWidth);
		}
	
	
	// Update a single segment
	else
		{
		theErr = HISegmentedViewSetSegmentContentWidth(*this, theIndex+1, NMathUtilities::NotZero(theWidth), theWidth);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NHISegmentedView::GetSegmentIcon : Get a segment's icon.
//----------------------------------------------------------------------------
NIcon NHISegmentedView::GetSegmentIcon(UInt32 theIndex) const
{	HIViewImageContentInfo	theContent;
	NIcon					theIcon;
	OSStatus				theErr;



	// Get the segment state
	theContent = kControlContentNone;
	theErr     = HISegmentedViewCopySegmentImage(*this, theIndex+1, &theContent);
	
	if (theContent.contentType == kControlContentIconRef);
		theIcon = theContent.u.iconRef;
	
	return(theIcon);
}





//============================================================================
//		NHISegmentedView::SetSegmentIcon : Set a segment's icon.
//----------------------------------------------------------------------------
void NHISegmentedView::SetSegmentIcon(UInt32 theIndex, const NIcon &theIcon)
{	UInt32					n, numItems;
	HIViewImageContentInfo	theContent;
	OSStatus				theErr;



	// Update all segments
	if (theIndex == kAllSegments)
		{
		numItems = GetSegmentCount();
		for (n = 0; n < numItems; n++)
			SetSegmentIcon(n, theIcon);
		}
	
	
	// Update a single segment
	else
		{
		theContent.contentType = kControlContentIconRef;
		theContent.u.iconRef   = theIcon;
		
		theErr = HISegmentedViewSetSegmentImage(*this, theIndex+1, &theContent);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NHISegmentedView::GetSegmentImage : Get a segment's image.
//----------------------------------------------------------------------------
NCGImage NHISegmentedView::GetSegmentImage(UInt32 theIndex) const
{	HIViewImageContentInfo	theContent;
	NCGImage				theImage;
	OSStatus				theErr;



	// Get the segment state
	theContent = kControlContentNone;
	theErr     = HISegmentedViewCopySegmentImage(*this, theIndex+1, &theContent);
	
	if (theContent.contentType == kControlContentCGImageRef);
		theImage = theContent.u.imageRef;
	
	return(theImage);
}





//============================================================================
//		NHISegmentedView::SetSegmentImage : Set a segment's image.
//----------------------------------------------------------------------------
void NHISegmentedView::SetSegmentImage(UInt32 theIndex, const NCGImage &theImage)
{	UInt32					n, numItems;
	HIViewImageContentInfo	theContent;
	OSStatus				theErr;



	// Update all segments
	if (theIndex == kAllSegments)
		{
		numItems = GetSegmentCount();
		for (n = 0; n < numItems; n++)
			SetSegmentImage(n, theImage);
		}
	
	
	// Update a single segment
	else
		{
		theContent.contentType = kControlContentCGImageRef;
		theContent.u.imageRef  = theImage;
		
		theErr = HISegmentedViewSetSegmentImage(*this, theIndex+1, &theContent);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NHISegmentedView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NHISegmentedView::InitializeView(void)
{


	// Apply our properties
	if (HasProperty(                      kPropertyNano, kTagInitialSegment))
		SetCurrentValue(GetPropertyUInt32(kPropertyNano, kTagInitialSegment));

	if (HasProperty(                                         kPropertyNano, kTagSegment0Image))
		SetSegmentImage(0, NBundleResource(GetPropertyString(kPropertyNano, kTagSegment0Image)));

	if (HasProperty(                                         kPropertyNano, kTagSegment1Image))
		SetSegmentImage(1, NBundleResource(GetPropertyString(kPropertyNano, kTagSegment1Image)));

	if (HasProperty(                                         kPropertyNano, kTagSegment2Image))
		SetSegmentImage(2, NBundleResource(GetPropertyString(kPropertyNano, kTagSegment2Image)));

	if (HasProperty(                                         kPropertyNano, kTagSegment3Image))
		SetSegmentImage(3, NBundleResource(GetPropertyString(kPropertyNano, kTagSegment3Image)));

	if (HasProperty(                                         kPropertyNano, kTagSegment4Image))
		SetSegmentImage(4, NBundleResource(GetPropertyString(kPropertyNano, kTagSegment4Image)));

	if (HasProperty(                                         kPropertyNano, kTagSegment5Image))
		SetSegmentImage(5, NBundleResource(GetPropertyString(kPropertyNano, kTagSegment5Image)));

	if (HasProperty(                                         kPropertyNano, kTagSegment6Image))
		SetSegmentImage(6, NBundleResource(GetPropertyString(kPropertyNano, kTagSegment6Image)));

	if (HasProperty(                                         kPropertyNano, kTagSegment7Image))
		SetSegmentImage(7, NBundleResource(GetPropertyString(kPropertyNano, kTagSegment7Image)));

	if (HasProperty(                                         kPropertyNano, kTagSegment8Image))
		SetSegmentImage(8, NBundleResource(GetPropertyString(kPropertyNano, kTagSegment8Image)));

	if (HasProperty(                                         kPropertyNano, kTagSegment9Image))
		SetSegmentImage(9, NBundleResource(GetPropertyString(kPropertyNano, kTagSegment9Image)));

	if (HasProperty(                         kPropertyNano, kTagSegment0Image))
		SetSegmentLabel(0, GetPropertyString(kPropertyNano, kTagSegment0Image));

	if (HasProperty(                         kPropertyNano, kTagSegment1Image))
		SetSegmentLabel(1, GetPropertyString(kPropertyNano, kTagSegment1Image));

	if (HasProperty(                         kPropertyNano, kTagSegment2Image))
		SetSegmentLabel(2, GetPropertyString(kPropertyNano, kTagSegment2Image));

	if (HasProperty(                         kPropertyNano, kTagSegment3Image))
		SetSegmentLabel(3, GetPropertyString(kPropertyNano, kTagSegment3Image));

	if (HasProperty(                         kPropertyNano, kTagSegment4Image))
		SetSegmentLabel(4, GetPropertyString(kPropertyNano, kTagSegment4Image));

	if (HasProperty(                         kPropertyNano, kTagSegment5Image))
		SetSegmentLabel(5, GetPropertyString(kPropertyNano, kTagSegment5Image));

	if (HasProperty(                         kPropertyNano, kTagSegment6Image))
		SetSegmentLabel(6, GetPropertyString(kPropertyNano, kTagSegment6Image));

	if (HasProperty(                         kPropertyNano, kTagSegment7Image))
		SetSegmentLabel(7, GetPropertyString(kPropertyNano, kTagSegment7Image));

	if (HasProperty(                         kPropertyNano, kTagSegment8Image))
		SetSegmentLabel(8, GetPropertyString(kPropertyNano, kTagSegment8Image));

	if (HasProperty(                         kPropertyNano, kTagSegment9Image))
		SetSegmentLabel(9, GetPropertyString(kPropertyNano, kTagSegment9Image));
}



