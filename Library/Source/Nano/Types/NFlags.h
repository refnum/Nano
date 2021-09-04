/*	NAME:
		NFlags.h

	DESCRIPTION:
		A flag object whose values are derived from an enumerated type.

		An enum can be a list of values:

			enum class MyValue
			{
				One,
				Two,
				Three
			};

			NN_FLAG_ENUM(MyValue, MyValueFlags);

		or a list of bits:

			enum class MyBit
			{
				One                                         = 1 << 5,
				Two                                         = 1 << 6,
				Three                                       = 1 << 7,
			};

			NN_FLAG_ENUM_BITS(MyBit, MyBitFlags);

		An enum that contains values will assign a bit to each value by
		shifting 1 left by the value. The underlying type must be large
		enough to hold the shifted bit.

		An enum that contains bits will use the specified bits.

		The NN_FLAG_ENUM / NN_FLAG_ENUM_BITS macro defines an NFlag type
		to hold the enum.


		All expected bit operations are allowed. Invalid operations produce
		a compile-time error:

			MyBitFlags theFlags;

			theFlags                                        = MyBit::One | MyBit::Two;		// Allowed
			theFlags                                        = MyBit::One & MyBit::Two;		// Error: always zero
			theFlags                                        = MyBit::One ^ MyBit::Two;		// Error: always zero

			if (theFlags & MyBit::One)					// Allowed
				...

			if (theFlags == MyBit::One)				// Error: use '&' to test bits
				...

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
#ifndef NFLAGS_H
#define NFLAGS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// System
#include <type_traits>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
template<typename T>
class NFlagsBool;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<typename T>
class NFlags
{
public:
	constexpr                           NFlags(T theValue);
	constexpr                           NFlags();


	// Clear the flags
	constexpr void                      Clear();


	// Clear a flag
	constexpr void                      Clear(T theValue);


	// Is a flag set?
	constexpr bool                      IsSet(T theValue) const;


	// Set a flag
	constexpr void                      Set(T theValue);


	// Toggle a flag
	constexpr void                      Toggle(T theValue);


	// Bitwise
	//
	// Operator & returns an object that can be converted to a bool for
	// use in conditionals or back to an NFlags<T> for assignments.
	constexpr NFlags<T>                 operator~() const;

	constexpr NFlagsBool<T>             operator&(       T  theValue) const;
	constexpr NFlags<T>                 operator|(       T  theValue) const;
	constexpr NFlags<T>                 operator^(       T  theValue) const;
	constexpr NFlags<T>                 operator&(NFlags<T> theValue) const;
	constexpr NFlags<T>                 operator|(NFlags<T> theValue) const;
	constexpr NFlags<T>                 operator^(NFlags<T> theValue) const;

	constexpr NFlags<T>&                operator&=(       T  theValue);
	constexpr NFlags<T>&                operator|=(       T  theValue);
	constexpr NFlags<T>&                operator^=(       T  theValue);
	constexpr NFlags<T>&                operator&=(NFlags<T> theValue);
	constexpr NFlags<T>&                operator|=(NFlags<T> theValue);
	constexpr NFlags<T>&                operator^=(NFlags<T> theValue);


	// Cast
	constexpr                           operator bool() const;


private:
	using Flag = std::underlying_type_t<T>;

	constexpr Flag                      GetFlag(T theValue) const;


private:
	Flag                                mFlags;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFlags.inl"



#endif // NFLAGS_H
