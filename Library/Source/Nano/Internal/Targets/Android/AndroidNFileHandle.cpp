/*	NAME:
		AndroidNFileHandle.cpp

	DESCRIPTION:
		Android file handle.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFileHandle.h"

// Nano
#include "NSharedPOSIX.h"





//=============================================================================
//		NFileHandle::FileOpen : Open the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileOpen(NFilePermission thePermission)
{


	// Open the file
	return NSharedPOSIX::FileOpen(mPath, thePermission, mHandle);
}





//=============================================================================
//		NFileHandle::FileClose : Close the file.
//-----------------------------------------------------------------------------
void NFileHandle::FileClose()
{


	// Open the file
	NSharedPOSIX::FileClose(mHandle);
}





//=============================================================================
//		NFileHandle::FileGetPosition : Get the file position.
//-----------------------------------------------------------------------------
uint64_t NFileHandle::FileGetPosition() const
{


	// Open the file
	return NSharedPOSIX::FileGetPosition(mHandle);
}





//=============================================================================
//		NFileHandle::FileSetPosition : Set the file position.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileSetPosition(int64_t thePosition, NFileOffset relativeTo)
{


	// Open the file
	return NSharedPOSIX::FileSetPosition(mHandle, thePosition, relativeTo);
}





//=============================================================================
//		NFileHandle::FileSetSize : Set the size.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileSetSize(uint64_t theSize)
{


	// Set the size
	return NSharedPOSIX::FileSetSize(mHandle, theSize);
}





//=============================================================================
//		NFileHandle::FileRead : Read from the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileRead(uint64_t theSize, void* thePtr, uint64_t& numRead)
{


	// Read from the file
	return NSharedPOSIX::FileRead(mHandle, theSize, thePtr, numRead);
}





//=============================================================================
//		NFileHandle::FileWrite : Write to the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileWrite(uint64_t theSize, const void* thePtr, uint64_t& numWritten)
{


	// Write to the file
	return NSharedPOSIX::FileWrite(mHandle, theSize, thePtr, numWritten);
}





//=============================================================================
//		NFileHandle::FileFlush : Flush the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileFlush()
{


	// Flush the file
	return NSharedPOSIX::FileFlush(mHandle);
}
