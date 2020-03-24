/*	NAME:
		NStdContainer.h

	DESCRIPTION:
		STL container extensions.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
#ifndef NSTD_CONTAINER_H
#define NSTD_CONTAINER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include <vector>

namespace nstd
{





//=============================================================================
//		nstd::vector : Create a vector from a C array of values.
//-----------------------------------------------------------------------------
template<class T, size_t numValues>
std::vector<T> vector(const T (&theValues)[numValues])
{
	return std::vector<T>(theValues, theValues + numValues);
}





//=============================================================================
//		nstd::vector : Create a vector from a pointer to values.
//-----------------------------------------------------------------------------
template<class T>
std::vector<T> vector(size_t numValues, const T* theValues)
{
	return std::vector<T>(theValues, theValues + numValues);
}





//=============================================================================
//		nstd::vector : Create a vector from a list of values.
//-----------------------------------------------------------------------------
template<typename T>
void _make_vector(std::vector<T>&)
{
}

template<typename T, typename P>
void _make_vector(std::vector<T>& theVector, P&& theParam)
{
	static_assert(std::is_convertible<P, T>::value, "Param type cannot be stored in vector");

	theVector.push_back(static_cast<T>(std::forward<P>(theParam)));
}

template<typename T, typename P, typename... Args>
void _make_vector(std::vector<T>& theVector, P&& theParam, Args&&... theArgs)
{
	nstd::_make_vector(theVector, std::forward<P>(theParam));
	nstd::_make_vector(theVector, std::forward<Args>(theArgs)...);
}

template<typename T, typename... Args>
std::vector<T> vector(Args&&... theArgs)
{
	std::vector<T> theVector;
	theVector.reserve(sizeof...(theArgs));

	nstd::_make_vector(theVector, std::forward<Args>(theArgs)...);
	return theVector;
}



}    // namespace nstd


#endif // NSTD_CONTAINER_H
