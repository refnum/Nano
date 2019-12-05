/*	NAME:
		NanoTypes.h

	DESCRIPTION:
		Nano core types.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
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
#ifndef NANO_TYPES_H
#define NANO_TYPES_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// System
#include <stdint.h>
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Numeric
using float32_t                                             = float;
using float64_t                                             = double;


// Unicode
using utf8_t                                                = char;
using utf16_t                                               = char16_t;
using utf32_t                                               = char32_t;


// Nano
using NRadians                                              = float64_t;
using NDegrees                                              = float64_t;
using NInterval                                             = float64_t;


// Containers
using NVectorUInt8                                          = std::vector<uint8_t>;
using NVectorUInt16                                         = std::vector<uint16_t>;
using NVectorUInt32                                         = std::vector<uint32_t>;
using NVectorUInt64                                         = std::vector<uint64_t>;

using NVectorInt8                                           = std::vector<int8_t>;
using NVectorInt16                                          = std::vector<int16_t>;
using NVectorInt32                                          = std::vector<int32_t>;
using NVectorInt64                                          = std::vector<int64_t>;

using NVectorFloat32                                        = std::vector<float32_t>;
using NVectorFloat64                                        = std::vector<float64_t>;

using NVectorUTF8                                           = std::vector<utf8_t>;
using NVectorUTF16                                          = std::vector<utf16_t>;
using NVectorUTF32                                          = std::vector<utf32_t>;



#endif // NANO_TYPES_H
