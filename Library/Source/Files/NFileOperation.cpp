/*	NAME:
		NFileOperation.cpp

	DESCRIPTION:
		File operation.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NFileOperation.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kToolChmod										= "/bin/chmod";

static const CFTimeInterval kStatusRefresh							= 1.0f;
static const EventTime      kProgressDelay							= 2.0f;





//============================================================================
//		NFileOperation::NFileOperation : Constructor.
//----------------------------------------------------------------------------
NFileOperation::NFileOperation(void)
{


	// Initialize ourselves
	memset(&mContext, 0x00, sizeof(mContext));
	mContext.info = this;

	mProgressBar  = NULL;
	
	mAuthPerm = 0;
}





//============================================================================
//		NFileOperation::~NFileOperation : Destructor.
//----------------------------------------------------------------------------
NFileOperation::~NFileOperation(void)
{


	// Clean up
	if (IsBusy())
		Cancel();
}





//============================================================================
//		NFileOperation::IsBusy : Is an operation in progress?
//----------------------------------------------------------------------------
bool NFileOperation::IsBusy(void) const
{


	// Check our state
	return(mOperation.IsValid());
}





//============================================================================
//		NFileOperation::GetStatus : Get the current status.
//----------------------------------------------------------------------------
NDictionary NFileOperation::GetStatus(void) const
{	OSStatus					theErr, currentStatus;
	void						*contextInfo;
	FSFileOperationStage		currentStage;
	FSRef						currentItem;
	NDictionary					theStatus;
	CFDictionaryRef				cfStatus;



	// Check our state
	if (!IsBusy())
		return(theStatus);



	// Get the status
	contextInfo = NULL;
	theErr      = FSFileOperationCopyStatus(mOperation, &currentItem, &currentStage, &currentStatus, &cfStatus, &contextInfo);

	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(contextInfo == this);



	// Construct the dictionary
	if (theErr == noErr && cfStatus != NULL)
		{
		theStatus = GetStatusDictionary(currentItem, currentStage, currentStatus, cfStatus);
		CFSafeRelease(cfStatus);
		}
	
	return(theStatus);
}





//============================================================================
//		NFileOperation::CopyObject : Perform a copy operation.
//----------------------------------------------------------------------------
OSStatus NFileOperation::CopyObject(const NFile &srcObject, const NFile &dstDir, const NString &dstName, OptionBits theFlags)
{


	// Perform the operation
	return(PerformOperation(true, srcObject, dstDir, dstName, theFlags));
}





//============================================================================
//		NFileOperation::MoveObject : Perform a move operation.
//----------------------------------------------------------------------------
OSStatus NFileOperation::MoveObject(const NFile &srcObject, const NFile &dstDir, const NString &dstName, OptionBits theFlags)
{


	// Perform the operation
	return(PerformOperation(false, srcObject, dstDir, dstName, theFlags));
}





//============================================================================
//		NFileOperation::Cancel : Cancel the current operation.
//----------------------------------------------------------------------------
void NFileOperation::Cancel(void)
{


	// Validate our state
	NN_ASSERT(IsBusy());



	// Destroy the operation
	DestroyOperation();
	DestroyAuthorization();
}





//============================================================================
//		NFileOperation::GetProgressBar : Get the progress bar.
//----------------------------------------------------------------------------
NProgressBar *NFileOperation::GetProgressBar(void) const
{


	// Get the progress bar
	return(mProgressBar);
}





//============================================================================
//		NFileOperation::SetProgressBar : Set the progress bar.
//----------------------------------------------------------------------------
void NFileOperation::SetProgressBar(NProgressBar *theView)
{


	// Set the progress bar
	mProgressBar = theView;
}





//============================================================================
//		NFileOperation::CreateAuthorization : Create the authorization.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NFileOperation::CreateAuthorization(OptionBits &theFlags, const NFile &dstDir)
{	NString			theMode;
	mode_t			dstPerm;
	bool			canAuth;
	OSStatus		theErr;



	// Process the flags
	//
	// Since we use some currently-unused bits in the operation flags, we must
	// strip our values off after testing to ensure they don't pass through to
	// a future system API which may use our values.
	canAuth   = (theFlags & kFSFileOperationCanAuthorize);
	theFlags &= ~kFSFileOperationCanAuthorize;



	// Authorize ourselves if necessary
	//
	// Authorization support is currently limited to ensuring that files can be written to
	// directories that are not writeable by the current user (we assume that the user has
	// permission to read the source).
	//
	// This is done by making the destination writeable for the duration of the operation (a
	// completely brute force solution - however a more fine-grained implementation could be
	// added here).
	//
	//
	// Unfortunately (rdar://4872308) FSCopyObject/FSMoveObject do not provide support for
	// authorization, and the Mac OS X authorization model is that of spawning a helper tool
	// with higher privileges rather than raising the privileges of the running application.
	//
	// Ideally FSCopyObject/FSMoveObject would support an optional 'can authorize' flag for
	// use in apps with a connection to the window server, allowing them to acquire whatever
	// rights are necessary to ensure the operation does not fail due to a lack of privileges.
	//
	//
	// Rather than use FSCopyObject/FSMoveObject we could simply spawn a ditto/mv with higher
	// privileges, however these tools do not emit progress information.
	//
	// Alternatively we could provide a helper tool that used FSCopyObject/FSMoveObject, and
	// emitted progress info to stdout, but this has additional drawbacks (unuseable for a
	// library solution, and would need a primitive IPC to submit cancel requests to the tool).
	theErr = noErr;
	
	if (canAuth)
		{
		dstPerm = dstDir.GetPermissions();
		if (!(dstPerm & S_IWUSR))
			{
			mAuthPerm = dstPerm;
			mAuthDir  = dstDir;
			
			theErr = UpdatePermissions(mAuthDir, mAuthPerm | S_IRWXU | S_IRWXG | S_IRWXO);
			if (theErr != noErr)
				{
				mAuthPerm = 0;
				mAuthDir  = NFile();
				}
			}
		}

	return(theErr);
}





//============================================================================
//		NFileOperation::DestroyAuthorization : Destroy the authorization.
//----------------------------------------------------------------------------
void NFileOperation::DestroyAuthorization(void)
{	OSStatus	theErr;



	// Check our state
	if (!mAuthorization.IsValid())
		return;



	// Restore the permissions
	if (mAuthDir.IsDirectory())
		{
		theErr = UpdatePermissions(mAuthDir, mAuthPerm);
		NN_ASSERT_NOERR(theErr);
		}



	// Clean up
	mAuthorization.Deauthorize();

	mAuthPerm = 0;
	mAuthDir  = NFile();
}





//============================================================================
//		NFileOperation::CreateOperation : Create the operation.
//----------------------------------------------------------------------------
OSStatus NFileOperation::CreateOperation(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(!IsBusy());



	// Create the operation
	if (!mOperation.Set(FSFileOperationCreate(kCFAllocatorNano)))
		return(memFullErr);

	theErr = FSFileOperationScheduleWithRunLoop(mOperation, NSystemUtilities::GetMainRunLoop(), kCFRunLoopCommonModes);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NFileOperation::DestroyOperation : Destroy the operation.
//----------------------------------------------------------------------------
void NFileOperation::DestroyOperation(void)
{	OSStatus	theErr;



	// Check our state
	if (!mOperation.IsValid())
		return;
	
	
	
	// Destroy the operation
	theErr = FSFileOperationCancel(mOperation);
	NN_ASSERT_NOERR(theErr);

	theErr = FSFileOperationUnscheduleFromRunLoop(mOperation, NSystemUtilities::GetMainRunLoop(), kCFRunLoopCommonModes);
	NN_ASSERT_NOERR(theErr);



	// Update our state
	mOperation.Set(NULL);
}





//============================================================================
//		NFileOperation::PerformOperation : Perform an operation.
//----------------------------------------------------------------------------
OSStatus NFileOperation::PerformOperation(bool isCopy, const NFile &srcObject, const NFile &dstDir, const NString &dstName, OptionBits theFlags)
{	FSRef			srcFSRef, dstFSRef;
	CFStringRef		dstNameCF;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(!IsBusy());



	// Get the state we need
	srcFSRef  = srcObject.GetFSRef();
	dstFSRef  = dstDir.GetFSRef();
	dstNameCF = dstName.IsNotEmpty() ? (CFStringRef) dstName : NULL;



	// Create the operation
	theErr = CreateAuthorization(theFlags, dstDir);
	if (theErr == noErr)
		theErr = CreateOperation();

	NN_ASSERT_NOERR(theErr);



	// Perform the operation
	if (theErr == noErr)
		{
		mStartTime = GetCurrentEventTime();
		
		if (isCopy)
			theErr = FSCopyObjectAsync(mOperation, &srcFSRef, &dstFSRef, dstNameCF, theFlags, StatusCallback, kStatusRefresh, &mContext);
		else
			theErr = FSMoveObjectAsync(mOperation, &srcFSRef, &dstFSRef, dstNameCF, theFlags, StatusCallback, kStatusRefresh, &mContext);

		NN_ASSERT_NOERR(theErr);
		}



	// Handle failure
	if (theErr != noErr)
		{
		DestroyOperation();
		DestroyAuthorization();
		}

	return(theErr);
}





//============================================================================
//		NFileOperation::UpdatePermissions : Update a directory's permissions.
//----------------------------------------------------------------------------
OSStatus NFileOperation::UpdatePermissions(const NFile &theDir, mode_t theMode)
{	NString		thePath, modeString;
	OSStatus	theErr;



	// Validate our parameters and state
	NN_ASSERT(theDir.IsDirectory());
	NN_ASSERT(mAuthorization.IsValid());



	// Update the permissions
	thePath = theDir.GetPath();
	modeString.Format("%o", theMode);
		
	theErr = mAuthorization.ExecuteTool(kToolChmod, NULL, modeString.GetUTF8(), thePath.GetUTF8(), NULL);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NFileOperation::UpdateProgressBar : Update the progress bar.
//----------------------------------------------------------------------------
void NFileOperation::UpdateProgressBar(const FileOperationStatus &theStatus)
{	float					valueDone, valueTotal;
	NString					keyDone, keyTotal;
	FSFileOperationStage	theStage;



	// Check our state
	if (mProgressBar == NULL)
		return;



	// Get the state we need
	if (theStatus.statusInfo.HasKey(kFSOperationTotalBytesKey))
		{
		keyDone  = kFSOperationBytesCompleteKey;
		keyTotal = kFSOperationTotalBytesKey;
		}
	else
		{
		keyDone  = kFSOperationObjectsCompleteKey;
		keyTotal = kFSOperationTotalObjectsKey;
		}

	theStage   = theStatus.statusInfo.GetValueUInt32(kFSOperationCurrentStageKey);
	valueDone  = theStatus.statusInfo.GetValueFloat32(keyDone);
	valueTotal = theStatus.statusInfo.GetValueFloat32(keyTotal);



	// Update the progress bar
	switch (theStage) {
		case kFSOperationStageUndefined:
		case kFSOperationStagePreflighting:
			mProgressBar->SetIndeterminate(true);
			break;
		
		case kFSOperationStageRunning:
			mProgressBar->SetIndeterminate(GetCurrentEventTime() < (mStartTime + kProgressDelay));
			mProgressBar->SetPercentValue(valueDone / valueTotal);
			break;
		
		case kFSOperationStageComplete:
			mProgressBar->SetPercentValue(1.0f);
			break;
		
		default:
			break;
		}
}





//============================================================================
//		NFileOperation::GetStatusDictionary : Get a status dictionary.
//----------------------------------------------------------------------------
NDictionary NFileOperation::GetStatusDictionary(const NFile &theFile, FSFileOperationStage theStage, OSStatus theErr, CFDictionaryRef cfStatus) const
{	NDictionary		theStatus(cfStatus);



	// Update the dictionary
	theStatus.SetValueString(kFSOperationCurrentItemKey,   theFile.GetPath());
	theStatus.SetValueUInt32(kFSOperationCurrentStageKey,  theStage);
	theStatus.SetValueUInt32(kFSOperationCurrentStatusKey, theErr);
	
	return(theStatus);
}





//============================================================================
//		NFileOperation::StatusCallback : Status callback
//----------------------------------------------------------------------------
void NFileOperation::StatusCallback(FSFileOperationRef		fileOp,
									const FSRef				*currentItem,
									FSFileOperationStage	currentStage,
									OSStatus				currentStatus,
									CFDictionaryRef			cfStatus,
									void					*contextInfo)
{	NFileOperation			*thisPtr = (NFileOperation *) contextInfo;
	FileOperationStatus		theStatus;



	// Validate our parameters
	NN_ASSERT(fileOp == thisPtr->mOperation);
	
	(void) fileOp;



	// Get the state we need
	theStatus.theOperation = thisPtr;
	theStatus.statusInfo   = thisPtr->GetStatusDictionary(*currentItem, currentStage, currentStatus, cfStatus);



	// Let everyone know
	thisPtr->UpdateProgressBar(theStatus);
	thisPtr->BroadcastMessage(kMsgFileOperationStatus, &theStatus);



	// Finish the operation
	if (currentStage == kFSOperationStageComplete)
		thisPtr->Cancel();
}


