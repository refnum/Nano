/*	NAME:
		NOpenGLView.h

	DESCRIPTION:
		OpenGL view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NOPENGLVIEW_HDR
#define NOPENGLVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include <AGL/agl.h>

#include "NTimerTask.h"
#include "NHIView.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const GLbitfield kGLClearAll							= GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef std::vector<GLint>									GLPixelFormatInfo;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NOpenGLView : public NHIView, public NTimerTask {
public:
										DECLARE_HIVIEW_SUBCLASS(NOpenGLView);

										NOpenGLView(HIWindowRef					theWindow,
													const HIRect				&theRect,
													const GLPixelFormatInfo		*theFormat=NULL);

	virtual								~NOpenGLView(void);


	// Get the context
	AGLContext							GetContext(void) const;


	// Get/set the pixel format
	GLPixelFormatInfo					GetPixelFormat(void) const;
	void								SetPixelFormat(const GLPixelFormatInfo &theFormat);


	// Get/set a parameter
	GLint								GetParameter(GLenum theParam) const;
	void								SetParameter(GLenum theParam, GLint theValue);


	// Get/set the frame rate
	//
	// By default, OpenGL views automatically re-render at 30fps.
	//
	// Setting the frame rate to 0 will disable automatic rendering, and ensures
	// that the view must be explicitly refreshed to render each frame.
	EventTime							GetFrameRate(void) const;
	void								SetFrameRate(EventTime theTime);


	// Swap the context
	void								Swap(void);


	// Clear the context
	void								Clear(GLbitfield		theMask      = kGLClearAll,
												const NColor	&clearColor  = kColorBlack,
												float			clearDepth   = 1.0f,
												SInt32			clearStencil = 0);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(               NCarbonEvent &theEvent);
	OSStatus							DoEventControlBoundsChanged(      NCarbonEvent &theEvent);
	OSStatus							DoEventControlVisibilityChanged(  NCarbonEvent &theEvent);
	OSStatus							DoEventControlOwningWindowChanged(NCarbonEvent &theEvent);
	OSStatus							DoEventWindowBoundsChanged(       NCarbonEvent &theEvent);
	OSStatus							DoEventWindowToolbarSwitchMode(   NCarbonEvent &theEvent);


	// Handle a timer
	virtual void						DoTimer(NTimerID theTimer, TimerMsg theMsg);


	// Draw the view
	//
	// OpenGL views may need to draw their content in response to kEventControlDraw,
	// or to animate their content.
	//
	// Sub-classes should override this method to draw the content for the view, to
	// replace the default scene drawn by the base class.
	virtual void						DrawContent(NCarbonEvent &theEvent);


private:
	OSStatus							CreateContext(void);
	void								DestroyContext(void);

	void								RebuildContext(void);
	void								SyncContextBounds(void);


private:
	AGLContext							mContext;
	EventTime							mFrameRate;
	GLPixelFormatInfo					mFormatInfo;
	NCarbonEventHandler					*mWindowHandler;
};





#endif // NOPENGLVIEW_HDR


