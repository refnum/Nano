/*	NAME:
		NHISegmentedView.h

	DESCRIPTION:
		Segmented view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHISEGMENTEDVIEW_HDR
#define NHISEGMENTEDVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"
#include "NArray.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const UInt32 kAllSegments								= ULONG_MAX;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHISegmentedView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NHISegmentedView);

										NHISegmentedView(HIWindowRef		theWindow,
															const HIRect	&theRect,
															UInt32			numItems = 0);
	virtual								~NHISegmentedView(void);


	// Get/set the segment count
	UInt32								GetSegmentCount(void) const;
	void								SetSegmentCount(UInt32 theValue);


	// Get/set the enabled state of a segment
	//
	// An index of kAllSegments may be used to update all segments.
	bool								IsSegmentEnabled(UInt32 theIndex) const;
	void								SetSegmentEnabled(UInt32 theIndex, bool isEnabled);


	// Get/set the behaviour of a segment
	//
	// An index of kAllSegments may be used to update all segments.
	HISegmentBehavior					GetSegmentBehaviour(UInt32 theIndex) const;
	void								SetSegmentBehaviour(UInt32 theIndex, HISegmentBehavior theBehaviour);


	// Get/set the attributes of a segment
	//
	// An index of kAllSegments may be used to update all segments.
	OptionBits							GetSegmentAttributes(UInt32 theIndex) const;
	void								SetSegmentAttributes(UInt32 theIndex, OptionBits setThese, OptionBits clearThese=kHISegmentNoAttributes);


	// Get/set the value of a segment
	//
	// An index of kAllSegments may be used to update all segments.
	SInt32								GetSegmentValue(UInt32 theIndex) const;
	void								SetSegmentValue(UInt32 theIndex, SInt32 theValue);
	

	// Get/set the command of a segment
	//
	// An index of kAllSegments may be used to update all segments.
	UInt32								GetSegmentCommand(UInt32 theIndex) const;
	void								SetSegmentCommand(UInt32 theIndex, UInt32 theCmd);


	// Get/set the label of a segment
	//
	// An index of kAllSegments may be used to update all segments.
	NString								GetSegmentLabel(UInt32 theIndex) const;
	void								SetSegmentLabel(UInt32 theIndex, const NString &theValue);


	// Get/set the width of a segment
	//
	// Assigning a width of 0 will auto-select an appropriate width.
	//
	// An index of kAllSegments may be used to update all segments.
	float								GetSegmentWidth(UInt32 theIndex) const;
	void								SetSegmentWidth(UInt32 theIndex, float theWidth);


	// Get/set the icon for a segment
	//
	// An index of kAllSegments may be used to update all segments.
	NIcon								GetSegmentIcon(UInt32 theIndex) const;
	void								SetSegmentIcon(UInt32 theIndex, const NIcon &theIcon);


	// Get/set the image for a segment
	//
	// An index of kAllSegments may be used to update all segments.
	NCGImage							GetSegmentImage(UInt32 theIndex) const;
	void								SetSegmentImage(UInt32 theIndex, const NCGImage &theImage);


protected:
	// Initialize the view
	void								InitializeView(void);


private:


};





#endif // NHISEGMENTEDVIEW_HDR


