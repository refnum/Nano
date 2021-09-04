/*	NAME:
		NFlags.inl

	DESCRIPTION:
		Enum flags.

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
//		Traits
//-----------------------------------------------------------------------------
template<typename T>
struct NFlagEnumTraits
{
	static constexpr bool IsEnum = std::is_enum_v<T>;
	static constexpr bool IsMask = false;
};


#define NN_FLAG_ENUM(_enum, _flags)                             \
	template<>                                                  \
	struct NFlagEnumTraits<_enum>                               \
	{                                                           \
		static constexpr bool IsEnum = std::is_enum_v<_enum>;   \
		static constexpr bool IsMask = false;                   \
	};                                                          \
																\
	using _flags = NFlags<_enum>



#define NN_FLAG_ENUM_MASK(_enum, _flags)                        \
	template<>                                                  \
	struct NFlagEnumTraits<_enum>                               \
	{                                                           \
		static constexpr bool IsEnum = std::is_enum_v<_enum>;   \
		static constexpr bool IsMask = true;                    \
	};                                                          \
																\
	using _flags = NFlags<_enum>





//=============================================================================
//		Global Operatorss
//-----------------------------------------------------------------------------
template<typename T, typename = std::enable_if_t<NFlagEnumTraits<T>::IsEnum>>
constexpr NFlags<T> operator~(T lhs)
{
	return ~NFlags<T>(lhs);
}

template<typename T, typename = std::enable_if_t<NFlagEnumTraits<T>::IsEnum>>
constexpr NFlags<T> operator|(T lhs, T rhs)
{
	return NFlags<T>(lhs) | NFlags<T>(rhs);
}

template<typename T, typename = std::enable_if_t<NFlagEnumTraits<T>::IsEnum>>
constexpr NFlags<T> operator|(T lhs, NFlags<T> rhs)
{
	return NFlags<T>(lhs) | rhs;
}

template<typename T, typename = std::enable_if_t<NFlagEnumTraits<T>::IsEnum>>
constexpr NFlags<T> operator&(T lhs, NFlags<T> rhs)
{
	return NFlags<T>(lhs) & rhs;
}

template<typename T, typename = std::enable_if_t<NFlagEnumTraits<T>::IsEnum>>
constexpr NFlags<T> operator^(T lhs, NFlags<T> rhs)
{
	return NFlags<T>(lhs) ^ rhs;
}





//=============================================================================
//		Internal Class Declaration
//-----------------------------------------------------------------------------
template<typename T>
class NFlagsBool
{
public:
	constexpr NFlagsBool(const NFlags<T> theFlags)
		: mFlags(theFlags)
	{
		static_assert(std::is_enum_v<T>);
	}

	constexpr operator bool() const
	{
		return bool(mFlags);
	}

	constexpr operator NFlags<T>() const
	{
		return mFlags;
	}


private:
	NFlags<T>                           mFlags;
};





//=============================================================================
//		NFlags::NFlags : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T>::NFlags(T theValue)
	: mFlags(GetFlag(theValue))
{
	// Validate our state
										static_assert(NFlagEnumTraits<T>::IsEnum);
}





//=============================================================================
//		NFlags::NFlags : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T>::NFlags()
	: mFlags(0)
{
}





//=============================================================================
//		NFlags::Clear : Clear the flags.
//-----------------------------------------------------------------------------
template<typename T>
constexpr void NFlags<T>::Clear()
{
	mFlags = 0;
}





//=============================================================================
//		NFlags::Clear : Clear a flag.
//-----------------------------------------------------------------------------
template<typename T>
constexpr void NFlags<T>::Clear(T theValue)
{
	*this &= ~theValue;
}





//=============================================================================
//		NFlags::IsSet : Is a flag set?
//-----------------------------------------------------------------------------
template<typename T>
constexpr bool NFlags<T>::IsSet(T theValue) const
{
	return *this & theValue;
}





//=============================================================================
//		NFlags::Set : Set a flag.
//-----------------------------------------------------------------------------
template<typename T>
constexpr void NFlags<T>::Set(T theValue)
{
	*this |= theValue;
}





//=============================================================================
//		NFlags::Toggle : Toggle a flag.
//-----------------------------------------------------------------------------
template<typename T>
constexpr void NFlags<T>::Toggle(T theValue)
{
	*this ^= theValue;
}





//=============================================================================
//		NFlags::operator ~ : Bitwise NOT operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T> NFlags<T>::operator~() const
{
	NFlags<T> theResult;

	theResult.mFlags = ~mFlags;

	return theResult;
}





//=============================================================================
//		NFlags::operator & : Bitwise AND operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlagsBool<T> NFlags<T>::operator&(T theValue) const
{
	NFlags<T> theResult;

	theResult.mFlags = mFlags & GetFlag(theValue);

	return theResult;
}





//=============================================================================
//		NFlags::operator | : Bitwise OR operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T> NFlags<T>::operator|(T theValue) const
{
	NFlags<T> theResult;

	theResult.mFlags = mFlags | GetFlag(theValue);

	return theResult;
}





//=============================================================================
//		NFlags::operator ^ : Bitwise XOR operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T> NFlags<T>::operator^(T theValue) const
{
	NFlags<T> theResult;

	theResult.mFlags = mFlags ^ GetFlag(theValue);

	return theResult;
}





//=============================================================================
//		NFlags::operator & : Bitwise AND operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T> NFlags<T>::operator&(NFlags<T> theValue) const
{
	NFlags<T> theResult;

	theResult.mFlags = mFlags & theValue.mFlags;

	return theResult;
}





//=============================================================================
//		NFlags::operator | : Bitwise OR operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T> NFlags<T>::operator|(NFlags<T> theValue) const
{
	NFlags<T> theResult;

	theResult.mFlags = mFlags | theValue.mFlags;

	return theResult;
}





//=============================================================================
//		NFlags::operator ^ : Bitwise XOR operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T> NFlags<T>::operator^(NFlags<T> theValue) const
{
	NFlags<T> theResult;

	theResult.mFlags = mFlags ^ theValue.mFlags;

	return theResult;
}





//=============================================================================
//		NFlags::operator &= : Bitwise AND operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T>& NFlags<T>::operator&=(T theValue)
{
	mFlags &= GetFlag(theValue);

	return *this;
}





//=============================================================================
//		NFlags::operator |= : Bitwise OR operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T>& NFlags<T>::operator|=(T theValue)
{
	mFlags |= GetFlag(theValue);

	return *this;
}





//=============================================================================
//		NFlags::operator ^= : Bitwise XOR operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T>& NFlags<T>::operator^=(T theValue)
{
	mFlags ^= GetFlag(theValue);

	return *this;
}





//=============================================================================
//		NFlags::operator &= : Bitwise AND operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T>& NFlags<T>::operator&=(NFlags<T> theValue)
{
	mFlags &= theValue.mFlags;

	return *this;
}





//=============================================================================
//		NFlags::operator |= : Bitwise OR operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T>& NFlags<T>::operator|=(NFlags<T> theValue)
{
	mFlags |= theValue.mFlags;

	return *this;
}





//=============================================================================
//		NFlags::operator ^= : Bitwise XOR operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T>& NFlags<T>::operator^=(NFlags<T> theValue)
{
	mFlags ^= theValue.mFlags;

	return *this;
}





//=============================================================================
//		NFlags::operator bool : Cast operator.
//-----------------------------------------------------------------------------
template<typename T>
constexpr NFlags<T>::operator bool() const
{
	return mFlags != 0;
}





#pragma mark private
//=============================================================================
//		NFlags::GetFlag : Get an enum value as a flag.
//-----------------------------------------------------------------------------
template<typename T>
constexpr typename NFlags<T>::Flag NFlags<T>::GetFlag(T theValue) const
{


	// Get the flag
	if constexpr (NFlagEnumTraits<T>::IsMask)
	{
		return Flag(theValue);
	}
	else
	{
		return Flag(1) << Flag(theValue);
	}
}
