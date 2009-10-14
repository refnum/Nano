/*	NAME:
		NMenuItemView.cpp

	DESCRIPTION:
		Custom menu item view.
		
		The menu item view is an HIView sub-class that can be embedded within
		a menu, completely replacing the contents of a menu item with a view.
		
		The base class registers for a set of events likely to be useful for
		sub-classes, handles embedding the HIView into the menu view, and will
		provide default behaviour.


		A simple sub-class simply needs to implement DoEventControlDraw and
		DoEventControlGetOptimalBounds to draw and measure their content.
		
		More complex views can implement the hit-testing and mouse-tracking
		methods, allowing them to track the mouse over parts within the view.
		
		A set of DoMenuItemXXX methods are provided for notifications, which
		sub-classes can override to update their state as necessary.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGContext.h"
#include "NMenuItemView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Example
static const float  kExampleBorder									= 20.0f;
static const HIRect kExampleBounds									= { kHIPointZero, { 150.0f, 90.0f } };
static const NColor kExampleFill									= NColor(0.0f, 0.447f, 0.776f, 1.0f);
static const NColor kExampleStroke									= kColorWhite;


// Events
static const EventTypeSpec kEventsView[] =							{	{ kEventClassControl, kEventControlDraw					},
																		{ kEventClassControl, kEventControlGetOptimalBounds		},
																		{ kEventClassControl, kEventControlGetPartRegion		},
																		{ kEventClassControl, kEventControlHit					},
																		{ kEventClassControl, kEventControlHitTest				},
																		{ kEventClassControl, kEventControlTrack				},
																		{ kEventClassControl, kEventControlVisibilityChanged	},
																		{ kEventClassNone,    kEventKindNone					} };

static const EventTypeSpec kEventsMenu[] =							{	{ kEventClassCommand, kEventCommandProcess				},
																		{ kEventClassMenu,    kEventMenuCalculateSize			},
																		{ kEventClassMenu,    kEventMenuDrawItem				},
																		{ kEventClassMenu,    kEventMenuMeasureItemHeight		},
																		{ kEventClassMenu,    kEventMenuMeasureItemWidth		},
																		{ kEventClassMenu,    kEventMenuPopulate				},
																		{ kEventClassNone,    kEventKindNone					} };

static const EventTypeSpec kEventsMenuView[] =						{	{ kEventClassControl, kEventControlSimulateHit			},
																		{ kEventClassNone,    kEventKindNone					} };

static const EventTypeSpec kEventsMenuWindow[] =					{	{ kEventClassMouse,   kEventMouseDragged				},
																		{ kEventClassMouse,   kEventMouseMoved					},
																		{ kEventClassNone,    kEventKindNone					} };





//============================================================================
//		NMenuItemView::NMenuItemView : Constructor.
//----------------------------------------------------------------------------
NMenuItemView::NMenuItemView(void)
{
}





//============================================================================
//		NMenuItemView::~NMenuItemView : Destructor.
//----------------------------------------------------------------------------
NMenuItemView::~NMenuItemView(void)
{
}





//============================================================================
//		NMenuItemView::GetMenuItem : Get the menu item.
//----------------------------------------------------------------------------
NMenuItem NMenuItemView::GetMenuItem(void) const
{


	// Get the menu item
	return(mMenuItem);
}





//============================================================================
//		NMenuItemView::SetMenuItem : Set the menu item.
//----------------------------------------------------------------------------
OSStatus NMenuItemView::SetMenuItem(const NMenuItem &menuItem, ThemeMenuType menuType)
{	HIViewRef		menuView;
	NMenu			theMenu;
	OSStatus		theErr;



	// Detach from the old menu
	if (GetSuperView() != NULL)
		RemoveFromSuperView();



	// Set the menu itme
	mMenuItem = menuItem;
	theErr    = noErr;



	// Attach to the new menu
	if (mMenuItem.IsValid())
		{
		// Get the state we need
		theMenu = menuItem.GetMenu();
		theErr  = HIMenuGetContentView(theMenu, menuType, &menuView);
		NN_ASSERT_NOERR(theErr);

		if (theErr != noErr)
			return(theErr);



		// Install ourselves into the menu view
		theErr = HIViewAddSubview(menuView, *this);
		NN_ASSERT_NOERR(theErr);

		mDelegateMenu.InstallHandler(     theMenu, kEventsMenu);
		mDelegateMenuView.InstallHandler(menuView, kEventsMenuView);



		// Update our state
		//
		// To allow our appearance to change dynamically as the mouse
		// tracks over the view, we disable image caching for the menu.
		theMenu.SetAttributes(  kMenuAttrDoNotCacheImage);
		mMenuItem.SetAttributes(kMenuItemAttrCustomDraw);

		DoMenuItemChanged();
		}

	return(theErr);
}





//============================================================================
//		NMenuItemView::IsMenuItemEnabled : Is the menu item enabled?
//----------------------------------------------------------------------------
bool NMenuItemView::IsMenuItemEnabled(void) const
{


	// Get our state
	return(mMenuItem.IsEnabled());
}





//============================================================================
//		NMenuItemView::IsMenuItemSelected : Is the menu item selected?
//----------------------------------------------------------------------------
bool NMenuItemView::IsMenuItemSelected(void) const
{


	// Get our state
	return(mMenuItem.GetMenu().GetTracking().itemSelected == mMenuItem.GetIndex());
}





//============================================================================
//		NMenuItemView::GetMenuItemText : Get the menu item text.
//----------------------------------------------------------------------------
NString NMenuItemView::GetMenuItemText(void) const
{


	// Get our state
	return(mMenuItem.GetText());
}





//============================================================================
//		NMenuItemView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NMenuItemView::InitializeView(void)
{


	// Register for events
	AddEvents(kEventsView);

	mDelegateMenu.SetEventDelegate(    this);
	mDelegateMenuView.SetEventDelegate(this);
}





//============================================================================
//		NMenuItemView::DoMenuItemChanged : Handle changes to the menu item.
//----------------------------------------------------------------------------
//		Note : Sub-classes should override to handle a change in menu item.
//----------------------------------------------------------------------------
void NMenuItemView::DoMenuItemChanged(void)
{
}





//============================================================================
//		NMenuItemView::DoMenuItemPopulate : Populate the menu item.
//----------------------------------------------------------------------------
//		Note : Sub-classes should override to populate their content.
//----------------------------------------------------------------------------
void NMenuItemView::DoMenuItemPopulate(void)
{
}





//============================================================================
//		NMenuItemView::DoMenuItemHitTest : Hit-test the menu item.
//----------------------------------------------------------------------------
//		Note : Sub-classes should override to hit-test their parts.
//----------------------------------------------------------------------------
HIViewPartCode NMenuItemView::DoMenuItemHitTest(const HIPoint &/*thePoint*/)
{


	// Base class has no parts
	return(kHIViewEntireView);
}





//============================================================================
//		NMenuItemView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
//		Note :	Sub-classes must override this method to draw their content,
//				so we provide a default implementation to ensure they do.
//----------------------------------------------------------------------------
OSStatus NMenuItemView::DoEventControlDraw(NCarbonEvent &theEvent)
{	NColor			colorStroke, colorFill;
	NCGContext		cgContext(theEvent);
	HIRect			theRect;



	// Get the state we need
	theRect = kExampleBounds;
	NGeometryUtilities::PositionRect(theRect, GetBounds(), kPositionCenter);

	colorStroke = kExampleStroke;
	colorFill   = kExampleFill;
	
	if (IsMenuItemSelected())
		std::swap(colorStroke, colorFill);



	// Draw the example
	if (!IsMenuItemEnabled())
		cgContext.SetAlpha(kHIViewDisabledAlpha);

	cgContext.SetStrokeColor(colorStroke);
	cgContext.SetFillColor(  colorFill);
	cgContext.SetLineWidth(  kExampleBounds.size.height / 5.0f);

	cgContext.ClipToRect(theRect);
	cgContext.FillRect(theRect);
	cgContext.BeginPath();
		cgContext.MoveTo(   CGRectGetMinX(theRect), CGRectGetMinY(theRect));
		cgContext.AddLineTo(CGRectGetMaxX(theRect), CGRectGetMaxY(theRect));
		cgContext.MoveTo(   CGRectGetMaxX(theRect), CGRectGetMinY(theRect));
		cgContext.AddLineTo(CGRectGetMinX(theRect), CGRectGetMaxY(theRect));
	cgContext.StrokePath();

	cgContext.SetStrokeColor(kColorGray);
	cgContext.SetLineWidth(1.0f);
	cgContext.StrokeRect(theRect);

	return(noErr);
}





//============================================================================
//		NMenuItemView::DoEventControlGetOptimalBounds : Handle kEventControlGetOptimalBounds.
//----------------------------------------------------------------------------
//		Note :	Sub-classes must override this method to measure their content,
//				so we provide a default implementation to ensure they do.
//----------------------------------------------------------------------------
OSStatus NMenuItemView::DoEventControlGetOptimalBounds(NCarbonEvent &theEvent)
{	HIRect	theRect;



	// Measure the example
	theRect = CGRectInset(kExampleBounds, -kExampleBorder, -kExampleBorder / 2.0f);
	
	theEvent.SetParameterHIRect(kEventParamControlOptimalBounds, theRect);
	
	return(noErr);
}





//============================================================================
//		NMenuItemView::DoEventControlHitTest : Handle kEventControlHitTest.
//----------------------------------------------------------------------------
OSStatus NMenuItemView::DoEventControlHitTest(NCarbonEvent &theEvent)
{	HIPoint				thePoint;
	HIViewPartCode		thePart;



	// Get the state we need
	thePoint = theEvent.GetMouseLocation();
	thePart  = kHIViewNoPart;



	// Hit-test the view
	//
	// The base class performs a basic hit-test against the view, and sub
	// classes can overide DoMenuItemHitTest to test their specific parts.
	if (IsMenuItemEnabled() && CGRectContainsPoint(GetBounds(), thePoint))
		thePart = DoMenuItemHitTest(thePoint);
		
	theEvent.SetViewPartCode(thePart);

	return(noErr);
}





//============================================================================
//		NMenuItemView::DoEventControlVisibilityChanged : Handle kEventControlVisibilityChanged.
//----------------------------------------------------------------------------
OSStatus NMenuItemView::DoEventControlVisibilityChanged(NCarbonEvent &/*theEvent*/)
{


	// Attach to the menu window
	//
	// Whenever our view becomes visible, we install a handler on the menu's
	// window to allow us to capture mouse moved/dragged events over our view.
	if (IsVisible())
		{
		if (mDelegateMenuWindow.GetEventDelegate() == NULL)
			{
			mDelegateMenuWindow.SetEventDelegate(this);
			mDelegateMenuWindow.InstallHandler(GetHIWindow(), kEventsMenuWindow);
			}
		}
	
	
	// Or detach ourselves
	else
		{
		if (mDelegateMenuWindow.GetEventDelegate() != NULL)
			{
			mDelegateMenuWindow.SetEventDelegate(NULL);
			mDelegateMenuWindow.RemoveHandler();
			}
		}

	return(eventNotHandledErr);
}





//============================================================================
//		NMenuItemView::DoEventMenuCalculateSize : Handle kEventMenuCalculateSize.
//----------------------------------------------------------------------------
OSStatus NMenuItemView::DoEventMenuCalculateSize(NCarbonEvent &theEvent)
{	Rect			qdRect = { 0 };
	HIRect			theRect;
	OSStatus		theErr;
	RgnHandle		qdRgn;



	// Invoke the default handler
	theErr = CallNextHandler(theEvent);
	if (theErr != noErr)
		return(theErr);



	// Retrieve the bounds it calculated
	qdRgn = NewRgn();
	if (qdRgn != NULL)
		{
		GetControlRegion(*GetSuperView(), mMenuItem.GetIndex(), qdRgn);
		GetRegionBounds(qdRgn, &qdRect);

		DisposeRgn(qdRgn);
		}



	// Reposition our view to match
	NGeometryUtilities::ConvertRect(qdRect, theRect);
	SetFrame(theRect);
	
	return(noErr);
}





//============================================================================
//		NMenuItemView::DoEventMenuDrawItem : Handle kEventMenuDrawItem.
//----------------------------------------------------------------------------
OSStatus NMenuItemView::DoEventMenuDrawItem(NCarbonEvent &/*theEvent*/)
{


	// Suppress the default handler
	//
	// By preventing the default handler from drawing the menu item,
	// our view will be able to draw its content instead.
	return(noErr);
}





//============================================================================
//		NMenuItemView::DoEventMenuMeasureItemHeight : Handle kEventMenuMeasureItemHeight.
//----------------------------------------------------------------------------
OSStatus NMenuItemView::DoEventMenuMeasureItemHeight(NCarbonEvent &theEvent)
{	NMenuItem		menuItem(theEvent);
	HISize			theSize;



	// Check our parameters
	if (menuItem != mMenuItem)
		return(eventNotHandledErr);



	// Get the height
	theSize = GetIdealSize().size;
	theEvent.SetParameterSInt32(kEventParamMenuItemHeight, (SInt32) theSize.height);
	
	return(noErr);
}





//============================================================================
//		NMenuItemView::DoEventMenuMeasureItemWidth : Handle kEventMenuMeasureItemWidth.
//----------------------------------------------------------------------------
OSStatus NMenuItemView::DoEventMenuMeasureItemWidth(NCarbonEvent &theEvent)
{	NMenuItem		menuItem(theEvent);
	HISize			theSize;



	// Check our parameters
	if (menuItem != mMenuItem)
		return(eventNotHandledErr);



	// Get the width
	theSize = GetIdealSize().size;
	theEvent.SetParameterSInt32(kEventParamMenuItemWidth, (SInt32) theSize.width);
	
	return(noErr);
}





//============================================================================
//		NMenuItemView::DoEventMenuPopulate : Handle kEventMenuPopulate.
//----------------------------------------------------------------------------
OSStatus NMenuItemView::DoEventMenuPopulate(NCarbonEvent &theEvent)
{	UInt32		theContext;
	bool		isDisplay;



	// Get the state we need
	theContext = theEvent.GetParameterUInt32(kEventParamMenuContext);
	isDisplay  = (theContext & (kMenuContextMenuBarTracking | kMenuContextPopUpTracking));



	// Update the base class
	if (isDisplay)
		DoMenuItemPopulate();

	return(eventNotHandledErr);
}




