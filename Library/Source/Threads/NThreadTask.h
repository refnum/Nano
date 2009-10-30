/*	NAME:
		NThreadTask.h

	DESCRIPTION:
		Thread pool task.
	
	COPYRIGHT:
        Copyright (c) 2006-2007, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTHREADTASK_HDR
#define NTHREADTASK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NString.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NThreadTask;

typedef std::vector<NThreadTask*>								NThreadTaskList;
typedef NThreadTaskList::iterator								NThreadTaskListIterator;
typedef NThreadTaskList::const_iterator							NThreadTaskListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThreadTask {
public:
										NThreadTask(void);
	virtual								~NThreadTask(void);


	// Get/set the name
	NString								GetName(void) const;
	void								SetName(const NString &theName);


	// Get/set the stopped state
	bool								IsStopped(void) const;
	void								Stop(void);


	// Get/set the priority
	float								GetPriority(void) const;
	void								SetPriority(float theValue);


	// Get/set the time stamp
	EventTime							GetTimeStamp(void) const;
	void								SetTimeStamp(EventTime theValue);


	// Run the task
	virtual void						Run(void) = 0;


private:
	NString								mName;
	bool								mStopped;
	float								mPriority;
	EventTime							mTimeStamp;
};









#endif // NThreadTask_HDR


