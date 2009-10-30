/*	NAME:
		NOpenGLView.cpp

	DESCRIPTION:
		OpenGL view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <OpenGL/glu.h>

#include "NCGContext.h"
#include "NOpenGLView.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NOpenGLView, "com.nano.nopenglview", kHIViewClassID);





//============================================================================
//		NOpenGLView::NOpenGLView : Constructor.
//----------------------------------------------------------------------------
NOpenGLView::NOpenGLView(HIWindowRef					theWindow,
							const HIRect				&theRect,
							const GLPixelFormatInfo		*theFormat)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	if (theFormat != NULL)
		SetPixelFormat(*theFormat);
}





//============================================================================
//		NOpenGLView::~NOpenGLView : Destructor.
//----------------------------------------------------------------------------
NOpenGLView::~NOpenGLView(void)
{


	// Clean up
	DestroyContext();
}





//============================================================================
//		NOpenGLView::GetContext : Get the context.
//----------------------------------------------------------------------------
AGLContext NOpenGLView::GetContext(void) const
{


	// Get the context
	return(mContext);
}





//============================================================================
//		NOpenGLView::GetPixelFormat : Get the pixel format.
//----------------------------------------------------------------------------
GLPixelFormatInfo NOpenGLView::GetPixelFormat(void) const
{


	// Get the pixel format
	return(mFormatInfo);
}





//============================================================================
//		NOpenGLView::SetPixelFormat : Set the pixel format.
//----------------------------------------------------------------------------
void NOpenGLView::SetPixelFormat(const GLPixelFormatInfo &theFormat)
{


	// Set the pixel format
	mFormatInfo = theFormat;
	
	RebuildContext();
}





//============================================================================
//		NOpenGLView::GetParameter : Get a parameter.
//----------------------------------------------------------------------------
GLint NOpenGLView::GetParameter(GLenum theParam) const
{	GLint		theValue;
	GLboolean	glOK;



	// Validate our state
	NN_ASSERT(mContext != NULL);



	// Get the parameter
	theValue = 0;
	glOK     = aglGetInteger(mContext, theParam, &theValue);
	NN_ASSERT(glOK);
	
	return(theValue);
}





//============================================================================
//		NOpenGLView::SetParameter : Set a parameter.
//----------------------------------------------------------------------------
void NOpenGLView::SetParameter(GLenum theParam, GLint theValue)
{	GLboolean	glOK;



	// Validate our state
	NN_ASSERT(mContext != NULL);



	// Set the parameter
	glOK = aglSetInteger(mContext, theParam, &theValue);
	NN_ASSERT(glOK);
}





//============================================================================
//		NOpenGLView::GetFrameRate : Get the frame rate.
//----------------------------------------------------------------------------
EventTime NOpenGLView::GetFrameRate(void) const
{


	// Get the frame rate
	return(mFrameRate);
}





//============================================================================
//		NOpenGLView::SetFrameRate : Set the frame rate.
//----------------------------------------------------------------------------
void NOpenGLView::SetFrameRate(EventTime theTime)
{	NCarbonEvent	theEvent;



	// Set the frame rate
	mFrameRate = theTime;



	// Adjust the timer
	mTimer.RemoveTimer();
	
	if (NMathUtilities::NotZero(mFrameRate))
		mTimer.AddTimer(BindSelf(NOpenGLView::DrawContent, theEvent), 0.0f, mFrameRate);
}





//============================================================================
//		NOpenGLView::Swap : Swap the context.
//----------------------------------------------------------------------------
void NOpenGLView::Swap(void)
{


	// Validate our state
	NN_ASSERT(mContext != NULL);



	// Swap the context
	aglSwapBuffers(mContext);
}





//============================================================================
//		NOpenGLView::Clear : Clear the context.
//----------------------------------------------------------------------------
void NOpenGLView::Clear(GLbitfield		theMask,
						const NColor	&clearColor,
						float			clearDepth,
						SInt32			clearStencil)
{


	// Clear the context
	if (theMask & GL_COLOR_BUFFER_BIT)
		glClearColor(clearColor.GetRed(),
					 clearColor.GetGreen(),
					 clearColor.GetBlue(),
					 clearColor.GetAlpha());

	if (theMask & GL_DEPTH_BUFFER_BIT)
		glClearDepth(clearDepth);

	if (theMask & GL_STENCIL_BUFFER_BIT)
		glClearStencil(clearStencil);

	glClear(theMask);
}





//============================================================================
//		NOpenGLView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NOpenGLView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw					},
										{ kEventClassControl, kEventControlBoundsChanged		},
										{ kEventClassControl, kEventControlVisibilityChanged	},
										{ kEventClassControl, kEventControlOwningWindowChanged	},
										{ kEventClassNone,    kEventKindNone					} };



	// Initialize ourselves
	mContext       = NULL;
	mFrameRate     = kEventDurationSecond / 30.0f;
	mWindowHandler = NULL;

	mFormatInfo.push_back(AGL_DOUBLEBUFFER);
	mFormatInfo.push_back(AGL_ACCELERATED);
	mFormatInfo.push_back(AGL_RGBA);
	mFormatInfo.push_back(GL_TRUE);
	mFormatInfo.push_back(AGL_DEPTH_SIZE);
	mFormatInfo.push_back(32);



	// Create the context
	RebuildContext();



	// Register for events
	AddEvents(theEvents);

	SetFrameRate(mFrameRate);
}





//============================================================================
//		NOpenGLView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NOpenGLView::DoEventControlDraw(NCarbonEvent &theEvent)
{


	// Draw the content
	DrawContent(theEvent);

	return(noErr);
}





//============================================================================
//		NOpenGLView::DoEventControlBoundsChanged : Handle kEventControlBoundsChanged.
//----------------------------------------------------------------------------
OSStatus NOpenGLView::DoEventControlBoundsChanged(NCarbonEvent &/*theEvent*/)
{


	// Update the context
	SyncContextBounds();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NOpenGLView::DoEventControlVisibilityChanged : Handle kEventControlVisibilityChanged.
//----------------------------------------------------------------------------
OSStatus NOpenGLView::DoEventControlVisibilityChanged(NCarbonEvent &/*theEvent*/)
{


	// Show/hide the context
	//
	// OpenGL contexts don't have a traditional show/hide interface, and
	// so we destroy/create the context when our visibility changes.
	if (IsVisible())
		RebuildContext();
	else
		DestroyContext();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NOpenGLView::DoEventControlOwningWindowChanged : Handle kEventControlOwningWindowChanged.
//----------------------------------------------------------------------------
OSStatus NOpenGLView::DoEventControlOwningWindowChanged(NCarbonEvent &/*theEvent*/)
{


	// Rebuild the context
	RebuildContext();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NOpenGLView::DoEventWindowBoundsChanged : Handle kEventWindowBoundsChanged.
//----------------------------------------------------------------------------
OSStatus NOpenGLView::DoEventWindowBoundsChanged(NCarbonEvent &/*theEvent*/)
{	GLboolean	glOK;



	// Update the context
	if (mContext != NULL)
		{
		glOK = aglUpdateContext(mContext);
		NN_ASSERT(glOK);
		
		SyncContextBounds();
		}
	
	return(eventNotHandledErr);
}





//============================================================================
//		NOpenGLView::DoEventWindowToolbarSwitchMode : Handle kEventWindowToolbarSwitchMode.
//----------------------------------------------------------------------------
OSStatus NOpenGLView::DoEventWindowToolbarSwitchMode(NCarbonEvent &theEvent)
{	OSStatus	theErr;



	// Update the context
	theErr = CallNextHandler(theEvent);
	
	SyncContextBounds();

	return(theErr);
}





//============================================================================
//		NOpenGLView::DrawContent : Draw the content.
//----------------------------------------------------------------------------
//		Note :	The base class draws a simple scene, simply to confirm that
//				OpenGL context has been created and can be rendered to.
//
//				Sub-classes would typically override this method to render
//				their own content.
//----------------------------------------------------------------------------
void NOpenGLView::DrawContent(NCarbonEvent &theEvent)
{	static float	sRotateBy = 0.0f;
	NColor			theColor;
	HIRect			theRect;



	// Get the state we need
	theRect  = GetBounds();
	theColor = kColorBlack;



	// Prepare to render
	glViewport(0, 0, (GLint) theRect.size.width, (GLint) theRect.size.height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, theRect.size.width / theRect.size.height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	// Clear the background
	//
	// The scene may be drawn due to the refresh timer firing, or an HIView redraw.
	//
	// Unfortunately OpenGL works outside the HIView compositing system, and will
	// just draw on top of the views within our window.
	//
	// To minimize flicker between the window being flushed to the screen and the
	// OpenGL context being swapped, we clear the background of the HIView to match
	// the OpenGL background.
	if (theEvent.IsValid())
		{	NCGContext			cgContext(theEvent);
			StContextState		cgState(cgContext);

		cgContext.SetFillColor(theColor);
		cgContext.FillRect(theRect);
		}

	Clear(kGLClearAll, theColor);



	// Render the scene
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();
	glTranslatef(0.0f, 0.15f, -4.0f);
	glRotatef(sRotateBy, 0.0f, 1.0f, 0.0f);

	glBegin(GL_TRIANGLES);
		glColor3f(  1.0f,  0.0f,  0.0f);
		glVertex3f( 0.0f,  1.0f,  0.0f);
		glColor3f(  0.0f,  1.0f,  0.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glColor3f(  0.0f,  0.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);

		glColor3f(  1.0f,  0.0f,  0.0f);
		glVertex3f( 0.0f,  1.0f,  0.0f);
		glColor3f(  0.0f,  0.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glColor3f(  0.0f,  1.0f,  0.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);

		glColor3f(  1.0f,  0.0f,  0.0f);
		glVertex3f( 0.0f,  1.0f,  0.0f);
		glColor3f(  0.0f,  1.0f,  0.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glColor3f(  0.0f,  0.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);

		glColor3f(  1.0f,  0.0f,  0.0f);
		glVertex3f( 0.0f,  1.0f,  0.0f);
		glColor3f(  0.0f,  0.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glColor3f(  0.0f,  1.0f,  0.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
	glEnd();

	sRotateBy -= 2.0f;



	// Swap the context
	Swap();
}





//============================================================================
//		NOpenGLView::CreateContext : Create the context.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NOpenGLView::CreateContext(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassWindow, kEventWindowBoundsChanged		},
										{ kEventClassWindow, kEventWindowToolbarSwitchMode	},
										{ kEventClassNone,   kEventKindNone					} };

	GLPixelFormatInfo		glFormatInfo;
	HIWindowRef				theWindow;
	AGLPixelFormat			glFormat;



	// Validate our state
	NN_ASSERT(mContext == NULL);



	// Get the state we need
	theWindow = GetHIWindow();
	NN_ASSERT(theWindow != NULL);

	glFormatInfo = mFormatInfo;
	glFormatInfo.push_back(AGL_NONE);



	// Create the context
	glFormat = aglChoosePixelFormat(NULL, 0, &glFormatInfo[0]);
	
	if (glFormat != NULL)
		{
		mContext = aglCreateContext(glFormat, NULL);
		aglDestroyPixelFormat(glFormat);
		}
	
	if (mContext == NULL)
		return(paramErr);



	// Configure the context
	aglSetCurrentContext(mContext);
	aglSetDrawable(      mContext, GetWindowPort(theWindow));

	SyncContextBounds();
	Clear();



	// Register for events
	//
	// Although most of our events are view-related, we also need to know when
	// the state of our parent window changes to keep OpenGL in sync.
	mWindowHandler = new NCarbonEventHandler;
	if (mWindowHandler != NULL)
		{
		mWindowHandler->SetEventDelegate(this);
		mWindowHandler->AddEvents(theEvents);
		mWindowHandler->InstallHandler(theWindow);
		}

	return(noErr);
}





//============================================================================
//		NOpenGLView::DestroyContext : Destroy the context.
//----------------------------------------------------------------------------
void NOpenGLView::DestroyContext(void)
{


	// Destroy the context
	if (mWindowHandler != NULL)
		delete mWindowHandler;

	if (mContext != NULL)
		aglDestroyContext(mContext);



	// Reset our state
	mContext       = NULL;
	mWindowHandler = NULL;
}





//============================================================================
//		NOpenGLView::RebuildContext : Rebuild the context.
//----------------------------------------------------------------------------
void NOpenGLView::RebuildContext(void)
{


	// Rebuild the context
	DestroyContext();

	if (GetHIWindow() != NULL)
		CreateContext();
}





//============================================================================
//		NOpenGLView::SyncContextBounds : Synchronize the context bounds.
//----------------------------------------------------------------------------
void NOpenGLView::SyncContextBounds(void)
{	NHIWindow		theWindow(GetHIWindow());
	HIRect			viewRect, windowRect;
	GLint			glRect[4];
	OSStatus		theErr;



	// Check our state
	if (mContext == NULL || !theWindow.IsValid())
		return;



	// Get the state we need
	//
	// OpenGL works in window coordinates, and so we need our bounds in the
	// root coordinate system of our window.
	windowRect = theWindow.GetFrame();
	viewRect   = GetBounds();

	theErr = HIViewConvertRect(&viewRect, *this, theWindow.GetHIView(kHIViewWindowContentID));
	NN_ASSERT_NOERR(theErr);



	// Update the context
	//
	// An OpenGL context is associated with a window, and renders to the drawable
	// area of that window (i.e., the content view).
	//
	// In order to constrain the context to our view bounds, we must set the buffer
	// rect of the context to our bounds. The origin of the buffer rect coordinate
	// system is the bottom-left of the window.
	glRect[0] = (GLint) viewRect.origin.x;
	glRect[1] = (GLint) (windowRect.size.height - CGRectGetMaxY(viewRect));
	glRect[2] = (GLint) viewRect.size.width;
	glRect[3] = (GLint) viewRect.size.height;

	aglSetInteger(mContext, AGL_BUFFER_RECT, glRect);
	aglEnable(    mContext, AGL_BUFFER_RECT);
}


