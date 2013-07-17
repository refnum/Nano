/*	NAME:
		NDebuggable.h

	DESCRIPTION:
		Mix-in class for debugging objects.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDEBUGGABLE_HDR
#define NDEBUGGABLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDebuggable {
public:
										NDebuggable(void);
	virtual							   ~NDebuggable(void);


protected:
	// Update the debugger summary
#if NN_DEBUG
	void								UpdateSummary(const char *theFormat, ...);
#endif


private:
#if NN_DEBUG
	std::string							mDebugSummary;
#endif
};





#endif // NDEBUGGABLE_HDR
