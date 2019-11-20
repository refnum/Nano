#!/usr/local/bin/python -w
#==============================================================================
#	NAME:
#		Nano.py
#
#	DESCRIPTION:
#		lldb summarisers for Nano types.
#
#		Import into your ~/.lldbinit to enable:
#
#			command script import /path/to/Nano.py
#
#	COPYRIGHT:
#		Copyright (c) 2006-2019, refNum Software
#		All rights reserved.
#
#		Redistribution and use in source and binary forms, with or without
#		modification, are permitted provided that the following conditions
#		are met:
#		
#		1. Redistributions of source code must retain the above copyright
#		notice, this list of conditions and the following disclaimer.
#		
#		2. Redistributions in binary form must reproduce the above copyright
#		notice, this list of conditions and the following disclaimer in the
#		documentation and/or other materials provided with the distribution.
#		
#		3. Neither the name of the copyright holder nor the names of its
#		contributors may be used to endorse or promote products derived from
#		this software without specific prior written permission.
#		
#		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#==============================================================================
#		Imports
#------------------------------------------------------------------------------
import lldb
import os
import sys





#=============================================================================
#		SummariseNData : Summarise an NData.
#-----------------------------------------------------------------------------
def SummariseNData(theValue, theInfo):

	# Get the size
	kSmallSizeBit   = 0b00000001
	kSmallSizeMask  = 0b11111000
	kSmallSizeShift = 3

	sizeFlags = theValue.GetValueForExpressionPath("->mSmall.sizeFlags").GetValueAsUnsigned(0)

	if (sizeFlags & kSmallSizeBit):
		theSize = (sizeFlags & kSmallSizeMask) >> kSmallSizeShift;
	else:
		theSize = theValue.GetValueForExpressionPath("->mShared.theSlice.mSize").GetValueAsUnsigned(0)


	# Get the summary
	if (theSize == 1):
		return "1 byte"
	else:
		return str(theSize) + " bytes"





#==============================================================================
#		loadNano : Load the Nano summarisers.
#------------------------------------------------------------------------------
def loadNano(theDebugger):

	theDebugger.HandleCommand('type summary add -w Nano -F Nano.SummariseNData NData')





#==============================================================================
#		__lldb_init_module : Entry point.
#------------------------------------------------------------------------------
def __lldb_init_module(theDebugger, theInfo):
	loadNano(theDebugger)


