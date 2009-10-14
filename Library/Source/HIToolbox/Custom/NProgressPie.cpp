/*	NAME:
		NProgressPie.cpp

	DESCRIPTION:
		Xcode-style "progress pie" view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGContext.h"
#include "NProgressPie.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const float kBorderWidth											= 2.0f;





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NProgressPie, "com.nano.nprogresspie", kHIViewClassID);





//============================================================================
//		NProgressPie::NProgressPie : Constructor.
//----------------------------------------------------------------------------
NProgressPie::NProgressPie(HIWindowRef			theWindow,
							const HIRect		&theRect,
							SInt32				valueInitial,
							SInt32				valueMinimum,
							SInt32				valueMaximum)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	SetMinimumValue(valueMinimum);
	SetMaximumValue(valueMaximum);
	SetCurrentValue(valueInitial);
}





//============================================================================
//		NProgressPie::~NProgressPie : Destructor.
//----------------------------------------------------------------------------
NProgressPie::~NProgressPie(void)
{
}





//============================================================================
//		NProgressPie::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NProgressPie::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw	},
										{ kEventClassNone,    kEventKindNone	} };



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NProgressPie::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NProgressPie::DoEventControlDraw(NCarbonEvent &theEvent)
{	float			theRadius, percentDone, angleCircle, angleStart, angleEnd;
	NCGContext		cgContext(theEvent);
	HIPoint			theCenter;
	HIRect			theRect;



	// Get the state we need
	theRect     = GetBounds();
	theCenter   = NGeometryUtilities::GetCenter(theRect);
	theRadius   = (std::min(theRect.size.width, theRect.size.height) / 2.0f) - (kBorderWidth / 2.0f);
	percentDone = GetPercentValue();

	angleCircle = M_PI * 2.0f;
	angleStart  = angleCircle * -0.25f;
	angleEnd    = angleStart + (angleCircle * percentDone);



	// Draw the background
	cgContext.SetFillColor(kThemeBrushFocusHighlight);

	cgContext.AddCircle(theCenter, theRadius);
	cgContext.FillPath();



	// Draw the filled portion
	if (NMathUtilities::NotEqual(percentDone, 1.0f))
		{
		cgContext.SetFillColor(kThemeBrushWhite);

		cgContext.MoveTo(theCenter);
		cgContext.AddArc(theCenter, theRadius - kBorderWidth, angleEnd, angleStart, false);
		cgContext.FillPath();
		}

	return(noErr);
}


