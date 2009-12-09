/*	NAME:
		NTargetSystem.h

	DESCRIPTION:
		System support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_SYSTEM_HDR
#define NTARGET_SYSTEM_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NLocale.h"
#include "NVariant.h"
#include "NFile.h"
#include "NTask.h"





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


	// Tasks
	static TaskInfo						TaskCreate(const NString &theCmd, const NStringList &theArgs);
	static void							TaskDestroy(    TaskInfo &theTask);
	static void							TaskUpdate(     TaskInfo &theTask);
	static NString						TaskRead(       TaskInfo &theTask, bool stdErr=false);
	static void							TaskWrite(const TaskInfo &theTask, const NString &theText);
	static void							TaskWait( const TaskInfo &theTask, NTime theDelay);
	static void							TaskKill( const TaskInfo &theTask);


	// Locale
	static NVariant						GetLocaleValue(      const NString &theID, const NString &theKey);
	static NBroadcaster				   *GetLocaleBroadcaster(const NString &theID);
};





#endif // NTARGET_SYSTEM_HDR
