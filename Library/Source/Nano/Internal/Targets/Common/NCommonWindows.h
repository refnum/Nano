/*	NAME:
		NCommonWindows.h

	DESCRIPTION:
		Windows support.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
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
#ifndef NCOMMON_WINDOWS_H
#define NCOMMON_WINDOWS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NWindows.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NString;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NCommonWindows
{
public:
	// Registry
	static int32_t                      RegistryGetInt32( HKEY hKey, const NString& theKey, const NString& theValue);
	static NString                      RegistryGetString(HKEY hKey, const NString& theKey, const NString& theValue);


	// Error conversion
	//
	// StatusLastError returns NStatus::OK if passed true, otherwise it
	// returns the current value of ::GetLastError as an NStatus.
	static NStatus                      StatusResult(HRESULT winErr);
	static NStatus                      StatusLastError(bool wasOK = false);
};



#endif // NCOMMON_WINDOWS_H
