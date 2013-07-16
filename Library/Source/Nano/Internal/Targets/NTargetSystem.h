/*	NAME:
		NTargetSystem.h

	DESCRIPTION:
		System support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_SYSTEM_HDR
#define NTARGET_SYSTEM_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NString.h"
#include "NLocale.h"
#include "NVariant.h"
#include "NImage.h"
#include "NFile.h"
#include "NTask.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Tasks
typedef enum {
	kTaskKill,
	kTaskInterrupt
} NTaskSignal;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetSystem {
public:
	// Emit a debug message
	static void							DebugLog(const char *theMsg);


	// Find a bundle
	//
	// Returns the application bundle if bundleID is empty.
	static NFile						FindBundle(const NString &bundleID);


	// Get the OS details
	static OSVersion					GetOSVersion(void);
	static NString						GetOSName(   void);


	// Get the process details
	static NString						GetProcessName(void);
	

	// Get the system details
	static UInt64						GetSystemCPU( void);
	static UInt64						GetSystemRAM( void);
	static NString						GetSystemArch(void);


	// Tasks
	static TaskInfo						TaskCreate(const NString &theCmd, const NStringList &theArgs);
	static void							TaskDestroy(     TaskInfo &theTask);
	static void							TaskUpdate(      TaskInfo &theTask);
	static NString						TaskRead(        TaskInfo &theTask, bool stdErr=false);
	static void							TaskWrite( const TaskInfo &theTask, const NString &theText);
	static void							TaskWait(  const TaskInfo &theTask, NTime waitFor);
	static void							TaskSignal(const TaskInfo &theTask, NTaskSignal theSignal);


	// Locale
	static NVariant						GetLocaleValue(      const NString &theID, const NString &theKey);
	static NBroadcaster				   *GetLocaleBroadcaster(const NString &theID);


	// Strings
	static NString						TransformString(const NString &theString, NStringTransform theTransform);


	// Images
	static NData						ImageEncode(const NImage &theImage, const NUTI &theType);
	static NImage						ImageDecode(const NData  &theData);
};





#endif // NTARGET_SYSTEM_HDR
