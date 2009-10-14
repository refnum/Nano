/*	NAME:
		NSoftwareUpdate.h

	DESCRIPTION:
		Software Update client.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSOFTWAREUPDATE_HDR
#define NSOFTWAREUPDATE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFPreferences.h"
#include "NFileOperation.h"
#include "NCommandDialog.h"
#include "NMsgListener.h"
#include "NXMLTree.h"
#include "NTask.h"
#include "NClientHTTP.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Commands
static const OSType kCmdSoftwareUpdateCheckNow						= 'supd';
static const OSType kCmdSoftwareUpdateIntervalNever					= 'snev';
static const OSType kCmdSoftwareUpdateIntervalDaily					= 'sday';
static const OSType kCmdSoftwareUpdateIntervalWeekly				= 'swek';
static const OSType kCmdSoftwareUpdateIntervalMonthly				= 'smon';


// Preference keys
static const NString kSUCheckEnabledKey								= "SUCheckEnabled";
static const NString kSUCheckIntervalKey							= "SUCheckInterval";
static const NString kSULastCheckedKey								= "SULastChecked";
static const NString kSUSkipVersionKey								= "SUSkipVersion";


// States
typedef enum {
	kUpdateFeed,
	kUpdateDownload,
	kUpdateVerify,
	kUpdateInstall
} SUState;





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NSUWindowProgress;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSoftwareUpdate : public NMsgListener {
public:
										NSoftwareUpdate( void);
	virtual								~NSoftwareUpdate(void);


	// Should we check for an update?
	bool								ShouldCheck(void);


	// Check for updates
	//
	// A check can be performed in "silent" or "normal" modes.
	//
	// In silent mode, typically used for scheduled checks, errors fetching or
	// processing the feed will be suppressed and the user will only be informed
	// if an update was detected.
	//
	// In normal mode, typically due to user action, the user will be informed
	// of any errors and of the presence or absence of any updates.
	OSStatus							CheckForUpdates(bool silentCheck);


protected:
	// Handle messages
	void								DoMessage(BroadcastMsg theMsg, void *msgData);


private:
	OSStatus							FeedBegin(const NCFURL &theURL, bool silentCheck);
	void								FeedEvent(const ClientHTTPEvent *theEvent);
	void								FeedComplete(OSStatus theErr);

	OSStatus							DownloadBegin(void);
	void								DownloadEvent(const ClientHTTPEvent *theEvent);
	OSStatus							DownloadComplete(OSStatus theErr);
	
	OSStatus							VerifyBegin(void);
	OSStatus							VerifyComplete(OSStatus theErr);
	
	OSStatus							InstallBegin(void);
	OSStatus							InstallComplete(OSStatus theErr, const NFile &newApp);

	NDictionary							ParseFeed(const NXMLTree &theNode);
	NDictionary							ParseItem(const NXMLTree &theNode);

	bool								IsUpdateAvailable(const NDictionary &theInfo);
	void								DisplayUpdate(    const NDictionary &theInfo);
	void								InstallUpdate(    const NDictionary &theInfo, NCommandDialog *theWindow);

	void								DisplayError(NCommandDialog *theWindow, OSStatus theErr);

	void								AddVersionInfo(NDictionary &theInfo);
	NString								GetUserVersion(const NString &verName, const NString &verBuild);

	static CFComparisonResult			SortByVersion(const void *value1, const void *value2, void *userData);


private:
	NCFPreferences						*mPrefs;
	SUState								mState;
	bool								mSilentCheck;
	
	NTask								mTask;
	NClientHTTP							mClient;
	NCFStream							mClientStream;

	NDictionary							mUpdateInfo;
	NFile								mUpdateFile;
	NSUWindowProgress					*mUpdateSheet;
	NFileOperation						mUpdateCopy;
};





#endif // NSOFTWAREUPDATE_HDR

