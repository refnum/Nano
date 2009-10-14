/*	NAME:
		NHelpTag.h

	DESCRIPTION:
		Help tag.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHelpTag_HDR
#define NHelpTag_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NString.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NHIView;
class NHIWindow;
class NMenuItem;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHelpTag {
public:
										NHelpTag(const NHIView   *theView);
										NHelpTag(const NHIWindow *theWindow);
										NHelpTag(const NMenuItem &theItem);

										NHelpTag(const NString  &theValue);
										NHelpTag(const NHelpTag &theTag);
										
										NHelpTag(void);
	virtual								~NHelpTag(void);


	// Show/hide the tag
	//
	// If no point is supplied, the tag is shown at the current mouse position.
	void								Show(const HIPoint    &thePoint=kHIPointZero);
	void								Hide(      OptionBits  theFlags=kHMHideTagFade);


	// Get/set the bounds
	HIRect								GetBounds(void) const;
	void								SetBounds(const HIRect &theRect);


	// Get/set the placement
	HMTagDisplaySide					GetPlacement(void) const;
	void								SetPlacement(HMTagDisplaySide thePlacement);


	// Get/set the value
	NString								GetValue(                         bool isExtended=false) const;
	void								SetValue(const NString &theValue, bool isExtended=false);


	// Operators
	NHelpTag							&operator = (const NHelpTag &otherTag);
	operator							const HMHelpContentRec *(void) const;


private:
	void								InitializeSelf(void);
	void								SyncContent(   void) const;

	void								AssignTag(    const NHelpTag         &theTag);
	void								AssignContent(const HMHelpContentRec &theContent);

	HMHelpContent						GetContentRecord(const NString       &theValue)  const;
	NString								GetContentValue( const HMHelpContent &theRecord) const;


private:
	NString								mValueStd;
	NString								mValueExt;
	
	mutable HMHelpContentRec			mContent;
};





#endif // NHelpTag_HDR


