/*	NAME:
		NDebuggable.cpp

	DESCRIPTION:
		Mix-in class for debugging objects.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDebuggable.h"





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
#if NN_DEBUG
static const NIndex kDefaultBufferSize								= 19;
#endif





//============================================================================
//		NDebuggable::NDebuggable : Constructor.
//----------------------------------------------------------------------------
NDebuggable::NDebuggable(void)
{
}





//============================================================================
//		NDebuggable::~NDebuggable : Destructor.
//----------------------------------------------------------------------------
NDebuggable::~NDebuggable(void)
{
}





#pragma mark protected
//============================================================================
//		NDebuggable::UpdateSummary : Update the debugger summary.
//----------------------------------------------------------------------------
#if NN_DEBUG
void NDebuggable::UpdateSummary(const char *theFormat, ...)
{	std::vector<char>		theBuffer;
	NIndex					theSize;
	va_list					argList;



	// Get the state we need
	theSize = kDefaultBufferSize;



	// Prepare the text
	while (true)
		{
		theBuffer.resize(theSize + 1);

		va_start(argList, theFormat);
		theSize = vsnprintf(&theBuffer[0], theBuffer.size(), theFormat, argList);
		va_end(argList);

		if (theSize < 0)
			{
			theBuffer.clear();
			theBuffer.push_back(0x00);
			break;
			}
		
		else if ((theSize+1) <= (NIndex) theBuffer.size())
			break;
		}



	// Update the summary
	//
	// Since std::string may use copy on write, we need to go through a temporary
	// buffer to ensure each debuggable object has its own unique string pointer.
	mDebugSummary.clear();
	mDebugSummary.insert(0, &theBuffer[0]);
}
#endif // NN_DEBUG




