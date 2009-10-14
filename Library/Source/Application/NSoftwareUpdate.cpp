/*	NAME:
		NSoftwareUpdate.cpp

	DESCRIPTION:
		Software Update client.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <netdb.h>

#include "Nano Build.h"
#include "NLaunchServicesUtilities.h"
#include "NSystemUtilities.h"
#include "NHIViewUtilities.h"
#include "NNetworkUtilities.h"
#include "NProcessUtilities.h"
#include "NTextUtilities.h"
#include "NFileUtilities.h"
#include "NFileIterator.h"
#include "NShadingView.h"
#include "NStaticText.h"
#include "NHIWebView.h"
#include "NProgressBar.h"
#include "NIconView.h"
#include "NCFBundle.h"
#include "NApplication.h"
#include "NSoftwareUpdate.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kItemPublishedKey								= "pubDate";
static const NString kItemEnclosureURLKey							= "enclosureURL";
static const NString kItemEnclosureSizeKey							= "enclosureSize";
static const NString kItemEnclosureMD5Key							= "appCast:enclosureMD5";
static const NString kItemSelfInstallKey							= "appCast:selfInstall";
static const NString kItemUpdateInfoKey								= "appCast:updateInfo";
static const NString kItemUpdateVersionKey							= "appCast:updateVersion";
static const NString kItemUpdateVersionNameKey						= "appCast:updateVersionName";
static const NString kNanoAppNameKey								= "nano:appName";
static const NString kNanoVersionOldKey								= "nano:versionOld";
static const NString kNanoVersionNewKey								= "nano:versionNew";

static const NString kTokenAppName									= "APPNAME";
static const NString kTokenVersionOld								= "OLDVERSION";
static const NString kTokenVersionNew								= "NEWVERSION";

static const UInt32 kCmdUpdateSkip									= 'skip';
static const UInt32 kCmdUpdateLater									= 'late';
static const UInt32 kCmdUpdateInstall								= 'inst';

static const NString kBundleIDMyNewApp								= "com.refnum.nano.nsoftwareupdate.mynewapp";
static const float   kProgressUnknown								= -1.0f;





//============================================================================
//		Internal class definition
//----------------------------------------------------------------------------
class NSUWindowInfo : public NCommandDialog {
public:
	virtual								~NSUWindowInfo(void) { };


	// Display the update
	UInt32								DisplayUpdate(const NDictionary &theInfo);


private:
	UInt32								mCommand;
	
	NIB_VIEW('icon', NIconView,			Icon);
	NIB_VIEW('info', NStaticText,		Info);
	NIB_VIEW('rnot', NHIWebView,		ReleaseNotes);
	NIB_VIEW('rbox', NShadingView,		ReleaseBox);
};





//============================================================================
//		Internal class definition
//----------------------------------------------------------------------------
class NSUWindowProgress : public NCommandDialog {
public:
	virtual								~NSUWindowProgress(void) { };


	// Run/stop a task
	OSStatus							RunTask(void);
	void								StopTask(OSStatus theErr);


	// Set the status text
	void								SetStatus(const NString &theValue);


	// Set the info text
	void								SetInfo(const NString &theValue);


	// Get/set the progress bar
	NProgressBar						*GetProgress(void);
	void								SetProgress(float theValue);


private:
	NIB_VIEW('stat', NStaticText,		Status);
	NIB_VIEW('info', NStaticText,		Info);
	NIB_VIEW('prog', NProgressBar,		Progress);
};





//============================================================================
//		NSUWindowInfo::DisplayUpdate : Display an update to the user.
//----------------------------------------------------------------------------
UInt32 NSUWindowInfo::DisplayUpdate(const NDictionary &theInfo)
{	NCFBundle	appBundle;



	// Prepare the window
	mIcon->SetIcon(appBundle.GetInfoString(kCFBundleIconFileKey));

	NHIViewUtilities::ReplaceTextValue(mInfo, kTokenAppName,    theInfo.GetValueString(kNanoAppNameKey));
	NHIViewUtilities::ReplaceTextValue(mInfo, kTokenVersionOld, theInfo.GetValueString(kNanoVersionOldKey));
	NHIViewUtilities::ReplaceTextValue(mInfo, kTokenVersionNew, theInfo.GetValueString(kNanoVersionNewKey));

	mReleaseNotes->SetURL(theInfo.GetValueString(kItemUpdateInfoKey));
	mReleaseNotes->SetOptions(kHIViewOptionWebViewMakeLinksExternal,
							  kHIViewOptionWebViewUpdateWindowTitle |
							  kHIViewOptionWebViewUpdateWindowIcon);



	// Run the dialog
	SetCloseCommand(kCmdUpdateLater);
	
	return(WaitForCommand());
}





//============================================================================
//		NSUWindowProgress::RunTask : Run a task.
//----------------------------------------------------------------------------
OSStatus NSUWindowProgress::RunTask(void)
{	OSStatus	theErr;
	UInt32		theCmd;



	// Run the dialog
	theCmd = WaitForCommand();
	theErr = (theCmd == kHICommandCancel) ? (OSStatus) userCanceledErr : (OSStatus) theCmd;

	return(theErr);
}





//============================================================================
//		NSUWindowProgress::StopTask : Stop a task.
//----------------------------------------------------------------------------
void NSUWindowProgress::StopTask(OSStatus theErr)
{


	// Stop the task
	SetCloseCommand(theErr);
	CommandClose();
}





//============================================================================
//		NSUWindowProgress::SetStatus : Set the status text.
//----------------------------------------------------------------------------
void NSUWindowProgress::SetStatus(const NString &theValue)
{


	// Set the status text
	mStatus->SetTextValue(theValue);
}





//============================================================================
//		NSUWindowProgress::SetInfo : Set the info text.
//----------------------------------------------------------------------------
void NSUWindowProgress::SetInfo(const NString &theValue)
{


	// Set the info text
	mInfo->SetTextValue(theValue);
}





//============================================================================
//		NSUWindowProgress::GetProgress : Get the progress bar.
//----------------------------------------------------------------------------
NProgressBar *NSUWindowProgress::GetProgress(void)
{


	// Get the progress bar
	return(&(*mProgress));
}





//============================================================================
//		NSUWindowProgress::SetProgress : Set the progress bar.
//----------------------------------------------------------------------------
void NSUWindowProgress::SetProgress(float theValue)
{


	// Set the progress bar
	if (NMathUtilities::AreEqual(theValue, kProgressUnknown))
		mProgress->SetIndeterminate(true);
	else
		{
		mProgress->SetPercentValue(theValue);
		mProgress->SetIndeterminate(false);
		
		if (NMathUtilities::AreEqual(theValue, 1.0f))
			mProgress->Render();
		}
}





//============================================================================
//		NSoftwareUpdate::NSoftwareUpdate : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NSoftwareUpdate::NSoftwareUpdate(void)
{


	// Initialize ourselves
	mPrefs       = NCFPreferences::GetPrefs();
	mState       = kUpdateFeed;
	mSilentCheck = true;

	mUpdateSheet = NULL;



	// Register for events
	mTask.AddListener(this);
	mClient.AddListener(this);
}





//============================================================================
//		NSoftwareUpdate::~NSoftwareUpdate : Destructor.
//----------------------------------------------------------------------------
NSoftwareUpdate::~NSoftwareUpdate(void)
{
}





//============================================================================
//		NSoftwareUpdate::ShouldCheck : Should we check for an update?
//----------------------------------------------------------------------------
bool NSoftwareUpdate::ShouldCheck(void)
{	NDate			timeNow, lastCheck, nextCheck;
	Float64			checkInterval;
	bool			shouldCheck;
	NCFBundle		appBundle;
	NString			theURL;



	// Get the state we need
	theURL        = appBundle.GetInfoString(kSUFeedURLKey);
	shouldCheck   = mPrefs->GetValueBoolean(kSUCheckEnabledKey);
	checkInterval = mPrefs->GetValueFloat64(kSUCheckIntervalKey);
	lastCheck     = mPrefs->GetValueDate(   kSULastCheckedKey);



	// Can we/do we need to perform a check?
	if (theURL.IsEmpty() || NMathUtilities::IsZero(checkInterval))
		shouldCheck = false;
	
	if (shouldCheck)
		{
		nextCheck   = lastCheck + checkInterval;
		shouldCheck = (timeNow >= nextCheck);
		}
	
	return(shouldCheck);
}





//============================================================================
//		NSoftwareUpdate::CheckForUpdates : Check for updates.
//----------------------------------------------------------------------------
OSStatus NSoftwareUpdate::CheckForUpdates(bool silentCheck)
{	NCFBundle		appBundle;
	NDictionary		theInfo;
	NString			theURL;



	// Get the state we need
	theURL = appBundle.GetInfoString(kSUFeedURLKey);
	if (theURL.IsEmpty())
		return(paramErr);



	// Prepare the URL
	//
	// Feed URLs can contain a single %s, which is replaced with the current version.
	//
	// This allows the feed to be handled by a script, rather than a static page, and
	// so dynamically select release notes for the user's upgrade path.
	AddVersionInfo(theInfo);

	theURL.Format(theURL.GetUTF8(), theInfo.GetValueString(kNanoVersionOldKey).GetUTF8());



	// Fetch the feed
	return(FeedBegin(theURL, silentCheck));
}





//============================================================================
//		NSoftwareUpdate::DoMessage : Handle messages.
//----------------------------------------------------------------------------
#pragma mark -
void NSoftwareUpdate::DoMessage(BroadcastMsg theMsg, void *msgData)
{	const ClientHTTPEvent	*httpEvent = (const ClientHTTPEvent *) msgData;



	// Handle the message
	switch (mState) {
		case kUpdateFeed:
			if (theMsg == kMsgClientHTTPEvent)
				FeedEvent(httpEvent);
			break;

		case kUpdateDownload:
			if (theMsg == kMsgClientHTTPEvent)
				DownloadEvent(httpEvent);
			break;
				
		case kUpdateVerify:
			if (theMsg == kMsgTaskTerminated)
				mUpdateSheet->StopTask(noErr);
			break;

		case kUpdateInstall:
			if (theMsg == kMsgTaskTerminated)
				mUpdateSheet->StopTask(noErr);
			break;
		}
}





//============================================================================
//		NSoftwareUpdate::FeedBegin : Fetch the feed.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NSoftwareUpdate::FeedBegin(const NCFURL &theURL, bool silentCheck)
{	OSStatus	theErr;



	// Check the network
	//
	// Silent checks are not allowed to activate the network.
	if (silentCheck && !NNetworkUtilities::IsHostReachable(theURL.GetHost()))
		return(kENETUNREACHErr);



	// Update our state
	mState        = kUpdateFeed;
	mSilentCheck  = silentCheck;
	mClientStream = NCFStream(0, NULL, kCFStreamWrite);



	// Submit the request
	//
	// If the user requests an additional check while one is in progress,
	// we silently eat the error and let the original request continue.
	theErr = mClient.SubmitRequest(theURL, &mClientStream);
	if (theErr == kEBUSYErr && !silentCheck)
		return(noErr);

	NN_ASSERT_NOERR(theErr);



	// Process the feed
	if (theErr != noErr)
		FeedComplete(theErr);
	
	return(theErr);
}





//============================================================================
//		NSoftwareUpdate::FeedEvent : Handle an HTTP event for the feed.
//----------------------------------------------------------------------------
void NSoftwareUpdate::FeedEvent(const ClientHTTPEvent *theEvent)
{	CFStreamError	streamErr;
	NData			theData;



	// Handle the event
	switch (theEvent->streamEvent) {
		case kCFStreamEventErrorOccurred:
			streamErr = theEvent->theStream->GetError();
			if (streamErr.domain == kCFStreamErrorDomainNetDB && streamErr.error == EAI_NODATA)
				streamErr.error = kEHOSTUNREACHErr;

			FeedComplete(streamErr.error);
			break;
			
		case kCFStreamEventEndEncountered:
			FeedComplete(noErr);
			break;

		default:
			break;
		}
}





//============================================================================
//		NSoftwareUpdate::FeedComplete : Process the feed.
//----------------------------------------------------------------------------
void NSoftwareUpdate::FeedComplete(OSStatus theErr)
{	NString				textMajor, textMinor;
	NStandardAlert		theAlert;
	NDate				timeNow;
	NXMLTree			theNode;
	NDictionary			theInfo;



	// Parse the feed
	if (theErr == noErr)
		theErr = theNode.Load(mClientStream.GetPropertyData(kCFStreamPropertyDataWritten));

	if (theErr == noErr)
		{
		theInfo = ParseFeed(theNode);
		if (!theInfo.IsValid())
			theErr = badFormat;
		}



	// Check for updates
	if (theErr == noErr)
		{
		if (IsUpdateAvailable(theInfo))
			DisplayUpdate(theInfo);
		
		else
			{
			textMajor = kStringSoftwareUpdateValidMajor;
			textMinor = kStringSoftwareUpdateValidMinor;
			}

		mPrefs->SetValueDate(kSULastCheckedKey, timeNow);
		}



	// Handle failure
	//
	// We don't try and interpret every network error, but distinguish between being
	// unable to contact the server vs an error once we download or parse the feed.
	if (theErr != noErr)
		{
		textMajor = kStringSoftwareUpdateServerLost;
		textMinor = kStringSoftwareUpdateErrorNetwork;

		if (theErr == kEHOSTUNREACHErr)
			textMajor = kStringSoftwareUpdateServerMissing;
		}



	// Inform the user
	if (textMajor.IsNotEmpty() && !mSilentCheck)
		NStandardAlert::DoAlert(NBundleString(textMajor, "", kNanoStrings),
								NBundleString(textMinor, "", kNanoStrings));
}





//============================================================================
//		NSoftwareUpdate::DownloadBegin : Download the update.
//----------------------------------------------------------------------------
OSStatus NSoftwareUpdate::DownloadBegin(void)
{	NString		theStatus, appName, appVersion;
	NCFURL		theURL;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mUpdateInfo.IsNotEmpty());
	NN_ASSERT(mUpdateSheet != NULL);



	// Get the state we need
	theURL     = mUpdateInfo.GetValueString(kItemEnclosureURLKey);
	appName    = mUpdateInfo.GetValueString(kNanoAppNameKey);
	appVersion = mUpdateInfo.GetValueString(kNanoVersionNewKey);



	// Update the sheet
	theStatus = NBundleString(kStringSoftwareUpdateProgressDownload, "", kNanoStrings);
	theStatus.Format(theStatus.GetUTF8(), appName.GetUTF8(), appVersion.GetUTF8());

	mUpdateSheet->SetStatus(theStatus);



	// Update our state
	mState        = kUpdateDownload;
	mUpdateFile   = NFileUtilities::GetTempFile(theURL.GetPathFile());
	mClientStream = NCFStream(mUpdateFile, kCFStreamWrite);



	// Download the update
	theErr = mClient.SubmitRequest(theURL, &mClientStream);
	if (theErr == noErr)
		theErr = mUpdateSheet->RunTask();

	return(DownloadComplete(theErr));
}





//============================================================================
//		NSoftwareUpdate::DownloadEvent : Handle an HTTP event for the download.
//----------------------------------------------------------------------------
void NSoftwareUpdate::DownloadEvent(const ClientHTTPEvent *theEvent)
{	NString		theInfo;



	// Handle the event
	switch (theEvent->streamEvent) {
		case kCFStreamEventOpenCompleted:
		case kCFStreamEventHasBytesAvailable:
			if (theEvent->progressPercentDone > 0.0)
				{
				theInfo = NTextUtilities::GetTransferInfo(theEvent->readContentDone,
														  theEvent->readContentLength,
														  theEvent->progressBytesPerSec,
														  theEvent->progressTimeLeft);

				mUpdateSheet->SetInfo(theInfo);
				mUpdateSheet->SetProgress(theEvent->progressPercentDone);
				}
			break;

		case kCFStreamEventErrorOccurred:
			mUpdateSheet->SetInfo(theInfo);
			mUpdateSheet->StopTask(networkErr);
			break;

		case kCFStreamEventEndEncountered:
			mUpdateSheet->SetInfo(theInfo);
			mUpdateSheet->StopTask(noErr);
			break;

		default:
			break;
		}
}





//============================================================================
//		NSoftwareUpdate::DownloadComplete : Process the download.
//----------------------------------------------------------------------------
OSStatus NSoftwareUpdate::DownloadComplete(OSStatus theErr)
{


	// Process the download
	if (theErr == noErr)
		theErr = VerifyBegin();



	// Clean up
	if (mClient.IsBusy())
		mClient.Cancel();
		
	if (mUpdateFile.Exists())
		mUpdateFile.Delete();
	
	return(theErr);
}





//============================================================================
//		NSoftwareUpdate::VerifyBegin : Verify the update.
//----------------------------------------------------------------------------
OSStatus NSoftwareUpdate::VerifyBegin(void)
{	NString		theStatus;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mUpdateInfo.IsNotEmpty());
	NN_ASSERT(mUpdateSheet != NULL);



	// Update the sheet
	theStatus = NBundleString(kStringSoftwareUpdateProgressVerify, "", kNanoStrings);
	theStatus.Format(theStatus.GetUTF8(), mUpdateFile.GetFileName().GetUTF8());

	mUpdateSheet->SetStatus(theStatus);
	mUpdateSheet->SetProgress(kProgressUnknown);



	// Update our state
	mState = kUpdateVerify;



	// Verify the update
	mTask.SetCommand("/usr/bin/env");
	mTask.SetArguments("md5", "-q", mUpdateFile.GetPath().GetUTF8(), NULL);
	
	theErr = mTask.Launch();
	if (theErr == noErr)
		theErr = mUpdateSheet->RunTask();
	
	return(VerifyComplete(theErr));
}





//============================================================================
//		NSoftwareUpdate::VerifyComplete : Finish verification.
//----------------------------------------------------------------------------
OSStatus NSoftwareUpdate::VerifyComplete(OSStatus theErr)
{	NString		actualMD5, expectedMD5;



	// Get the checksum
	if (theErr == noErr)
		{
		expectedMD5 = mUpdateInfo.GetValueString(kItemEnclosureMD5Key);
		actualMD5   = mTask.ReadOutput();
		actualMD5.TrimWhitespace();

		if (actualMD5 != expectedMD5)
			theErr = badCksmErr;
		}



	// Install the update
	if (theErr == noErr)
		theErr = InstallBegin();

	return(theErr);
}





//============================================================================
//		NSoftwareUpdate::InstallBegin : Install the update.
//----------------------------------------------------------------------------
OSStatus NSoftwareUpdate::InstallBegin(void)
{	NFile			mountPoint, oldApp, newApp, dmgApp, dstDir;
	NString			theStatus, appName, appVersion;
	NCFBundle		appBundle;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(mUpdateInfo.IsNotEmpty());
	NN_ASSERT(mUpdateSheet != NULL);



	// Get the state we need
	appName    = mUpdateInfo.GetValueString(kNanoAppNameKey);
	appVersion = mUpdateInfo.GetValueString(kNanoVersionNewKey);
	
	oldApp = appBundle.GetFile();
	dstDir = oldApp.GetParent();



	// Update the sheet
	theStatus = NBundleString(kStringSoftwareUpdateProgressInstall, "", kNanoStrings);
	theStatus.Format(theStatus.GetUTF8(), appName.GetUTF8(), appVersion.GetUTF8());

	mUpdateSheet->SetStatus(theStatus);
	mUpdateSheet->SetProgress(kProgressUnknown);



	// Mount the disk image
	mState     = kUpdateInstall;
	mountPoint = NFileUtilities::MountDMG(mUpdateFile, "-noverify", "-nobrowse", "-noautoopen", NULL);
	if (!mountPoint.Exists())
		return(nsDrvErr);



	// Find the update
	//
	// As a special case, to support apps built from a stationary project, we
	// look for the demo app's identifier if the real application wasn't found.
	dmgApp = NFileUtilities::FindBundle(mountPoint, appBundle.GetIdentifier());
	if (!dmgApp.Exists())
		dmgApp = NFileUtilities::FindBundle(mountPoint, kBundleIDMyNewApp);



	// Install the update
	theErr = dmgApp.Exists() ? (OSStatus) noErr : (OSStatus) fnfErr;
	if (theErr == noErr)
		{
		// Copy the file
		newApp = NFileUtilities::GetUniqueFile(dstDir, ".Software Update");

		mUpdateCopy.SetProgressBar(mUpdateSheet->GetProgress());
		theErr = mUpdateCopy.CopyObject(dmgApp, dstDir, newApp.GetFileName(), kFSFileOperationDefaultOptions | kFSFileOperationCanAuthorize);

		if (theErr == noErr)
			{
			while (mUpdateCopy.IsBusy())
				RunCurrentEventLoop(1.0f);
			}


		// Replace the old app
		if (theErr == noErr)
			theErr = NFileUtilities::MoveToTrash(oldApp);

		if (theErr == noErr)
			{
			appName = NFileUtilities::GetUniqueFile(dstDir, dmgApp.GetFileName()).GetFileName();
			theErr  = newApp.SetFileName(appName, true);
			}


		// Launch the new app, replacing this process
		if (theErr == noErr)
			NProcessUtilities::LaunchReplacement(newApp.GetPath());
		}



	// Clean up
	NFileUtilities::UnmountDMG(mountPoint);

	return(InstallComplete(theErr, newApp));
}





//============================================================================
//		NSoftwareUpdate::InstallComplete : Finish installation.
//----------------------------------------------------------------------------
OSStatus NSoftwareUpdate::InstallComplete(OSStatus theErr, const NFile &newApp)
{


	// Handle failure
	if (theErr != noErr && newApp.Exists())
		NFileUtilities::MoveToTrash(newApp);
	
	return(theErr);
}





//============================================================================
//		NSoftwareUpdate::ParseFeed : Parse the feed.
//----------------------------------------------------------------------------
NDictionary NSoftwareUpdate::ParseFeed(const NXMLTree &theNode)
{	NXMLTree		tmpNode, theChild;
	UInt32			n, numChildren;
	NArray			theItems;
	NDictionary		theItem;



	// Parse the items
	tmpNode = theNode.GetChild("rss").GetChild("channel");
	
	if (tmpNode.IsValid())
		{
		numChildren = tmpNode.GetChildCount();
		for (n = 0; n < numChildren; n++)
			{
			theChild = tmpNode.GetChild(n);

			if (theChild.IsElement("item"))
				theItems.AppendValueDictionary(ParseItem(theChild));
			}
		}



	// Get the latest release
	if (!theItems.IsEmpty())
		{
		theItems.Sort(SortByVersion);
		theItem = theItems.GetValueDictionary(0);

		AddVersionInfo(theItem);
		}

	return(theItem);
}





//============================================================================
//		NSoftwareUpdate::ParseItem : Parse an item.
//----------------------------------------------------------------------------
NDictionary NSoftwareUpdate::ParseItem(const NXMLTree &theNode)
{	UInt32			n, numChildren;
	NXMLTree		theChild;
	NDictionary		theItem;



	// Validate our parameters
	NN_ASSERT(theNode.IsElement("item"));



	// Parse the item
	numChildren = theNode.GetChildCount();
	
	for (n = 0; n < numChildren; n++)
		{
		theChild = theNode.GetChild(n);

		if (theChild.IsElement("enclosure"))
			{
			theItem.SetValueString(kItemEnclosureURLKey,  theChild.GetElementAttribute("url"));
			theItem.SetValueString(kItemEnclosureSizeKey, theChild.GetElementAttribute("length"));
			}
			
		else if (theChild.IsElement("pubDate"))
			theItem.SetValueDate(  theChild.GetData(), theChild.GetElementContents());

		else if (theChild.IsElement())
			theItem.SetValueString(theChild.GetData(), theChild.GetElementContents());
		}

	return(theItem);
}





//============================================================================
//		NSoftwareUpdate::IsUpdateAvailable : Is an update available?
//----------------------------------------------------------------------------
bool NSoftwareUpdate::IsUpdateAvailable(const NDictionary &theInfo)
{	NString			appVersion, newVersion;
	NCFBundle		appBundle;



	// Get the state we need
	appVersion = appBundle.GetInfoString(  kCFBundleVersionKey);
	newVersion =  theInfo.GetValueString(kItemUpdateVersionKey);



	// Check for an update
	//
	// Skipping versions is only supported for silent checks; we assume that normal
	// checks are in response to a user action and so all updates should be shown.
	if (mSilentCheck && mPrefs->GetValueString(kSUSkipVersionKey) == newVersion)
		return(false);

	return(NSystemUtilities::CompareVersions(newVersion, appVersion) == kCFCompareGreaterThan);
}





//============================================================================
//		NSoftwareUpdate::DisplayUpdate : Display an update.
//----------------------------------------------------------------------------
void NSoftwareUpdate::DisplayUpdate(const NDictionary &theInfo)
{	NSUWindowInfo	*theWindow;
	UInt32			theCmd;
	


	// Display the update
	theWindow = CreateController<NSUWindowInfo>(kNanoNibFile, "SU Update");
	theCmd    = theWindow->DisplayUpdate(theInfo);



	// Handle the command
	switch (theCmd) {
		case kCmdUpdateSkip:
			mPrefs->SetValueString(kSUSkipVersionKey, theInfo.GetValueString(kItemUpdateVersionKey));
			break;

		case kCmdUpdateLater:
			// Do nothing - will try again on next launch/check
			break;
		
		case kCmdUpdateInstall:
			if (theInfo.GetValueString(kItemSelfInstallKey).Contains("true", kCFCompareCaseInsensitive))
				InstallUpdate(theInfo, theWindow);
			else
				NLaunchServicesUtilities::OpenURL(theInfo.GetValueString(kItemEnclosureURLKey));
			break;
		
		default:
			NN_LOG("Unknown command: %d", theCmd);
			break;
		}



	// Clean up
	delete theWindow;
}





//============================================================================
//		NSoftwareUpdate::InstallUpdate : Install an update.
//----------------------------------------------------------------------------
void NSoftwareUpdate::InstallUpdate(const NDictionary &theInfo, NCommandDialog *theWindow)
{	NString			textMajor, textMinor;
	OSStatus		theErr;



	// Create the sheet
	mUpdateInfo  = theInfo;
	mUpdateSheet = CreateController<NSUWindowProgress>(kNanoNibFile, "SU Progress");

	InitCursor();



	// Install the update
	theWindow->BeginSheet(mUpdateSheet);
	theErr = DownloadBegin();

	if (theWindow->GetWindow()->HasSheet())
		theWindow->EndSheet();
	
	mUpdateSheet = NULL;

	if (theErr == noErr || theErr == userCanceledErr)
		return;



	// Handle failure
	switch (theErr) {
		case networkErr:
			textMajor = NBundleString(kStringSoftwareUpdateServerLost,   "", kNanoStrings);
			textMinor = NBundleString(kStringSoftwareUpdateErrorNetwork, "", kNanoStrings);
			break;

		case badCksmErr:
			textMajor = NBundleString(kStringSoftwareUpdateFailed,        "", kNanoStrings);
			textMinor = NBundleString(kStringSoftwareUpdateErrorChecksum, "", kNanoStrings);
			break;

		case nsDrvErr:
			textMajor = NBundleString(kStringSoftwareUpdateFailed,        "", kNanoStrings);
			textMinor = NBundleString(kStringSoftwareUpdateErrorMounting, "", kNanoStrings);
			break;
		
		default:
			textMajor = NBundleString(kStringSoftwareUpdateFailed,     "", kNanoStrings);
			textMinor = NBundleString(kStringSoftwareUpdateErrorTypeN, "", kNanoStrings);
			textMinor.Format(textMinor.GetUTF8(), theErr);
			break;
		}

	theWindow->DisplayAlert(textMajor, textMinor, false, kDefaultAlertText, "", "", kAlertStopAlert);
	theWindow->WaitForCommand();
}





//============================================================================
//		NSoftwareUpdate::AddVersionInfo : Add the version info.
//----------------------------------------------------------------------------
void NSoftwareUpdate::AddVersionInfo(NDictionary &theInfo)
{	NString			nameDisplay, nameBundle, newVerName, newVerBuild, oldVerName, oldVerBuild;
	NCFBundle		appBundle;



	// Get the state we need
	nameDisplay = appBundle.GetInfoString(kCFBundleDisplayNameKey);
	nameBundle  = appBundle.GetInfoString(kCFBundleNameKey);

	oldVerName  = appBundle.GetInfoString(kCFBundleShortVersionStringKey);
	oldVerBuild = appBundle.GetInfoString(kCFBundleVersionKey);

	newVerName  = theInfo.GetValueString(kItemUpdateVersionNameKey);
	newVerBuild = theInfo.GetValueString(kItemUpdateVersionKey);



	// Add the version info
	theInfo.SetValueString(kNanoAppNameKey,    nameDisplay.IsNotEmpty() ? nameDisplay : nameBundle);
	theInfo.SetValueString(kNanoVersionOldKey, GetUserVersion(oldVerName, oldVerBuild));
	theInfo.SetValueString(kNanoVersionNewKey, GetUserVersion(newVerName, newVerBuild));
}





//============================================================================
//		NSoftwareUpdate::GetUserVersion : Get a user-visible version.
//----------------------------------------------------------------------------
NString NSoftwareUpdate::GetUserVersion(const NString &verName, const NString &verBuild)
{	NString		theValue;



	// Get the user-visible version
	//
	// Version numbers come in two forms - a named version for user display, and a build
	// number (although these are somewhat fuzzy, and many apps simply use the "build
	// number" to hold their entire version).
	//
	// Although we always use the build version for comparisons, for display to the user
	// we combine the two into a single string that uniquely identifies a version.
	if (verName.IsEmpty())
		theValue = verBuild;

	else if (verName == verBuild)
		theValue = verName;

	else
		theValue.Format("%s (%s)", verName.GetUTF8(), verBuild.GetUTF8());
	
	return(theValue);
}





//============================================================================
//		NSoftwareUpdate::SortByVersion : Sort items by version.
//----------------------------------------------------------------------------
CFComparisonResult NSoftwareUpdate::SortByVersion(const void *value1, const void *value2, void */*userData*/)
{	CFStringRef		cfString1, cfString2;



	// Sort by version number
	cfString1 = (CFStringRef) CFDictionaryGetValue((CFDictionaryRef) value1, kItemUpdateVersionKey);
	cfString2 = (CFStringRef) CFDictionaryGetValue((CFDictionaryRef) value2, kItemUpdateVersionKey);

	return(NSystemUtilities::CompareVersions(cfString1, cfString2));
}
