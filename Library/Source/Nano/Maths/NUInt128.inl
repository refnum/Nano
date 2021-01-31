/*	NAME:
		NUInt128.inl

	DESCRIPTION:
		uint128_t type.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// uint128_t
//
// uint128_t is defined natively if possible, or mapped to NUInt128.
#undef NN_UINT128_IS_NATIVE

#if defined(__SIZEOF_INT128__)
	#define NN_UINT128_IS_NATIVE                            1
using uint128_t                                             = unsigned __int128;
#else
	#define NN_UINT128_IS_NATIVE                            0
using uint128_t                                             = NUInt128;
#endif


// NUInt128Division
//
// Quotient and remainder for a division operation.
struct NDivision128
{
	NUInt128 theQuotient;
	NUInt128 theRemainder;
};





//=============================================================================
//		Public Functions
//-----------------------------------------------------------------------------
//		NMakeUInt128 : Construct a uint128_t.
//-----------------------------------------------------------------------------
constexpr uint128_t NMakeUInt128(uint64_t valueHi, uint64_t valueLo);
constexpr uint128_t NMakeUInt128(uint64_t valueHi, uint64_t valueLo)
{
#if NN_UINT128_IS_NATIVE
	return uint128_t(valueHi) << 64 | uint128_t(valueLo);
#else
	return NUInt128(valueHi, valueLo);
#endif
}





#pragma mark NUInt128
//=============================================================================
//		NUInt128::NUInt128 : Constructor.
//-----------------------------------------------------------------------------
constexpr NUInt128::NUInt128(uint64_t valueHi, uint64_t valueLo)
#if NN_ENDIAN_LITTLE
	: mLo(valueLo)
	, mHi(valueHi)
#else
	: mHi(valueHi)
	, mLo(valueLo)
#endif
{
}





//=============================================================================
//		NUInt128::NUInt128 : Constructor.
//-----------------------------------------------------------------------------
constexpr NUInt128::NUInt128()
#if NN_ENDIAN_LITTLE
	: mLo(0)
	, mHi(0)
#else
	: mHi(0)
	, mLo(0)
#endif
{
}





//=============================================================================
//		NUInt128::NUInt128 : Constructor.
//-----------------------------------------------------------------------------
constexpr NUInt128::NUInt128(const NUInt128& theValue)
#if NN_ENDIAN_LITTLE
	: mLo(theValue.mLo)
	, mHi(theValue.mHi)
#else
	: mHi(theValue.mHi)
	, mLo(theValue.mLo)
#endif
{
}





//=============================================================================
//		NUInt128::operator= : Assignment operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator=(const NUInt128& theValue)
{
	mHi = theValue.mHi;
	mLo = theValue.mLo;

	return *this;
}





//=============================================================================
//		NUInt128::NUInt128 : Constructor.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128::NUInt128(T theValue)
#if NN_ENDIAN_LITTLE
	: mLo(uint64_t(theValue))
	, mHi(0)
#else
	: mHi(0)
	, mLo(uint64_t(theValue))
#endif
{
}





//=============================================================================
//		NUInt128::operator= : Assignment operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128& NUInt128::operator=(T theValue)
{
	*this = NUInt128(theValue);

	return *this;
}





//=============================================================================
//		NUInt128::operator++ : Pre-increment operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator++()
{
	operator+=(NUInt128(1));

	return *this;
}





//=============================================================================
//		NUInt128::operator++ : Post-increment operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator++(int)
{
	NUInt128 theResult(*this);

	operator++();

	return theResult;
}





//=============================================================================
//		NUInt128::operator-- : Pre-decrement operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator--()
{
	operator-=(NUInt128(1));

	return *this;
}





//=============================================================================
//		NUInt128::operator-- : Post-decrement operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator--(int)
{
	NUInt128 theResult(*this);

	operator--();

	return theResult;
}





//=============================================================================
//		NUInt128::operator+ : Addition operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator+(const NUInt128& theValue) const
{
	NUInt128 theResult;

	uint64_t resultLo = (mLo + theValue.mLo);
	uint64_t carryBit = (resultLo < mLo) ? 1u : 0u;

	theResult.mHi = mHi + theValue.mHi + carryBit;
	theResult.mLo = resultLo;

	return theResult;
}





//=============================================================================
//		NUInt128::operator- : Subtraction operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator-(const NUInt128& theValue) const
{
	NUInt128 theResult;

	uint64_t resultLo  = (mLo - theValue.mLo);
	uint64_t borrowBit = (resultLo > mLo) ? 1u : 0u;

	theResult.mHi = mHi - theValue.mHi - borrowBit;
	theResult.mLo = resultLo;

	return theResult;
}





//=============================================================================
//		NUInt128::operator* : Multiplication operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator*(const NUInt128& theValue) const
{
	NUInt128 theResult;

	theResult = Multiply64x64(mLo, theValue.mLo);
	theResult.mHi += (mHi * theValue.mLo) + (mLo * theValue.mHi);

	return theResult;
}





//=============================================================================
//		NUInt128::operator/ : Division operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator/(const NUInt128& theValue) const
{
	return DivideModulo128(*this, theValue).theQuotient;
}





//=============================================================================
//		NUInt128::operator% : Modulo operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator%(const NUInt128& theValue) const
{
	return DivideModulo128(*this, theValue).theRemainder;
}





//=============================================================================
//		NUInt128::operator+= : Addition operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator+=(const NUInt128& theValue)
{
	*this = *this + theValue;

	return *this;
}





//=============================================================================
//		NUInt128::operator-= : Subtraction operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator-=(const NUInt128& theValue)
{
	*this = *this - theValue;

	return *this;
}





//=============================================================================
//		NUInt128::operator*= : Multiplication operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator*=(const NUInt128& theValue)
{
	*this = *this * theValue;

	return *this;
}





//=============================================================================
//		NUInt128::operator/= : Division operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator/=(const NUInt128& theValue)
{
	*this = *this / theValue;

	return *this;
}





//=============================================================================
//		NUInt128::operator%= : Modulo operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator%=(const NUInt128& theValue)
{
	*this = *this % theValue;

	return *this;
}





//=============================================================================
//		NUInt128::operator+ : Addition operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128 NUInt128::operator+(T theValue) const
{
	return operator+(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator- : Subtraction operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128 NUInt128::operator-(T theValue) const
{
	return operator-(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator* : Multiplication operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128 NUInt128::operator*(T theValue) const
{
	return operator*(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator/ : Division operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128 NUInt128::operator/(T theValue) const
{
	return operator/(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator% : Modulo operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128 NUInt128::operator%(T theValue) const
{
	return operator%(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator+= : Addition operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128& NUInt128::operator+=(T theValue)
{
	return operator+=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator-= : Subtraction operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128& NUInt128::operator-=(T theValue)
{
	return operator-=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator*= : Multiplication operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128& NUInt128::operator*=(T theValue)
{
	return operator*=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator/= : Division operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128& NUInt128::operator/=(T theValue)
{
	return operator/=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator%= : Modulo operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128& NUInt128::operator%=(T theValue)
{
	return operator%=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator~ : Bitwise NOT operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator~() const
{
	NUInt128 theResult;

	theResult.mHi = ~mHi;
	theResult.mLo = ~mLo;

	return theResult;
}





//=============================================================================
//		NUInt128::operator& : Bitwise AND operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator&(const NUInt128& theValue) const
{
	NUInt128 theResult;

	theResult.mHi = mHi & theValue.mHi;
	theResult.mLo = mLo & theValue.mLo;

	return theResult;
}





//=============================================================================
//		NUInt128::operator| : Bitwise OR operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator|(const NUInt128& theValue) const
{
	NUInt128 theResult;

	theResult.mHi = mHi | theValue.mHi;
	theResult.mLo = mLo | theValue.mLo;

	return theResult;
}





//=============================================================================
//		NUInt128::operator^ : Bitwise XOR operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator^(const NUInt128& theValue) const
{
	NUInt128 theResult;

	theResult.mHi = mHi ^ theValue.mHi;
	theResult.mLo = mLo ^ theValue.mLo;

	return theResult;
}





//=============================================================================
//		NUInt128::operator<< : Left shift operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator<<(const NUInt128& theValue) const
{
	NUInt128 theResult;

	if (theValue.mHi == 0)
	{
		uint64_t shiftLeft = theValue.mLo;

		if (shiftLeft == 0)
		{
			theResult = *this;
		}
		else if (shiftLeft < 64)
		{
			theResult.mHi = (mHi << shiftLeft) | (mLo >> (64 - shiftLeft));
			theResult.mLo = (mLo << shiftLeft);
		}
		else
		{
			theResult.mHi = (mLo << (shiftLeft - 64));
			theResult.mLo = 0;
		}
	}

	return theResult;
}





//=============================================================================
//		NUInt128::operator>> : Right shift operator.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::operator>>(const NUInt128& theValue) const
{
	NUInt128 theResult;

	if (theValue.mHi == 0)
	{
		uint64_t shiftRight = theValue.mLo;

		if (shiftRight == 0)
		{
			theResult = *this;
		}
		else if (shiftRight < 64)
		{
			theResult.mHi = (mHi >> shiftRight);
			theResult.mLo = (mLo >> shiftRight) | (mHi << (64 - shiftRight));
		}
		else
		{
			theResult.mHi = 0;
			theResult.mLo = (mHi >> (shiftRight - 64));
		}
	}

	return theResult;
}





//=============================================================================
//		NUInt128::operator&= : Bitwise AND operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator&=(const NUInt128& theValue)
{
	*this = *this & theValue;

	return *this;
}





//=============================================================================
//		NUInt128::operator|= : Bitwise OR operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator|=(const NUInt128& theValue)
{
	*this = *this | theValue;

	return *this;
}





//=============================================================================
//		NUInt128::operator^= : Bitwise XOR operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator^=(const NUInt128& theValue)
{
	*this = *this ^ theValue;

	return *this;
}





//=============================================================================
//		NUInt128::operator<<= : Left shift operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator<<=(const NUInt128& theValue)
{
	*this = *this << theValue;

	return *this;
}





//=============================================================================
//		NUInt128::operator>>= : Right shift operator.
//-----------------------------------------------------------------------------
constexpr NUInt128& NUInt128::operator>>=(const NUInt128& theValue)
{
	*this = *this >> theValue;

	return *this;
}





//=============================================================================
//		NUInt128::operator& : Bitwise AND operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128 NUInt128::operator&(T theValue) const
{
	return operator&(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator| : Bitwise OR operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128 NUInt128::operator|(T theValue) const
{
	return operator|(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator^ : Bitwise XOR operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128 NUInt128::operator^(T theValue) const
{
	return operator^(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator<< : Left shift operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128 NUInt128::operator<<(T theValue) const
{
	return operator<<(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator>> : Right shift operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128 NUInt128::operator>>(T theValue) const
{
	return operator>>(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator&= : Bitwise AND operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128& NUInt128::operator&=(T theValue)
{
	return operator&=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator|= : Bitwise OR operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128& NUInt128::operator|=(T theValue)
{
	return operator|=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator^= : Bitwise XOR operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128& NUInt128::operator^=(T theValue)
{
	return operator^=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator<<= : Left shift operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128& NUInt128::operator<<=(T theValue)
{
	return operator<<=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator>>= : Right shift operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr NUInt128& NUInt128::operator>>=(T theValue)
{
	return operator>>=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator! : Logical NOT operator.
//-----------------------------------------------------------------------------
constexpr bool NUInt128::operator!() const
{
	return !bool(mHi | mLo);
}





//=============================================================================
//		NUInt128::operator&& : Logical AND operator.
//-----------------------------------------------------------------------------
constexpr bool NUInt128::operator&&(const NUInt128& theValue) const
{
	return bool(mHi | mLo) && bool(theValue.mHi | theValue.mLo);
}





//=============================================================================
//		NUInt128::operator|| : Logical OR operator.
//-----------------------------------------------------------------------------
constexpr bool NUInt128::operator||(const NUInt128& theValue) const
{
	return bool(mHi | mLo) || bool(theValue.mHi | theValue.mLo);
}





//=============================================================================
//		NUInt128::operator&& : Logical AND operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr bool NUInt128::operator&&(T theValue) const
{
	return operator&&(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator|| : Logical OR operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr bool NUInt128::operator||(T theValue) const
{
	return operator||(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator< : Less-than operator.
//-----------------------------------------------------------------------------
constexpr bool NUInt128::operator<(const NUInt128& theValue) const
{
	if (mHi == theValue.mHi)
	{
		return mLo < theValue.mLo;
	}
	else
	{
		return mHi < theValue.mHi;
	}
}





//=============================================================================
//		NUInt128::operator> : Greater-than operator.
//-----------------------------------------------------------------------------
constexpr bool NUInt128::operator>(const NUInt128& theValue) const
{
	return theValue < *this;
}





//=============================================================================
//		NUInt128::operator<= : Less-than-equals operator.
//-----------------------------------------------------------------------------
constexpr bool NUInt128::operator<=(const NUInt128& theValue) const
{
	return !(theValue < *this);
}





//=============================================================================
//		NUInt128::operator>= : Greater-than-equals operator.
//-----------------------------------------------------------------------------
constexpr bool NUInt128::operator>=(const NUInt128& theValue) const
{
	return !(*this < theValue);
}





//=============================================================================
//		NUInt128::operator== : Equality operator.
//-----------------------------------------------------------------------------
constexpr bool NUInt128::operator==(const NUInt128& theValue) const
{
	return (mHi == theValue.mHi) && (mLo == theValue.mLo);
}





//=============================================================================
//		NUInt128::operator!= : Inequality operator.
//-----------------------------------------------------------------------------
constexpr bool NUInt128::operator!=(const NUInt128& theValue) const
{
	return !(*this == theValue);
}





//=============================================================================
//		NUInt128::operator< : Less-than operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr bool NUInt128::operator<(T theValue) const
{
	return operator<(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator> : Greater-than operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr bool NUInt128::operator>(T theValue) const
{
	return operator>(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator<= : Less-than-equals operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr bool NUInt128::operator<=(T theValue) const
{
	return operator<=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator>= : Greater-than-equals operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr bool NUInt128::operator>=(T theValue) const
{
	return operator>=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator== : Equality operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr bool NUInt128::operator==(T theValue) const
{
	return operator==(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator!= : Inequality operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr bool NUInt128::operator!=(T theValue) const
{
	return operator!=(NUInt128(theValue));
}





//=============================================================================
//		NUInt128::operator bool : Cast operator.
//-----------------------------------------------------------------------------
constexpr NUInt128::operator bool() const
{
	return bool(mHi | mLo);
}





//=============================================================================
//		NUInt128::operator uint8_t : Cast operator.
//-----------------------------------------------------------------------------
constexpr NUInt128::operator uint8_t() const
{
	return uint8_t(mLo);
}





//=============================================================================
//		NUInt128::operator uint16_t : Cast operator.
//-----------------------------------------------------------------------------
constexpr NUInt128::operator uint16_t() const
{
	return uint16_t(mLo);
}





//=============================================================================
//		NUInt128::operator uint32_t : Cast operator.
//-----------------------------------------------------------------------------
constexpr NUInt128::operator uint32_t() const
{
	return uint32_t(mLo);
}





//=============================================================================
//		NUInt128::operator uint64_t : Cast operator.
//-----------------------------------------------------------------------------
constexpr NUInt128::operator uint64_t() const
{
	return uint64_t(mLo);
}





//=============================================================================
//		NUInt128::operator GetHi : Get the high 64-bits.
//-----------------------------------------------------------------------------
constexpr uint64_t NUInt128::GetHi() const
{
	return mHi;
}





//=============================================================================
//		NUInt128::operator GetLo : Get the low 64-bits.
//-----------------------------------------------------------------------------
constexpr uint64_t NUInt128::GetLo() const
{
	return mLo;
}





#pragma mark private
//=============================================================================
//		NUInt128::CountLeadingZeros : Count the leading zeros.
//-----------------------------------------------------------------------------
constexpr size_t NUInt128::CountLeadingZeros() const
{
	if (mHi == 0)
	{
		return NMathUtils::CountLeadingZeros(mLo) + 64;
	}
	else
	{
		return NMathUtils::CountLeadingZeros(mHi);
	}
}





//=============================================================================
//		NUInt128::Multiply64x64 : Multiply two 64-bit numbers.
//-----------------------------------------------------------------------------
constexpr NUInt128 NUInt128::Multiply64x64(uint64_t valueA, uint64_t valueB) const
{


	// Multiply with Knuth's Algorithm M
	//
	// Adapted from Hacker's Delight muldwu.c.txt
	uint64_t aLo = valueA & 0xFFFFFFFF;
	uint64_t bLo = valueB & 0xFFFFFFFF;

	uint64_t aHi = valueA >> 32;
	uint64_t bHi = valueB >> 32;

	uint64_t t  = aLo * bLo;
	uint64_t w3 = t & 0xFFFFFFFF;
	uint64_t k  = t >> 32;

	t           = (aHi * bLo) + k;
	uint64_t w2 = (t & 0xFFFFFFFF);
	uint64_t w1 = (t >> 32);

	t = (aLo * bHi) + w2;
	k = (t >> 32);

	NUInt128 theResult;

	theResult.mHi = (aHi * bHi) + w1 + k;
	theResult.mLo = (t << 32) + w3;

	return theResult;
}





//=============================================================================
//		DivideModulo128 : Divide two 128-bit numbers.
//-----------------------------------------------------------------------------
constexpr NDivision128 NUInt128::DivideModulo128(const NUInt128& valueA,
												 const NUInt128& valueB) const
{


	// Special cases
	if (valueB == 0)
	{
		return {NUInt128(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL),
				NUInt128(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL)};
	}

	else if (valueB == 1)
	{
		return {valueA, 0};
	}

	else if (valueA == valueB)
	{
		return {1, 0};
	}

	else if (valueA == 0 || valueA < valueB)
	{
		return {0, valueA};
	}



	// General case
	//
	// Binary long-division: process each bit of the numerator, shifting
	// them into the LSB of the remainder as we accumulate the quotient.
	NDivision128 theResult;

	int8_t numBits = int8_t(128 - valueA.CountLeadingZeros());

	for (int8_t n = numBits - 1; n >= 0; n--)
	{
		NUInt128 bitN = ((valueA >> n) & 0x01);

		theResult.theRemainder = (theResult.theRemainder << 1) | bitN;

		if (theResult.theRemainder >= valueB)
		{
			theResult.theQuotient |= NUInt128(1) << n;
			theResult.theRemainder -= valueB;
		}
	}

	return theResult;
}
