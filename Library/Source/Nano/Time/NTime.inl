/*	NAME:
		NTime.inl

	DESCRIPTION:
		Time object.

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
#include "NDebug.h"
#include "NanoConstants.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr NInterval kNTimeMaxUInt64                  = NInterval(kNUInt64Max);





//=============================================================================
//		NTime::NTime : Constructor.
//-----------------------------------------------------------------------------
constexpr NTime::NTime(NInterval theValue)
	: mValue(theValue)
{
}





//=============================================================================
//		NTime::NTime : Constructor.
//-----------------------------------------------------------------------------
inline NTime::NTime(NInterval theValue, NInterval fromEpoch)
	: mValue(theValue + fromEpoch)
{


	// Validate our parameters
	NN_REQUIRE(fromEpoch <= 0.0, "Expected kNanoEpochFromXXXX");
}





//=============================================================================
//		NTime::ToMS : Convert to milliseconds.
//-----------------------------------------------------------------------------
inline uint64_t NTime::ToMS() const
{


	// Convert to milliseconds
	return ToMS(mValue);
}





//=============================================================================
//		NTime::ToUS : Convert to microseconds.
//-----------------------------------------------------------------------------
inline uint64_t NTime::ToUS() const
{


	// Convert to microseconds
	return ToUS(mValue);
}





//=============================================================================
//		NTime::ToNS : Convert to nanoseconds.
//-----------------------------------------------------------------------------
inline uint64_t NTime::ToNS() const
{


	// Convert to nanoseconds
	return ToNS(mValue);
}





//=============================================================================
//		NTime::ToMS : Convert to milliseconds.
//-----------------------------------------------------------------------------
inline uint64_t NTime::ToMS(NInterval theValue)
{


	// Validate our state
	NN_REQUIRE(theValue >= 0.0);
	NN_REQUIRE((theValue / kNTimeMillisecond) <= kNTimeMaxUInt64);



	// Convert to milliseconds
	return uint64_t(theValue / kNTimeMillisecond);
}





//=============================================================================
//		NTime::ToUS : Convert to microseconds.
//-----------------------------------------------------------------------------
inline uint64_t NTime::ToUS(NInterval theValue)
{


	// Validate our state
	NN_REQUIRE(theValue >= 0.0);
	NN_REQUIRE((theValue / kNTimeMicrosecond) <= kNTimeMaxUInt64);



	// Convert to microseconds
	return uint64_t(theValue / kNTimeMicrosecond);
}





//=============================================================================
//		NTime::ToNS : Convert to nanoseconds.
//-----------------------------------------------------------------------------
inline uint64_t NTime::ToNS(NInterval theValue)
{


	// Validate our state
	NN_REQUIRE(theValue >= 0.0);
	NN_REQUIRE((theValue / kNTimeNanosecond) <= kNTimeMaxUInt64);



	// Convert to nanoseconds
	return uint64_t(theValue / kNTimeNanosecond);
}





//=============================================================================
//		NTime::NInterval : Convert to an interval.
//-----------------------------------------------------------------------------
constexpr NTime::operator NInterval() const
{
	return mValue;
}





//=============================================================================
//		NTime::operator == : Equality operator.
//-----------------------------------------------------------------------------
constexpr bool NTime::operator==(const NTime& theTime) const
{
	return mValue == theTime.mValue;
}





//=============================================================================
//		NTime::operator != : Inequality operator.
//-----------------------------------------------------------------------------
constexpr bool NTime::operator!=(const NTime& theTime) const
{
	return mValue != theTime.mValue;
}





//=============================================================================
//		NTime::operator < : Less-than operator.
//-----------------------------------------------------------------------------
constexpr bool NTime::operator<(const NTime& theTime) const
{
	return mValue < theTime.mValue;
}





//=============================================================================
//		NTime::operator > : Greater-than operator.
//-----------------------------------------------------------------------------
constexpr bool NTime::operator>(const NTime& theTime) const
{
	return mValue > theTime.mValue;
}





//=============================================================================
//		NTime::operator <= : Less-than-or-equal operator.
//-----------------------------------------------------------------------------
constexpr bool NTime::operator<=(const NTime& theTime) const
{
	return mValue <= theTime.mValue;
}





//=============================================================================
//		NTime::operator >= : Greater-than-or-equal operator.
//-----------------------------------------------------------------------------
constexpr bool NTime::operator>=(const NTime& theTime) const
{
	return mValue >= theTime.mValue;
}





//=============================================================================
//		NTime::operator == : Equality operator.
//-----------------------------------------------------------------------------
constexpr bool NTime::operator==(NInterval theValue) const
{
	return mValue == theValue;
}





//=============================================================================
//		NTime::operator != : Inequality operator.
//-----------------------------------------------------------------------------
constexpr bool NTime::operator!=(NInterval theValue) const
{
	return mValue != theValue;
}





//=============================================================================
//		NTime::operator < : Less-than operator.
//-----------------------------------------------------------------------------
constexpr bool NTime::operator<(NInterval theValue) const
{
	return mValue < theValue;
}





//=============================================================================
//		NTime::operator > : Greater-than operator.
//-----------------------------------------------------------------------------
constexpr bool NTime::operator>(NInterval theValue) const
{
	return mValue > theValue;
}





//=============================================================================
//		NTime::operator <= : Less-than-or-equal operator.
//-----------------------------------------------------------------------------
constexpr bool NTime::operator<=(NInterval theValue) const
{
	return mValue <= theValue;
}





//=============================================================================
//		NTime::operator >= : Greater-than-or-equal operator.
//-----------------------------------------------------------------------------
constexpr bool NTime::operator>=(NInterval theValue) const
{
	return mValue >= theValue;
}





//=============================================================================
//		NTime::operator += : Addition operator.
//-----------------------------------------------------------------------------
inline NTime& NTime::operator+=(const NTime& theTime)
{
	mValue += theTime.mValue;
	return *this;
}





//=============================================================================
//		NTime::operator -= : Subtraction operator.
//-----------------------------------------------------------------------------
inline NTime& NTime::operator-=(const NTime& theTime)
{
	mValue -= theTime.mValue;
	return *this;
}





//=============================================================================
//		NTime::operator *= : Multiplication operator.
//-----------------------------------------------------------------------------
inline NTime& NTime::operator*=(const NTime& theTime)
{
	mValue *= theTime.mValue;
	return *this;
}





//=============================================================================
//		NTime::operator /= : Division operator.
//-----------------------------------------------------------------------------
inline NTime& NTime::operator/=(const NTime& theTime)
{
	mValue /= theTime.mValue;
	return *this;
}





//=============================================================================
//		NTime::operator += : Addition operator.
//-----------------------------------------------------------------------------
inline NTime& NTime::operator+=(NInterval theValue)
{
	mValue += theValue;
	return *this;
}





//=============================================================================
//		NTime::operator -= : Subtraction operator.
//-----------------------------------------------------------------------------
inline NTime& NTime::operator-=(NInterval theValue)
{
	mValue -= theValue;
	return *this;
}





//=============================================================================
//		NTime::operator *= : Multiplication operator.
//-----------------------------------------------------------------------------
inline NTime& NTime::operator*=(NInterval theValue)
{
	mValue *= theValue;
	return *this;
}





//=============================================================================
//		NTime::operator /= : Division operator.
//-----------------------------------------------------------------------------
inline NTime& NTime::operator/=(NInterval theValue)
{
	mValue /= theValue;
	return *this;
}





//=============================================================================
//		NTime::operator + : Addition operator.
//-----------------------------------------------------------------------------
constexpr NTime NTime::operator+(const NTime& theTime) const
{
	return NTime(mValue + theTime.mValue);
}





//=============================================================================
//		NTime::operator - : Subtraction operator.
//-----------------------------------------------------------------------------
constexpr NTime NTime::operator-(const NTime& theTime) const
{
	return NTime(mValue - theTime.mValue);
}





//=============================================================================
//		NTime::operator * : Multiplication operator.
//-----------------------------------------------------------------------------
constexpr NTime NTime::operator*(const NTime& theTime) const
{
	return NTime(mValue * theTime.mValue);
}





//=============================================================================
//		NTime::operator / : Division operator.
//-----------------------------------------------------------------------------
constexpr NTime NTime::operator/(const NTime& theTime) const
{
	return NTime(mValue / theTime.mValue);
}





//=============================================================================
//		NTime::operator + : Addition operator.
//-----------------------------------------------------------------------------
constexpr NTime NTime::operator+(NInterval theValue) const
{
	return NTime(mValue + theValue);
}





//=============================================================================
//		NTime::operator - : Subtraction operator.
//-----------------------------------------------------------------------------
constexpr NTime NTime::operator-(NInterval theValue) const
{
	return NTime(mValue - theValue);
}





//=============================================================================
//		NTime::operator * : Multiplication operator.
//-----------------------------------------------------------------------------
constexpr NTime NTime::operator*(NInterval theValue) const
{
	return NTime(mValue * theValue);
}





//=============================================================================
//		NTime::operator / : Division operator.
//-----------------------------------------------------------------------------
constexpr NTime NTime::operator/(NInterval theValue) const
{
	return NTime(mValue / theValue);
}
