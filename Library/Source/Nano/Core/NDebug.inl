/*	NAME:
		NDebug.inl

	DESCRIPTION:
		Debug logging and assrtions.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------





//=============================================================================
//		_nn_invoke_assertion : Invoke an assertion.
//-----------------------------------------------------------------------------
//		Note : Invoking via forwarding lambda allows use within constexpr functions.
//-----------------------------------------------------------------------------
#if defined(__cplusplus)

template <typename T>
inline void _nn_forward_assertion(T&& theAction) noexcept
{
	std::forward<T>(theAction)();
}

	#define _nn_invoke_assertion(...)                       \
		_nn_forward_assertion([]() {                        \
			__VA_ARGS__                                     \
		})

#else

	#define _nn_invoke_assertion(...)                       \
		do                                                  \
		{                                                   \
			__VA_ARGS__                                     \
		} while (false)

#endif // defined(__cplusplus)





//=============================================================================
//		_nn_has_assignment : Does an expression contain an assignment?
//-----------------------------------------------------------------------------
#if defined(__cplusplus)

template <size_t N>
constexpr bool _nn_has_assignment(char const (&theChars)[N])
{


	// Get the state we need
	char prevChar = '?';
	char theChar  = '?';



	// Scan the string
	for (size_t n = 1; n < (N - 1); n++)
	{
		prevChar = theChar;
		theChar  = theChars[n];


		// Reject '++' and '--'
		if (theChar == '+' || theChar == '-')
		{
			if (prevChar == theChar)
			{
				return true;
			}
		}


		// Reject '=', except for '==', '<=', '>=', and '!='
		else if (theChar == '=')
		{
			if (theChars[n + 1] == '=')
			{
				n++;
				continue;
			}

			if (prevChar == '>' || prevChar == '<' || prevChar == '!')
			{
				continue;
			}

			return true;
		}
	}

	return false;
}

#else

	#define _nn_has_assignment(...)                         false

#endif // defined(__cplusplus)





//=============================================================================
//		_nn_validate_condition : Is an assert condition valid?
//-----------------------------------------------------------------------------
#if defined(__cplusplus)

	#define _nn_validate_condition(_condition)                  \
		do                                                      \
		{                                                       \
			static_assert(!_nn_has_assignment(#_condition));    \
																\
		} while (false)

#else

	#define _nn_validate_condition(_condition)

#endif // defined(__cplusplus)
