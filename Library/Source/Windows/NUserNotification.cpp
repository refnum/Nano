/*	NAME:
		NUserNotification.cpp

	DESCRIPTION:
		User notification.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "Nano Build.h"
#include "NCFBundle.h"
#include "NApplication.h"
#include "NIBUtilities.h"
#include "NHIViewUtilities.h"
#include "NBezelView.h"
#include "NIconView.h"
#include "NUserNotification.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kWindowNotification							= "UserNotification";
static const float   kWindowInset									= 20.0f;
static const float   kTextBorder									= 2.0f;

static const HIViewID kViewIcon										= { 'icon', 0 };
static const HIViewID kViewMessage									= { 'text', 0 };

static const EventTime kTimeExpire									= 10.0;
static const EventTime kTimeUpdate									= 0.25;





//============================================================================
//		Controller
//----------------------------------------------------------------------------
DEFINE_WINDOW_CONTROLLER(NUserNotification, kNanoNibFile, kWindowNotification);





//============================================================================
//		NUserNotification::NUserNotification : Constructor.
//----------------------------------------------------------------------------
NUserNotification::NUserNotification(void)
{
}





//============================================================================
//		NUserNotification::~NUserNotification : Destructor.
//----------------------------------------------------------------------------
NUserNotification::~NUserNotification(void)
{
}





//============================================================================
//		NUserNotification::PostMessage : Post a notification message.
//----------------------------------------------------------------------------
void NUserNotification::PostMessage(const NString &theText)
{	NCFBundle			appBundle;
	NotificationMsg		theMsg;



	// Post the message
	theMsg.theMsg     = theText;
	theMsg.theIcon    = NIcon(appBundle.GetInfoString(kCFBundleIconFileKey));
	theMsg.timeExpire = GetCurrentEventTime() + kTimeExpire;

	mMessages.push_back(theMsg);
	UpdateMessages(true);
}





//============================================================================
//		NUserNotification::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NUserNotification::InitializeSelf(void)
{	NHIWindow		*theWindow;
	OSStatus		theErr;



	// Configure the window
	theWindow = GetWindow();

	theWindow->SetAttributes(kWindowNoTitleBarAttribute);
	theWindow->SetContentTransparent(true);



	// Work around rdar://5570606.
	//
	// Transparent floating windows do not draw correctly on 10.5 when their app
	// is in the background (they draw as solid white). To work around this, we
	// use a utility window to display notifications.
	theErr = HIWindowChangeClass(*theWindow, kUtilityWindowClass);
	NN_ASSERT_NOERR(theErr);
	
	return(noErr);
}





//============================================================================
//		NUserNotification::UpdateMessages : Update the messages.
//----------------------------------------------------------------------------
#pragma mark -
void NUserNotification::UpdateMessages(bool didChange)
{	NHIWindow		*theWindow;



	// Get the state we need
	theWindow  = GetWindow();
	didChange |= PurgeMessages();



	// Remove the window
	if (mMessages.empty())
		HideWindow();


	// Update the window
	else
		{
		if (didChange || !theWindow->IsVisible())
			UpdateWindow();

		ShowWindow();
		}
}





//============================================================================
//		NUserNotification::UpdateWindow : Update the window.
//----------------------------------------------------------------------------
void NUserNotification::UpdateWindow(void)
{	NHIView								*contentView, *theView;
	HIRect								windowRect, viewRect;
	NBezelView							*bezelView;
	NHIWindow							*theWindow;
	NotificationMsgListConstIterator	theIter;
	IBNibRef							nibRef;



	// Get the state we need
	nibRef      = NIBUtilities::CreateNib(kNanoNibFile);
	theWindow   = GetWindow();
	windowRect  = theWindow->GetFrame();
	contentView = theWindow->GetView(kHIViewWindowContentID);



	// Prepare the window
	contentView->RemoveSubViews();
	
	windowRect.size.height = 0.0f;
	viewRect               = kHIRectZero;



	// Create the views
	for (theIter = mMessages.begin(); theIter != mMessages.end(); theIter++)
		{
		theView = NHIViewUtilities::GetView(NIBUtilities::CreateView(nibRef, kWindowNotification));
		if (theView != NULL)
			{
			// Configure the view
			ApplyMessage(theView, *theIter);

			if (viewRect == kHIRectZero)
				viewRect = theView->GetBounds();
			else
				viewRect.origin.y += viewRect.size.height;
			

			
			// Append it to the window
			contentView->AddSubView(theView);
			theView->SetFrame(viewRect);

			windowRect.size.height += viewRect.size.height;
			}
		}



	// Configure the window
	bezelView = new NBezelView(*theWindow, CGRectMake(0, 0, windowRect.size.width, windowRect.size.height));
	if (bezelView != NULL)
		bezelView->SetZOrder(kHIViewZOrderBelow);

	theWindow->SetFrame(windowRect, kWindowContentRgn, kWindowSlideTransitionEffect, kWindowResizeTransitionAction);



	// Clean up
	DisposeNibReference(nibRef);
}





//============================================================================
//		NUserNotification::PurgeMessages : Purge any expired messages.
//----------------------------------------------------------------------------
bool NUserNotification::PurgeMessages(void)
{	UInt32							oldCount;
	EventTime						timeNow;
	NotificationMsgListIterator		theIter;



	// Get the state we need
	timeNow  = GetCurrentEventTime();
	oldCount = mMessages.size();



	// Purge the messages
	for (theIter = mMessages.begin(); theIter != mMessages.end(); )
		{
		if (timeNow >= theIter->timeExpire)
			theIter = mMessages.erase(theIter);
		else
			theIter++;
		}
	
	return(mMessages.size() != oldCount);
}





//============================================================================
//		NUserNotification::ApplyMessage : Apply a message to its view.
//----------------------------------------------------------------------------
void NUserNotification::ApplyMessage(NHIView *theView, const NotificationMsg &theMsg)
{	UInt32			n, numViews;
	float			textHeight;
	NHIView			*theChild;
	NIconView		*iconView;
	NStaticText		*textView;
	HIRect			textRect;
	HIViewID		childID;



	// Get the state we need
	iconView = NULL;
	textView = NULL;
	numViews = theView->GetSubViewCount();

	for (n = 0; n < numViews; n++)
		{
		theChild = theView->GetSubView(n);
		childID  = theChild->GetViewID();

		if (childID == kViewIcon)
			iconView = dynamic_cast<NIconView*>(theChild);
			
		else if (childID == kViewMessage)
			textView = dynamic_cast<NStaticText*>(theChild);
		}



	// Configure the icon
	if (iconView != NULL)
		iconView->SetIcon(theMsg.theIcon);



	// Configure the message
	if (textView != NULL)
		{
		textView->SetTextValue(theMsg.theMsg);

		textRect   = textView->GetFrame();
		textHeight = textView->GetTextHeight() + (kTextBorder * 2.0f);
			
		textRect.origin.y     = CGRectGetMidY(textRect) - (textHeight / 2.0f);
		textRect.size.height = textHeight;

		textView->SetFrame(textRect);
		}
}





//============================================================================
//		NUserNotification::ShowWindow : Show the window.
//----------------------------------------------------------------------------
void NUserNotification::ShowWindow(void)
{	HIRect		windowRect, screenRect;
	NHIWindow	*theWindow;



	// Get the state we need
	theWindow  = GetWindow();
	windowRect = theWindow->GetFrame();
	screenRect = CGDisplayBounds(CGMainDisplayID());
	
	windowRect.origin.x = CGRectGetMaxX(screenRect) - CGRectGetWidth(windowRect) - kWindowInset;
	windowRect.origin.y = GetMBarHeight() + kWindowInset;



	// Show the window
	if (!theWindow->IsVisible())
		{
		theWindow->SetFrame(windowRect);
		theWindow->SetVisible(true, true);
		}

	if (!mTimer.HasTimer())
		mTimer.AddTimer(BindSelf(NUserNotification::UpdateMessages, false), kTimeUpdate, kTimeUpdate);
}





//============================================================================
//		NUserNotification::HideWindow : Hide the window.
//----------------------------------------------------------------------------
void NUserNotification::HideWindow(void)
{	NHIWindow	*theWindow;



	// Get the state we need
	theWindow = GetWindow();
	
	
	
	// Hide the window
	if (theWindow->IsVisible())
		theWindow->SetVisible(false, true);

	mTimer.RemoveTimer();
}




