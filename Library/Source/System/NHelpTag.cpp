/*	NAME:
		NHelpTag.cpp

	DESCRIPTION:
		Help tag.
	
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
#include "NHelpUtilities.h"
#include "NMenuItem.h"
#include "NHIWindow.h"
#include "NHIView.h"
#include "NHelpTag.h"





//============================================================================
//		NHelpTag::NHelpTag : Constructor.
//----------------------------------------------------------------------------
NHelpTag::NHelpTag(const NHIView *theItem)
{	HMHelpContentRec	theContent;
	OSStatus			theErr;



	// Initialize ourselves
	InitializeSelf();



	// Get the content
	theErr = HMGetControlHelpContent(*theItem, &theContent);
	NN_ASSERT(theErr == noErr || theErr == menuPropertyNotFoundErr);

	if (theErr == noErr)
		AssignContent(theContent);
}





//============================================================================
//		NHelpTag::NHelpTag : Constructor.
//----------------------------------------------------------------------------
NHelpTag::NHelpTag(const NHIWindow *theItem)
{	HMHelpContentRec	theContent;
	OSStatus			theErr;



	// Initialize ourselves
	InitializeSelf();



	// Get the content
	theErr = HMGetWindowHelpContent(*theItem, &theContent);
	NN_ASSERT(theErr == noErr || theErr == menuPropertyNotFoundErr);

	if (theErr == noErr)
		AssignContent(theContent);
}





//============================================================================
//		NHelpTag::NHelpTag : Constructor.
//----------------------------------------------------------------------------
NHelpTag::NHelpTag(const NMenuItem &theItem)
{	HMHelpContentRec	theContent;
	OSStatus			theErr;



	// Initialize ourselves
	InitializeSelf();



	// Get the content
	theErr = HMGetMenuItemHelpContent(theItem.GetMenu(), theItem.GetIndex(), &theContent);
	NN_ASSERT(theErr == noErr || theErr == menuPropertyNotFoundErr);

	if (theErr == noErr)
		AssignContent(theContent);
}





//============================================================================
//		NHelpTag::NHelpTag : Constructor.
//----------------------------------------------------------------------------
NHelpTag::NHelpTag(const NString &theValue)
{


	// Initialize ourselves
	InitializeSelf();
	
	SetValue(theValue);
}





//============================================================================
//		NHelpTag::NHelpTag : Constructor.
//----------------------------------------------------------------------------
NHelpTag::NHelpTag(const NHelpTag &theTag)
{


	// Initialize ourselves
	InitializeSelf();
	
	AssignTag(theTag);
}





//============================================================================
//		NHelpTag::NHelpTag : Constructor.
//----------------------------------------------------------------------------
NHelpTag::NHelpTag(void)
{


	// Initialize ourselves
	InitializeSelf();
}





//============================================================================
//		NHelpTag::~NHelpTag : Destructor.
//----------------------------------------------------------------------------
NHelpTag::~NHelpTag(void)
{
}





//============================================================================
//		NHelpTag::Show : Show the tag.
//----------------------------------------------------------------------------
void NHelpTag::Show(const HIPoint &thePoint)
{	QDPoint		qdPoint;
	OSStatus	theErr;



	// Get the state we need
	if (thePoint == kHIPointZero)
		GetGlobalMouse(&qdPoint);
	else
		NGeometryUtilities::ConvertPoint(thePoint, qdPoint);



	// Show the tag
	//
	// To show a tag at a point, all four sides of the hot rect must be set.
	mContent.absHotRect.top  = mContent.absHotRect.bottom = qdPoint.v;
	mContent.absHotRect.left = mContent.absHotRect.right  = qdPoint.h;

	theErr = HMDisplayTag(*this);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHelpTag::Hide : Hide the tag.
//----------------------------------------------------------------------------
void NHelpTag::Hide(OptionBits theFlags)
{	OSStatus	theErr;



	// Hide the tag
	//
	// HMHideTag is used to support 10.3.
	if (HMHideTagWithOptions != NULL)
		theErr = HMHideTagWithOptions(theFlags);
	else
		theErr = HMHideTag();

	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHelpTag::GetBounds : Get the bounds.
//----------------------------------------------------------------------------
HIRect NHelpTag::GetBounds(void) const
{	HIRect		theRect;



	// Get the bounds
	NGeometryUtilities::ConvertRect(mContent.absHotRect, theRect);
	
	return(theRect);
}





//============================================================================
//		NHelpTag::SetBounds : Set the bounds.
//----------------------------------------------------------------------------
void NHelpTag::SetBounds(const HIRect &theRect)
{


	// Set the bounds
	NGeometryUtilities::ConvertRect(theRect, mContent.absHotRect);
}





//============================================================================
//		NHelpTag::GetPlacement : Get the placement.
//----------------------------------------------------------------------------
HMTagDisplaySide NHelpTag::GetPlacement(void) const
{


	// Get the placement
	return(mContent.tagSide);
}





//============================================================================
//		NHelpTag::SetPlacement : Set the placement.
//----------------------------------------------------------------------------
void NHelpTag::SetPlacement(HMTagDisplaySide thePlacement)
{


	// Set the placement
	mContent.tagSide = thePlacement;
}





//============================================================================
//		NHelpTag::GetValue : Get the value.
//----------------------------------------------------------------------------
NString NHelpTag::GetValue(bool isExtended) const
{


	// Get the value
	if (isExtended)
		return(mValueExt);
	else
		return(mValueStd);
}





//============================================================================
//		NHelpTag::SetValue : Set the value.
//----------------------------------------------------------------------------
void NHelpTag::SetValue(const NString &theValue, bool isExtended)
{


	// Set the value
	if (isExtended)
		mValueExt = theValue;
	else
		mValueStd = theValue;



	// Update our state
	SyncContent();
}





//============================================================================
//		NHelpTag::operator = : Assignment operator.
//----------------------------------------------------------------------------
NHelpTag &NHelpTag::operator=(const NHelpTag &otherTag)
{


	// Assign the content
	if (this != &otherTag)
		AssignTag(otherTag);
		
	return(*this);
}





//============================================================================
//		NHelpTag::operator HMHelpContentPtr : HMHelpContentPtr-cast operator.
//----------------------------------------------------------------------------
NHelpTag::operator const HMHelpContentRec *(void) const
{


	// Get the content
	return(&mContent);
}





//============================================================================
//		NHelpTag::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NHelpTag::InitializeSelf(void)
{


	// Initialize ourselves
	memset(&mContent, 0x00, sizeof(mContent));

	mContent.version                = kMacHelpVersion;
	mContent.content[0].contentType = kHMNoContent;
	mContent.content[1].contentType = kHMNoContent;
}





//============================================================================
//		NHelpTag::SyncContent : Sync the content.
//----------------------------------------------------------------------------
void NHelpTag::SyncContent(void) const
{


	// Sync the content
	//
	// Since some APIS (e.g., HMDisplayTag) do not retain the content record
	// strings, we need to provide our own storage for the string data.
	//
	// These strings must be synced into the content record whenever they are
	// changed, to keep it up to date.
	mContent.content[0] = GetContentRecord(mValueStd);
	mContent.content[1] = GetContentRecord(mValueExt);
}





//============================================================================
//		NHelpTag::AssignTag : Assign a help tag.
//----------------------------------------------------------------------------
void NHelpTag::AssignTag(const NHelpTag &theTag)
{


	// Assign the tag
	mValueStd = theTag.mValueStd;
	mValueExt = theTag.mValueExt;
	mContent  = theTag.mContent;



	// Update our state
	SyncContent();
}





//============================================================================
//		NHelpTag::AssignContent : Assign a help record.
//----------------------------------------------------------------------------
void NHelpTag::AssignContent(const HMHelpContentRec &theContent)
{


	// Assign the content
	mValueStd = GetContentValue(theContent.content[0]);
	mValueExt = GetContentValue(theContent.content[1]);
	mContent  = theContent;



	// Update our state
	SyncContent();
}





//============================================================================
//		NHelpTag::GetContentRecord : Get a content record.
//----------------------------------------------------------------------------
HMHelpContent NHelpTag::GetContentRecord(const NString &theValue) const
{	HMHelpContent	theContent;



	// Get the record
	if (theValue.IsEmpty())
		{
		theContent.contentType   = kHMNoContent;
		theContent.u.tagCFString = NULL;
		}
	else
		{
		theContent.contentType   = kHMCFStringContent;
		theContent.u.tagCFString = theValue;
		}
	
	return(theContent);
}





//============================================================================
//		NHelpTag::GetContentValue : Get a content value.
//----------------------------------------------------------------------------
NString NHelpTag::GetContentValue(const HMHelpContent &theRecord) const
{	NString		theValue;



	// Get the value
	switch (theRecord.contentType) {
		case kHMCFStringContent:
			theValue = theRecord.u.tagCFString;
			break;
			
		case kHMNoContent:
			break;
				
		default:
			NN_LOG("Unknown help content: %d", theRecord.contentType);
			break;
		}
	
	return(theValue);
}
