/*	NAME:
		NHIImageView.h

	DESCRIPTION:
		Image view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHIIMAGEVIEW_HDR
#define NHIIMAGEVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NScrollableView.h"
#include "NMsgBroadcaster.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Image scaling
//
// These values are used for kTagImageScaling, and can never change.
typedef enum {
	kScaleNone					= 'none',		// Clip  to GetBounds
	kScaleToImage				= 'imag',		// Scale to GetImageSize
	kScaleToFit					= 'fits',		// Scale to GetBounds
	kScaleProportionally		= 'prop'		// Scale to GetBounds, preserving proportions
} ImageScaling;


// Messages
static const BroadcastMsg kMsgImageReceivedFile						= 'ifil';





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHIImageView : public NScrollableView, public NMsgBroadcaster {
public:
										DECLARE_HIVIEW_SUBCLASS(NHIImageView);

										NHIImageView(HIWindowRef			theWindow,
														const HIRect		&theRect,
														const NCGImage		*theImage = NULL);
	virtual								~NHIImageView(void);


	// Get/set the image
	NCGImage							GetImage(void) const;
	void								SetImage(const NCGImage &theImage);


	// Get/set the alpha
	float								GetAlpha(void) const;
	void								SetAlpha(float theAlpha);


	// Get/set the image scale
	ImageScaling						GetScaling(void) const;
	void								SetScaling(ImageScaling theScaling);


	// Get/set the opaque state
	bool								IsOpaque(void) const;
	void								SetOpaque(bool isOpaque);


	// Get/set the editable state
	//
	// Editable images support kHICommandCut/Copy/Paste/Clear, and drag+drop.
	bool								IsEditable(void) const;
	void								SetEditable(bool isEditable);


	// Get the status of an edit command
	//
	// Obtain the command status of a kHICommandCut/Copy/Paste/Clear command.
	CommandStatus						GetEditStatus(UInt32 theCmd);


	// Perform an edit command
	//
	// Performs a kHICommandCut/Copy/Paste/Clear command on the image.
	OSStatus							DoEditCommand(UInt32 theCmd);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	virtual OSStatus					DoEventControlBoundsChanged(NCarbonEvent &theEvent);


	// Receive a drag
	DragActions							DoDragReceive(DragRef theDrag);


	// Draw the view
	void								DrawContent(NCarbonEvent &theEvent);


private:
	ImageScaling						mScaling;
	bool								mIsEditable;
};





#endif // NHIIMAGEVIEW_HDR


