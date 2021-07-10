/*	NAME:
		NLogOutput.h

	DESCRIPTION:
		Log output.

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
#ifndef NLOG_OUTPUT_H
#define NLOG_OUTPUT_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
struct NLogMessage;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NLogOutput
{
public:
										NLogOutput()          = default;
	virtual                            ~NLogOutput()          = default;

										NLogOutput(const NLogOutput& otherOutput) = delete;
	NLogOutput&                         operator=( const NLogOutput& otherOutput) = delete;

										NLogOutput(NLogOutput&& otherOutput) = delete;
	NLogOutput&                         operator=( NLogOutput&& otherOutput) = delete;


	// Log a message
	virtual void                        LogMessage(const NLogMessage& theMsg) = 0;
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NLogOutputConsole final : public NLogOutput
{
public:
										NLogOutputConsole()                   = default;
	virtual                            ~NLogOutputConsole() override          = default;

										NLogOutputConsole(const NLogOutputConsole& otherOutput) = delete;
	NLogOutputConsole&                  operator=(        const NLogOutputConsole& otherOutput) = delete;

										NLogOutputConsole(NLogOutputConsole&& otherOutput) = delete;
	NLogOutputConsole&                  operator=(        NLogOutputConsole&& otherOutput) = delete;


	// Log a message
	void                                LogMessage(const NLogMessage& theMsg) override;
};



#endif // NLOG_OUTPUT_H
