/*	NAME:
		NMixinHashable.h

	DESCRIPTION:
		Mix-in for hashable objects.

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
#ifndef NMIXIN_HASHABLE_H
#define NMIXIN_HASHABLE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// System
#include <stddef.h>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// "None" hash
//
// The hash of nothing is always kNHashNone.
inline constexpr size_t kNHashNone                          = 0;





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// STL specialisations
//
// Specialisations of std::equal_to, std::hash, and std::less to allow
// NMixinHashable to be used as keys in STL associative containers.
#define NHASHABLE_STD_EQUAL_TO(T)                                   \
	namespace std                                                   \
	{                                                               \
	template<>                                                      \
	struct equal_to<T>                                              \
	{                                                               \
		size_t operator()(const T& objectA, const T& objectB) const \
		{                                                           \
			size_t hashA = objectA.GetHash();                       \
			size_t hashB = objectB.GetHash();                       \
																	\
			/* Compare the hashes */                                \
			if (hashA != hashB)                                     \
			{                                                       \
				return false;                                       \
			}                                                       \
																	\
			/* Compare the objects */                               \
			return objectA == objectB;                              \
		}                                                           \
	};                                                              \
	}                                                               \
																	\
	static_assert(true)


#define NHASHABLE_STD_HASH(T)                               \
	namespace std                                           \
	{                                                       \
	template<>                                              \
	struct hash<T>                                          \
	{                                                       \
		size_t operator()(const T& theObject) const         \
		{                                                   \
			return theObject.GetHash();                     \
		}                                                   \
	};                                                      \
	}                                                       \
															\
	static_assert(true)


#define NHASHABLE_STD_LESS(T)                                       \
	namespace std                                                   \
	{                                                               \
	template<>                                                      \
	struct less<T>                                                  \
	{                                                               \
		size_t operator()(const T& objectA, const T& objectB) const \
		{                                                           \
			size_t hashA = objectA.GetHash();                       \
			size_t hashB = objectB.GetHash();                       \
																	\
			/* Compare the hashes */                                \
			if (hashA != hashB)                                     \
			{                                                       \
				return hashA < hashB;                               \
			}                                                       \
																	\
			/* Compare the objects */                               \
			return objectA < objectB;                               \
		}                                                           \
	};                                                              \
	}                                                               \
																	\
	static_assert(true)





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<class T>
class NMixinHashable
{
public:
	// Get the hash value
	//
	// Hash values are runtime-specific and must not be serialised.
	size_t                              GetHash() const;


	// Clear the hash
	void                                ClearHash();


public:
	// Provide the hash
	size_t                              HashGet() const;
	size_t                              HashUpdate();
	void                                HashClear();
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinHashable.inl"


#endif // NMIXIN_HASHABLE_H
