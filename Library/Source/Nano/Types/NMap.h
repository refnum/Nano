/*	NAME:
		NMap.h

	DESCRIPTION:
		Fast std::unordered_map-like object.

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
#ifndef NMAP_H
#define NMAP_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoConstants.h"
#include "NanoMacros.h"

NN_DIAGNOSTIC_PUSH();
NN_DIAGNOSTIC_IGNORE_CLANG("-Wsign-conversion");
NN_DIAGNOSTIC_IGNORE_CLANG("-Wunused-template");
NN_DIAGNOSTIC_IGNORE_GCC("-Wshadow");
NN_DIAGNOSTIC_IGNORE_GCC("-Wsign-conversion");
NN_DIAGNOSTIC_IGNORE_MSVC(C4127_conditional_expression_is_constant);
NN_DIAGNOSTIC_IGNORE_MSVC(C4365_signed_unsigned_mismatch);
#include "hopscotch_map.h"
NN_DIAGNOSTIC_POP();





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Map
//
// An NMap presents a broadly similar interface to std::unordered_map,
// but is based on a more efficient implementation.
//
// The current implementation is tsl::hopscotch_map.
template<typename Key, typename Value>
using NMap                                                  = tsl::hopscotch_map<Key, Value>;



#endif // NMAP_H
