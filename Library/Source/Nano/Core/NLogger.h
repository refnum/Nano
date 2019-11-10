/*	NAME:
		NLogger.h

	DESCRIPTION:
		Debug log.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
		<http://www.refnum.com/>

		All rights reserved.

		This software is distributed under the BSD-3-Clause license.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:

			o Redistributions of source code must retain the above
			copyright notice, this list of conditions and the following
			disclaimer.

			o Redistributions in binary form must reproduce the above
			copyright notice, this list of conditions and the following
			disclaimer in the documentation and/or other materials
			provided with the distribution.

			o Neither the name of refNum Software nor the names of its
			contributors may be used to endorse or promote products derived
			from this software without specific prior written permission.

		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
#ifndef NLOGGER_HDR
#define NLOGGER_HDR
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NLogger
{
public:
	NLogger();
	~NLogger() = default;

	NLogger(const NLogger&) = delete;
	NLogger& operator=(const NLogger&) = delete;

	NLogger(NLogger&&) = delete;
	NLogger& operator=(NLogger&&) = delete;


	// Get the instance
	static NLogger* Get();
};


#endif // NLOGGER_HDR
