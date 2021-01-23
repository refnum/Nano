/*	NAME:
		NStdUtility.h

	DESCRIPTION:
		STL utility extensions.

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
#ifndef NSTD_UTILITY_H
#define NSTD_UTILITY_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoTargets.h"

namespace nstd
{





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// std::any alignment
//
// Workaround for pre-VS 2019 16.15 lack of support for over-aligned
// types held within a std::any.
//
// May only be used where alignment is only used to improve codegen,
// and so can be safely omitted.
//
// https://developercommunity.visualstudio.com/idea/724444/stdany-should-support-over-aligned-types.html
//
#if NN_COMPILER_MSVC != 0 && NN_COMPILER_MSVC < 192528611    // VS 2019 16.5
	#define NN_STD_ANY_ALIGNAS(_x)
#else
	#define NN_STD_ANY_ALIGNAS(_x)                          alignas(_x)
#endif



}    // namespace nstd


#endif // NSTD_UTILITY_H
