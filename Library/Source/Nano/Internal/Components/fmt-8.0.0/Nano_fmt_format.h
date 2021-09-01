/*	NAME:
		Nano_fmt_format.h

	DESCRIPTION:
		Nano fmt support.

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
#ifndef NANO_FMT_FORMAT_H
#define NANO_FMT_FORMAT_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoMacros.h"





//=============================================================================
//		fmt
//-----------------------------------------------------------------------------
#define FMT_HEADER_ONLY

#if !NN_DEBUG && !defined(NDEBUG)
	#define NDEBUG
#endif


NN_DIAGNOSTIC_PUSH();
NN_DIAGNOSTIC_IGNORE_CLANG("-Wmissing-noreturn");
NN_DIAGNOSTIC_IGNORE_CLANG("-Wsign-conversion");
NN_DIAGNOSTIC_IGNORE_CLANG("-Wsigned-enum-bitfield");
NN_DIAGNOSTIC_IGNORE_GCC("-Wsuggest-attribute=format");
NN_DIAGNOSTIC_IGNORE_MSVC(C4355_this_used_in_base_member_initializer);
NN_DIAGNOSTIC_IGNORE_MSVC(C4365_signed_unsigned_mismatch);
NN_DIAGNOSTIC_IGNORE_MSVC(C4582_constructor_not_implicitly_called);
NN_DIAGNOSTIC_IGNORE_MSVC(C4668_undefined_macro_treated_as_zero);

#include "fmt_format.h"

NN_DIAGNOSTIC_POP();



#endif // NANO_FMT_FORMAT_H
