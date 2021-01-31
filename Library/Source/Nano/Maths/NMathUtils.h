/*	NAME:
		NMathUtils.h

	DESCRIPTION:
		Math utilities.

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
#ifndef NMATH_UTILS_H
#define NMATH_UTILS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoTypes.h"





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// std::enable_if helpers
#define NN_ENABLE_IF_INTEGER(_T)                            typename    = std::enable_if_t<std::is_integral_v<_T>>

#define NN_ENABLE_IF_UNSIGNED_INTEGER(_T)                   \
	typename                                                = std::enable_if_t < std::is_integral_v<_T> && std::is_unsigned_v<_T> >





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Standard constants
static constexpr float64_t kNE                              = 2.718281828459045235360;     // e
static constexpr float64_t kNLog2E                          = 1.442695040888963407360;     // log2(e)
static constexpr float64_t kNLog10E                         = 0.434294481903251827651;     // log10(e)
static constexpr float64_t kNPi                             = 3.141592653589793238460;     // pi
static constexpr float64_t kNInvPi                          = 0.318309886183790671538;     // 1/pi
static constexpr float64_t kNInvSqrtPi                      = 0.564189583547756286948;     // 1/sqrt(pi)
static constexpr float64_t kNLn2                            = 0.693147180559945309417;     // ln(2)
static constexpr float64_t kNLn10                           = 2.302585092994045684020;     // ln(10)
static constexpr float64_t kNSqrt2                          = 1.4142135623730950488016;    // sqrt(2)
static constexpr float64_t kNInvSqrt2                       = 0.7071067811865475244008;    // 1/sqrt(2)
static constexpr float64_t kNSqrt3                          = 1.7320508075688772935274;    // sqrt(3)
static constexpr float64_t kNInvSqrt3                       = 0.5773502691896257645091;    // 1/sqrt(3)
static constexpr float64_t kNEGamma                         = 0.577215664901532860606;     // Euler–Mascheroni constant
static constexpr float64_t kNPhi                            = 1.618033988749894848204;     // phi





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Geometry
using NRadians                                              = float64_t;
using NDegrees                                              = float64_t;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NMathUtils
{
public:
	// Test the parity
	template<typename T,                NN_ENABLE_IF_INTEGER(T)>
	static constexpr bool               IsOdd(T theValue);

	template<typename T,                NN_ENABLE_IF_INTEGER(T)>
	static constexpr bool               IsEven(T theValue);


	// Test for a power of 2
	template<typename T,                NN_ENABLE_IF_INTEGER(T)>
	static constexpr bool               IsPowerOf2(          T theValue);


	// Get the next power of 2
	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	static constexpr T                  NextPowerOf2(T theValue);


	// Count the number of set bits
	template<typename T,                NN_ENABLE_IF_INTEGER(T)>
	static constexpr size_t             CountBits(           T theValue);


	// Rotate left
	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	static T                            RotateLeft(T theValue, size_t rotateBy);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	static T                            RotateRight(T theValue, size_t rotateBy);


	// Geometry
	static constexpr NDegrees           ToDegrees(NRadians theValue);
	static constexpr NRadians           ToRadians(NDegrees theValue);
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMathUtils.inl"



#endif // NMATH_UTILS_H
