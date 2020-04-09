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
#		Copyright (c) 2006-2020, refNum Software
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
import array
import datetime
import lldb
import math
import os
import struct
import sys





#==============================================================================
#		Constants
#------------------------------------------------------------------------------
# Nano
kNNotFound													= 18446744073709551615

kNTimeNanosecond											= 1.0 / 1000000000.0
kNanoEpochTo1970											= 978307200


kNDataFlagIsLarge											= 0b10000000
kNDataFlagSmallSizeMask										= 0b00011111


kNStringFlagIsLarge											= 0b10000000
kNStringFlagIsSmallUTF16									= 0b01000000
kNStringFlagSmallSizeMask									= 0b00011111


NFileAccess_Read											= 0
NFileAccess_Write											= 1
NFileAccess_ReadWrite										= 2

NFileAccessNames = {
	NFileAccess_Read      : "Read",
	NFileAccess_Write     : "Write",
	NFileAccess_ReadWrite : "ReadWrite",
}


NStringEncoding_Unknown										= 0
NStringEncoding_UTF8										= 1
NStringEncoding_UTF16										= 2
NStringEncoding_UTF32										= 3
NStringEncoding_UTF16BE										= 4
NStringEncoding_UTF16LE										= 5
NStringEncoding_UTF32BE										= 6
NStringEncoding_UTF32LE										= 7
NStringEncoding_ASCII										= 8
NStringEncoding_MacRoman									= 9
NStringEncoding_ISOLatin1									= 10
NStringEncoding_WindowsLatin1								= 11

NStringEncodings = {
	NStringEncoding_Unknown			: { "size" : 1, "name" : "undefined"	},
	NStringEncoding_UTF8			: { "size" : 1, "name" : "utf_8"		},
	NStringEncoding_UTF16			: { "size" : 2, "name" : "utf_16"		},
	NStringEncoding_UTF32			: { "size" : 4, "name" : "utf_32"		},
	NStringEncoding_UTF16BE			: { "size" : 2, "name" : "utf_16_be"	},
	NStringEncoding_UTF16LE			: { "size" : 2, "name" : "utf_16_le"	},
	NStringEncoding_UTF32BE			: { "size" : 4, "name" : "utf_32_be"	},
	NStringEncoding_UTF32LE			: { "size" : 4, "name" : "utf_32_le"	},
	NStringEncoding_ASCII			: { "size" : 1, "name" : "ascii"		},
	NStringEncoding_MacRoman		: { "size" : 1, "name" : "mac_roman"	},
	NStringEncoding_ISOLatin1		: { "size" : 1, "name" : "latin_1"		},
	NStringEncoding_WindowsLatin1	: { "size" : 1, "name" : "cp1252"		}
}


# Inscrutable objects
#
# lldb will invoke summariser views on objects that are in scope but are
# yet to be initialised.
#
# Parsing these will typically produce an exception so our _Show methods
# assume any exceptions are an attempt to display an inscrutable object.
#
#
# If this value is ever shown for an object that has been initialised then
# this typically indicates an error in the summariser.
#
# Remove the exception handling and debug the summariser to find the cause.
kInscrutable												= u"\u2754"





#==============================================================================
#		getMemberUInt : Get member as an unsigned integer.
#------------------------------------------------------------------------------
def getMemberUInt(theValue, theName):

	return theValue.GetChildMemberWithName(theName).GetValueAsUnsigned(0)





#==============================================================================
#		getMemberFloat : Get member as a float.
#------------------------------------------------------------------------------
def getMemberFloat(theValue, theName):

	return float(theValue.GetChildMemberWithName(theName).GetValue())





#==============================================================================
#		getPathValue : Get a path as an SBValue.
#------------------------------------------------------------------------------
def getPathValue(theValue, thePath):

	return theValue.GetValueForExpressionPath(thePath)





#==============================================================================
#		getPathUInt : Get a path as an unsigned integer.
#------------------------------------------------------------------------------
def getPathUInt(theValue, thePath):

	return theValue.GetValueForExpressionPath(thePath).GetValueAsUnsigned(0)





#==============================================================================
#		getPathData : Get a path as an SBData.
#------------------------------------------------------------------------------
def getPathData(theValue, thePath):

	return theValue.GetValueForExpressionPath(thePath).GetData()





#==============================================================================
#		getPathNString : Get a path as an NString.
#------------------------------------------------------------------------------
def getPathNString(theValue, thePath):

	theString = getPathValue(theValue, thePath)

	return NString_Show(theString, None)





#==============================================================================
#		getMemory : Get bytes from an address.
#------------------------------------------------------------------------------
def getMemory(thePtr, theOffset, theSize):

	theErr     = lldb.SBError()
	theProcess = thePtr.GetProcess()

	theAddresss = thePtr.GetValueAsUnsigned() + theOffset
	theBytes    = theProcess.ReadMemory(theAddresss, theSize, theErr)

	return theBytes





#==============================================================================
#		getFileAccessName : Get the name of a file access mode.
#------------------------------------------------------------------------------
def getFileAccessName(theAccess):

	return NFileAccessNames.get(theAccess)





#==============================================================================
#		getEncodingSize : Get the code unit size of an encoding.
#------------------------------------------------------------------------------
def getEncodingSize(theEncoding):

	return NStringEncodings.get(theEncoding).get("size")





#==============================================================================
#		getEncodingName : Get the Python name of an encoding.
#------------------------------------------------------------------------------
def getEncodingName(theEncoding):

	return NStringEncodings.get(theEncoding).get("name")





#==============================================================================
#		getText : Convert bytes to text.
#------------------------------------------------------------------------------
def getText(theBytes, theEncoding):

	pyEncoding = getEncodingName(theEncoding)
	theText    = theBytes.decode(pyEncoding)
	
	return theText





#==============================================================================
#		SBData_GetBytes : Get bytes from an SBData.
#------------------------------------------------------------------------------
def SBData_GetBytes(sbData, theOffset, theSize):

	sbErr    = lldb.SBError()
	theBytes = sbData.ReadRawData(sbErr, theOffset, theSize)

	return theBytes





#==============================================================================
#		NData_GetBytes : Get the bytes from an NData.
#------------------------------------------------------------------------------
def NData_GetBytes(theData):

	theFlags = getPathUInt(theData, "->mData.theFlags")
	isLarge  = (theFlags & kNDataFlagIsLarge)

	if (isLarge):
		theOffset = getPathUInt(theData, "->mData.Large.theSlice.mLocation")
		theSize   = getPathUInt(theData, "->mData.Large.theSlice.mSize")
		dataPtr   = getPathValue(theData, "->mData.Large.theState->theData")
		theBytes  = getMemory(dataPtr, theOffset, theSize)

	else:
		theSize  = (theFlags & kNDataFlagSmallSizeMask)
		theBytes = bytes(getPathData(theData, "->mData.Small.theData").uint8s[0:theSize])

	return theBytes





#==============================================================================
#		NData_Show : Show an NData.
#------------------------------------------------------------------------------
def NData_Show(theData, theInfo):

	try:
		theFlags = getPathUInt(theData, "->mData.theFlags")
		isLarge  = (theFlags & kNDataFlagIsLarge)

		if (isLarge):
			theSize = getPathUInt(theData, "->mData.Large.theSlice.mSize")
		
			if (theSize != 0):
				theOffset = getPathUInt( theData, "->mData.Large.theSlice.mLocation")
				thePtr    = getPathValue(theData, "->mData.Large.theState->theData")
				theInfo   = ", data=" + hex(thePtr.GetValueAsUnsigned() + theOffset)

		else:
			theSize = (theFlags & kNDataFlagSmallSizeMask)
		
			if (theSize != 0):
				sbData  = getPathData(theData, "->mData.Small.theData")
				theInfo = ", data={0x" + ', 0x'.join(format(x, '02X') for x in sbData.uint8s[0:theSize]) + "}"

		if (theSize == 0):
			return "size=0"
		else:
			return "size=" + str(theSize) + theInfo

	except:
		return kInscrutable





#==============================================================================
#		NFile_Show : Show an NFile.
#------------------------------------------------------------------------------
def NFile_Show(theFile, theInfo):

	thePath = getPathNString(theFile, "->mInfo.mPath.mPath")

	return thePath





#==============================================================================
#		NFileHandle_Show : Show an NFileHandle.
#------------------------------------------------------------------------------
def NFileHandle_Show(fileHnd, theInfo):

	thePath    = getPathNString(fileHnd, "->mPath.mPath")
	accessName = ""

	if (thePath):
		fileAccess = getPathUInt(fileHnd, "->mAccess")
		accessName = " (" + getFileAccessName(fileAccess) + ")"

	return thePath + accessName





#==============================================================================
#		NFileInfo_Show : Show an NFileInfo.
#------------------------------------------------------------------------------
def NFileInfo_Show(fileInfo, theInfo):

	return getPathNString(fileInfo, "->mPath.mPath")





#==============================================================================
#		NFilePath_Show : Show an NFilePath.
#------------------------------------------------------------------------------
def NFilePath_Show(filePath, theInfo):

	return getPathNString(filePath, "->mPath")




#==============================================================================
#		NRange_Show : Show an NRange.
#------------------------------------------------------------------------------
def NRange_Show(theRange, theInfo):

	try:
		theLocation = getMemberUInt(theRange, "mLocation")
		theSize     = getMemberUInt(theRange, "mSize")
	
		theLocation = "kNNotFound" if (theLocation == kNNotFound) else str(theLocation)
		theSize     = "kNNotFound" if (theSize     == kNNotFound) else str(theSize)

		return "location=" + theLocation + ", size=" + theSize

	except:
		return kInscrutable





#==============================================================================
#		NString_Show : Show an NString.
#------------------------------------------------------------------------------
def NString_Show(theString, theInfo):

	try:
		theFlags = getPathUInt(theString, "->mString.theFlags")
		isLarge  = (theFlags & kNStringFlagIsLarge)
		theInfo  = ""

		if (isLarge):
			# Decode the string
			#
			# Large strings are stored as a null-terminated buffer.
			theEncoding = getPathUInt(theString, "->mString.Large.theState->stringData.theEncoding")
			theData     = getPathValue(theString, "->mString.Large.theState->stringData.theData")

			nullSize = getEncodingSize(theEncoding)
			theBytes = NData_GetBytes(theData)

			theBytes = theBytes[:-nullSize]
			theInfo  = getText(theBytes, theEncoding)



			# Extract sub-strings
			#
			# A sub-string must be converted to UTF32, sliced by code points, then re-decoded.
			stringSize = getPathUInt(theString, "->mString.Large.theState->theSize")
			sliceSize  = getPathUInt(theString, "->mString.Large.theSlice.mSize")

			if (sliceSize != stringSize):
				sliceOffset = getPathUInt(theString, "->mString.Large.theSlice.mLocation")

				bytesUTF32 = theInfo.encode("utf-32")	
				theFormat  = "=" + "I" * int(len(bytesUTF32) / 4)
				codePoints = struct.unpack(theFormat, bytesUTF32)

				sliceFirst = sliceOffset + 1
				sliceLast  = sliceFirst + sliceSize
				codePoints = codePoints[sliceFirst:sliceLast]

				theFormat  = "=" + "I" * len(codePoints)
				bytesUTF32 = struct.pack(theFormat, *codePoints)
				theInfo    = getText(bytesUTF32, NStringEncoding_UTF32)

		else:
			theSize = (theFlags & kNStringFlagSmallSizeMask)

			if (theSize != 0):
				sbData  = getPathData(theString, "->mString.Small.theData")
				isUTF16 = (theFlags & kNStringFlagIsSmallUTF16)

				if (isUTF16):
					theEncoding = NStringEncoding_UTF16
				else:
					theEncoding = NStringEncoding_UTF8

				byteSize = theSize * getEncodingSize(theEncoding)
				theBytes = SBData_GetBytes(sbData, 0, byteSize)
				theInfo  = getText(theBytes, theEncoding)

		return '"' + theInfo + '"'

	except:
		return kInscrutable





#==============================================================================
#		NTime_Show : Show an NTime.
#------------------------------------------------------------------------------
def NTime_Show(theTime, theInfo):

	unixSecs = getMemberFloat(theTime, "mValue") + kNanoEpochTo1970
	unixFrac = unixSecs - math.floor(unixSecs)

	strTime = datetime.datetime.utcfromtimestamp(unixSecs).strftime("%Y-%m-%d %H:%M:%S.")
	strSecs = "{:.9g}".format(unixFrac)

	if (strSecs != "0"):
		strSecs = strSecs[2:]

	return strTime + strSecs





#==============================================================================
#		loadNano : Load the Nano summarisers.
#------------------------------------------------------------------------------
def loadNano(theDebugger):

	theDebugger.HandleCommand('type summary add -w Nano -F Nano.NData_Show          NData')
	theDebugger.HandleCommand('type summary add -w Nano -F Nano.NFile_Show          NFile')
	theDebugger.HandleCommand('type summary add -w Nano -F Nano.NFileHandle_Show    NFileHandle')
	theDebugger.HandleCommand('type summary add -w Nano -F Nano.NFileInfo_Show      NFileInfo')
	theDebugger.HandleCommand('type summary add -w Nano -F Nano.NFilePath_Show      NFilePath')
	theDebugger.HandleCommand('type summary add -w Nano -F Nano.NRange_Show         NRange')
	theDebugger.HandleCommand('type summary add -w Nano -F Nano.NString_Show        NString')
	theDebugger.HandleCommand('type summary add -w Nano -F Nano.NTime_Show          NTime')
	theDebugger.HandleCommand('type category enable Nano')





#==============================================================================
#		__lldb_init_module : Entry point.
#------------------------------------------------------------------------------
def __lldb_init_module(theDebugger, theInfo):

	loadNano(theDebugger)


