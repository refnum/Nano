/*	NAME:
		NProcess.h

	DESCRIPTION:
		Process object.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPROCESS_HDR
#define NPROCESS_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NComparable.h"
#include "NDictionary.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Meta-constants
static const ProcessSerialNumber kInvalidPSN					= { 0, kNoProcess };
static const pid_t				 kInvalidPID					= UINT_MAX;


// Process flavors
static const UInt32 kFlavorClassic								= 0;
static const UInt32 kFlavorCarbon								= 2;
static const UInt32 kFlavorCocoa								= 4;


// Info dictionary keys
//
// Although these keys are documented in Processes.h, we provide a unified naming
// convention since the existing constants are a mixture of styles (rdar://4888011).
static const NString kProcessBundleExecutableKey				= "CFBundleExecutable";
static const NString kProcessBundleIdentifierKey				= "CFBundleIdentifier";
static const NString kProcessBundleNameKey						= "CFBundleName";
static const NString kProcessBundlePathKey						= "BundlePath";
static const NString kProcessFileCreatorKey						= "FileCreator";
static const NString kProcessFileTypeKey						= "FileType";
static const NString kProcessFlavorKey							= "Flavor";
static const NString kProcessIsBackgroundOnlyKey				= "LSBackgroundOnly";
static const NString kProcessIsHiddenKey						= "IsHiddenAttr";
static const NString kProcessNameKey							= "nano:Name";
static const NString kProcessParentPIDKey						= "nano:ParentPID";
static const NString kProcessParentPSNKey						= "ParentPSN";
static const NString kProcessPIDKey								= "pid";
static const NString kProcessPSNKey								= "PSN";





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NProcess;

typedef std::vector<NProcess>									NProcessList;
typedef NProcessList::iterator									NProcessListIterator;
typedef NProcessList::const_iterator							NProcessListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NProcess : public NComparable {
public:
										NProcess(const ProcessSerialNumber &thePSN);
										NProcess(pid_t                      thePID);

										NProcess(void);
	virtual								~NProcess(void);


	// Is the process valid?
	bool								IsValid(void) const;


	// Get the parent
	NProcess							GetParent(void) const;


	// Get the info dictionary
	NDictionary							GetInfo(void) const;


	// Get/set the front-most state
	bool								IsAtFront(void) const;
	void								SetAtFront(OptionBits theFlags=0);


	// Get/set the visible state
	bool								IsVisible(void) const;
	void								SetVisible(bool isVisible);


	// Operators
										operator const ProcessSerialNumber*(void) const;
										operator       pid_t               (void) const;


protected:
	// Compare two objects
	CFComparisonResult					Compare(const NComparable &theObject) const;


private:
	pid_t								GetParentPID(void) const;
	

private:
	ProcessSerialNumber					mPSN;
};





#endif // NPROCESS_HDR
