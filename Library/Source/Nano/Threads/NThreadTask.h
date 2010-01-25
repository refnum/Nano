/*	NAME:
		NThreadTask.h

	DESCRIPTION:
		Thread pool task.
	
	COPYRIGHT:
        Copyright (c) 2006-2010, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTHREADTASK_HDR
#define NTHREADTASK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NThreadTask;

typedef std::vector<NThreadTask*>									NThreadTaskList;
typedef NThreadTaskList::iterator									NThreadTaskListIterator;
typedef NThreadTaskList::const_iterator								NThreadTaskListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThreadTask {
public:
										NThreadTask(void);
	virtual							   ~NThreadTask(void);


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
	NTime								GetTimeStamp(void) const;
	void								SetTimeStamp(NTime theValue);


	// Run the task
	virtual void						Run(void) = 0;


private:
	NString								mName;
	bool								mStopped;
	float								mPriority;
	NTime								mTimeStamp;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThreadTaskFunctor : public NThreadTask {
public:
										NThreadTaskFunctor(const NFunctor &theFunctor);
	virtual							   ~NThreadTaskFunctor(void);


protected:
	// Run the task
	void								Run(void);


private:
	NFunctor							mFunctor;
};






#endif // NThreadTask_HDR


