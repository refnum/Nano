/*	NAME:
		NFileOperation.h

	DESCRIPTION:
		File operation.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NFILEOPERATION_HDR
#define NFILEOPERATION_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFile.h"
#include "NAuthorization.h"
#include "NMsgBroadcaster.h"
#include "NProgressBar.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgFileOperationStatus						= 'fops';


// Flags
//
// File operation flags are OptionBits, and so we define some additional
// flags in a currently unused portion of this bitfield.
//
// These values may need to change, and should not be archived to disk.
static const OptionBits kFSFileOperationCanAuthorize					= (1 << 31);


// Status keys
//
// Additional status keys are defined in Files.h.
static const NString kFSOperationCurrentItemKey							= "nano:FSOperationCurrentItem";
static const NString kFSOperationCurrentStageKey						= "nano:FSOperationCurrentStage";
static const NString kFSOperationCurrentStatusKey						= "nano:FSOperationCurrentStatus";





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NFileOperation;


// Message data for kMsgFileOperationStatus
//
// The status dictionary contains standard kFSOperationXXX values.
typedef struct {
	const NFileOperation	*theOperation;
	NDictionary				statusInfo;
} FileOperationStatus;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NFileOperation : public NMsgBroadcaster {
public:
										NFileOperation(void);
	virtual								~NFileOperation(void);


	// Is an operation in progress?
	bool								IsBusy(void) const;


	// Get the current status
	//
	// Returns a dictionary of kFSOperationXXX values describing the status of the
	// current operation, or an empty dictionary if no operation is in progress.
	NDictionary							GetStatus(void) const;


	// Perform an operation
	OSStatus							CopyObject(const NFile &srcObject, const NFile &dstDir, const NString &dstName, OptionBits theFlags=kFSFileOperationDefaultOptions);
	OSStatus							MoveObject(const NFile &srcObject, const NFile &dstDir, const NString &dstName, OptionBits theFlags=kFSFileOperationDefaultOptions);


	// Cancel the current operation
	void								Cancel(void);


	// Get/set the progress bar
	//
	// A progress bar can be assigned to provide feedback during the operation.
	NProgressBar						*GetProgressBar(void) const;
	void								SetProgressBar(NProgressBar *theView);


private:
	OSStatus							CreateAuthorization(OptionBits &theFlags, const NFile &dstDir);
	void								DestroyAuthorization(void);

	OSStatus							CreateOperation( void);
	void								DestroyOperation(void);
	OSStatus							PerformOperation(bool					isCopy,
															const NFile			&srcObject,
															const NFile			&dstDir,
															const NString		&dstName,
															OptionBits			theFlags);

	OSStatus							UpdatePermissions(const NFile &theDir, mode_t theMode);
	void								UpdateProgressBar(const FileOperationStatus &theStatus);

	NDictionary							GetStatusDictionary(const NFile &theFile, FSFileOperationStage theStage, OSStatus theErr, CFDictionaryRef cfStatus) const;

	static void							StatusCallback(FSFileOperationRef		fileOp,
														const FSRef				*currentItem,
														FSFileOperationStage	currentStage,
														OSStatus				currentErr,
														CFDictionaryRef			cfStatus,
														void					*contextInfo);


private:
	FSFileOperationClientContext		mContext;
	NCFObject							mOperation;

	EventTime							mStartTime;
	NProgressBar						*mProgressBar;

	NAuthorization						mAuthorization;
	mode_t								mAuthPerm;
	NFile								mAuthDir;
};





#endif // NFILEOPERATION_HDR
