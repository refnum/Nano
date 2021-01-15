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
import re
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


NAnyTypes = {
	# Stored at 8-byte __ptr field
	"5NData"			: "*((NData			*) this->__s.__ptr)",
	"7NString"			: "*((NString		*) this->__s.__ptr)",
	"11NDictionary"		: "*((NDictionary	*) this->__s.__ptr)",


	# Stored in 8-byte __ptr field
	"b"					: "*((bool			*) &this->__s.__ptr)",
	"h"					: "*((uint8_t		*) &this->__s.__ptr)",
	"t"					: "*((uint16_t		*) &this->__s.__ptr)",
	"j"					: "*((uint32_t		*) &this->__s.__ptr)",
	"y"					: "*((uint64_t		*) &this->__s.__ptr)",
	"a"					: "*((int8_t		*) &this->__s.__ptr)",
	"s"					: "*((int16_t		*) &this->__s.__ptr)",
	"i"					: "*((int32_t		*) &this->__s.__ptr)",
	"x"					: "*((int64_t		*) &this->__s.__ptr)",
	"f"					: "*((float32_t		*) &this->__s.__ptr)",
	"d"					: "*((float64_t		*) &this->__s.__ptr)",


	# Stored in 24-byte __buf field
	"5NTime"			: "*((NTime			*) &this->__s.__buf)",
	"6NArray"			: "*((NArray		*) &this->__s.__buf)",
	"7NNumber"			: "*((NNumber		*) &this->__s.__buf)"
}


NNumberTypes = {
	"unsigned long long"	: "uint64_t",
	"long long"				: "int64_t",
	"double"				: "float64_t"
}



# Inscrutable objects
#
# lldb will invoke summariser views on objects that are in scope but are
# yet to be initialised.
#
# Parsing these will typically produce an exception so our summary methods
# assume any exceptions are an attempt to display an inscrutable object.
#
#
# If this value is ever shown for an object that has been initialised then
# this typically indicates an error in the summariser.
#
# Remove the exception handling and debug the summariser to find the cause.
kInscrutable												= u"\u2754"





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
#		getMemberUInt : Get a member as an unsigned integer.
#------------------------------------------------------------------------------
def getMemberUInt(sbValue, theName):

	return sbValue.GetChildMemberWithName(theName).GetValueAsUnsigned()





#==============================================================================
#		getMemberFloat : Get a member as a float.
#------------------------------------------------------------------------------
def getMemberFloat(sbValue, theName):

	return float(sbValue.GetChildMemberWithName(theName).GetValue())





#==============================================================================
#		getExpressionPathSBValue : Get an expression path as an SBValue.
#------------------------------------------------------------------------------
def getExpressionPathSBValue(sbValue, thePath):

	return sbValue.GetValueForExpressionPath(thePath)





#==============================================================================
#		getExpressionPathSBData : Get an expression path as an SBData.
#------------------------------------------------------------------------------
def getExpressionPathSBData(sbValue, thePath):

	return sbValue.GetValueForExpressionPath(thePath).GetData()





#==============================================================================
#		getExpressionPathUInt : Get an expression path as an unsigned integer.
#------------------------------------------------------------------------------
def getExpressionPathUInt(sbValue, thePath):

	return sbValue.GetValueForExpressionPath(thePath).GetValueAsUnsigned()





#==============================================================================
#		getMemoryBytes : Get bytes from an address.
#------------------------------------------------------------------------------
def getMemoryBytes(sbValue, theOffset, theSize):

	theProcess  = sbValue.GetProcess()
	theAddresss = sbValue.GetValueAsUnsigned() + theOffset
	theBytes    = theProcess.ReadMemory(theAddresss, theSize, lldb.SBError())

	return theBytes





#==============================================================================
#		getMemoryString : Get a C string from an address.
#------------------------------------------------------------------------------
def getMemoryString(sbValue, maxSize=128):

	theProcess  = sbValue.GetProcess()
	theAddresss = sbValue.GetValueAsUnsigned()
	theString   = theProcess.ReadCStringFromMemory(theAddresss, maxSize, lldb.SBError())
	
	return theString





#==============================================================================
#		getBytesString : Get a string from encoded bytes.
#------------------------------------------------------------------------------
def getBytesString(theBytes, theEncoding):

	pyEncoding = getEncodingName(theEncoding)
	theText    = theBytes.decode(pyEncoding)
	
	return theText





#==============================================================================
#		getSBDataBytes : Get bytes from an SBData.
#------------------------------------------------------------------------------
def getSBDataBytes(sbData, theOffset, theSize):

	theBytes = sbData.ReadRawData(lldb.SBError(), theOffset, theSize)

	return theBytes





#==============================================================================
#		getNDataBytes : Get the bytes from an NData.
#------------------------------------------------------------------------------
def getNDataBytes(theData):

	theFlags = getExpressionPathUInt(theData, "->mData.theFlags")
	isLarge  = (theFlags & kNDataFlagIsLarge)

	if (isLarge):
		theOffset = getExpressionPathUInt(   theData, "->mData.Large.theSlice.mLocation")
		theSize   = getExpressionPathUInt(   theData, "->mData.Large.theSlice.mSize")
		dataPtr   = getExpressionPathSBValue(theData, "->mData.Large.theState->theData")
		theBytes  = getMemoryBytes(dataPtr, theOffset, theSize)

	else:
		theSize  = (theFlags & kNDataFlagSmallSizeMask)
		theBytes = bytes(getExpressionPathSBData(theData, "->mData.Small.theData").uint8s[0:theSize])

	return theBytes





#==============================================================================
#		getFilePath : Get a file path.
#------------------------------------------------------------------------------
def getFilePath(theValue, thePath):

	theString = getExpressionPathSBValue(theValue, thePath)
	thePath   = NString_Summary(theString, None).strip('"')

	if (not thePath):
		thePath = "none"

	return thePath





#==============================================================================
#		NAny_Summary : NAny summary.
#------------------------------------------------------------------------------
def NAny_Summary(theObject, theInfo):

	try:
		# Check for validity
		theHandler = getMemberUInt(theObject, "__h")
		if (theHandler == 0):
			return "none";


		# Extract the type
		valueTypeID     = getMemoryString(theObject.EvaluateExpression("this->type().name()"))
		valueExpression = NAnyTypes.get(valueTypeID)

		if (valueExpression == None):
			theValue = "<unknown type '" + valueTypeID + "'>"

		else:
			theValue = str(theObject.EvaluateExpression(valueExpression))
			theValue = re.sub(r"\) \$\d+ = ", ") ", theValue);

		return theValue

	except:
		return kInscrutable





#==============================================================================
#		NArray_Summary : NArray summary.
#------------------------------------------------------------------------------
def NArray_Summary(theObject, theInfo):

	try:
		return "size=" + str(theObject.GetNumChildren())

	except:
		return kInscrutable





#==============================================================================
#		NArray_Contents : NArray contents.
#------------------------------------------------------------------------------
class NArray_Contents:

	def __init__(self, theArray, theInfo):
		self.theArray = theArray


	def num_children(self):
		return self.numChildren


	def has_children(self):
		return True


	def get_child_index(self, theName):
		try:
			return int(theName.lstrip('[').rstrip(']'))

		except:
			return -1


	def get_child_at_index(self, theIndex):
		if (theIndex < 0 or theIndex >= self.numChildren):
			return None

		try:
			theOffset = theIndex * self.dataSize
			return self.dataStart.CreateChildAtOffset('[' + str(theIndex) + ']', theOffset, self.dataType)

		except:
			return None


	def update(self):
		try:
			theTarget = lldb.debugger.GetSelectedTarget()

			self.numChildren = 0
			self.dataType    = theTarget.FindTypes('NAny').GetTypeAtIndex(0);
			self.dataSize    = self.dataType.GetByteSize()
			self.dataStart   = self.theArray.GetChildMemberWithName('__begin_')
			self.dataEnd     = self.theArray.GetChildMemberWithName('__end_')


			ptrStart = self.dataStart.GetValueAsUnsigned()
			ptrEnd   = self.dataEnd.GetValueAsUnsigned()

			if ptrStart != 0 and ptrEnd != 0 and ptrStart < ptrEnd:
				numBytes = ptrEnd - ptrStart

				if ((numBytes % self.dataSize) == 0):
					self.numChildren = int(numBytes / self.dataSize)

		except:
			self.dataType = None
			pass





#==============================================================================
#		NData_Summary : NData summary.
#------------------------------------------------------------------------------
def NData_Summary(theData, theInfo):

	try:
		theFlags = getExpressionPathUInt(theData, "->mData.theFlags")
		isLarge  = (theFlags & kNDataFlagIsLarge)

		if (isLarge):
			theSize = getExpressionPathUInt(theData, "->mData.Large.theSlice.mSize")
		
			if (theSize != 0):
				theOffset = getExpressionPathUInt(   theData, "->mData.Large.theSlice.mLocation")
				thePtr    = getExpressionPathSBValue(theData, "->mData.Large.theState->theData")
				theInfo   = ", data=" + hex(thePtr.GetValueAsUnsigned() + theOffset)

		else:
			theSize = (theFlags & kNDataFlagSmallSizeMask)
		
			if (theSize != 0):
				sbData  = getExpressionPathSBData(theData, "->mData.Small.theData")
				theInfo = ", data={0x" + ', 0x'.join(format(x, '02X') for x in sbData.uint8s[0:theSize]) + "}"


		if (theSize == 0):
			return "size=0"

		else:
			return "size=" + str(theSize) + theInfo

	except:
		return kInscrutable





#==============================================================================
#		NDictionary_Summary : NDictionary summary.
#------------------------------------------------------------------------------
def NDictionary_Summary(theObject, theInfo):

	try:
		return "size=" + str(theObject.GetNumChildren())

	except:
		return kInscrutable





#==============================================================================
#		NDictionary_Contents : NDictionary content.
#------------------------------------------------------------------------------
class NDictionary_Contents:

	def __init__(self, theDictionary, theInfo):
		self.theDictionary = theDictionary
		self.keyValues     = None


	def num_children(self):
		if (self.keyValues != None):
			return len(self.keyValues)

		return 0


	def has_children(self):
		return True


	def get_child_index(self, theName):
		for n in range(len(self.keyValues)):
			childKey = self.keyValues[n]["key"]
			if (childKey.GetSummary() == theName):
				return n
		
		return None


	def get_child_at_index(self, theIndex):
		theInfo    = self.keyValues[theIndex]
		childKey   = theInfo["key"]
		childValue = theInfo["value"]

		valueTypeID     = getMemoryString(childValue.EvaluateExpression("this->type().name()"))
		valueExpression = NAnyTypes.get(valueTypeID)

		if (valueExpression == None):
			return childValue

		return childValue.EvaluateExpression(valueExpression, lldb.SBExpressionOptions(), "[" + str(childKey.GetSummary() + "]"))


	def update(self):
		if self.keyValues == None:

			mapBuckets = self.theDictionary.GetValueForExpressionPath("->__table_.__bucket_list_.__ptr_")
			thePointer = struct.unpack("Q", getMemoryBytes(mapBuckets.AddressOf(), 0, 8))[0]
			isValid    = (thePointer != 0)

			if (not isValid):
				return

			try:
				self.keyValues = []
				theKeys        = self.theDictionary.EvaluateExpression("this->GetKeys()")
				theValues      = self.theDictionary.EvaluateExpression("this->GetValues()")

				for n in range(theKeys.GetNumChildren()):
					self.keyValues.append({ "key" : theKeys.GetChildAtIndex(n), "value" : theValues.GetChildAtIndex(n) })

			except:
				self.keyValues = None





#==============================================================================
#		NFile_Summary : NFile summary.
#------------------------------------------------------------------------------
def NFile_Summary(theFile, theInfo):

	return getFilePath(theFile, "->mInfo.mPath.mPath")





#==============================================================================
#		NFileHandle_Summary : NFileHandle summary.
#------------------------------------------------------------------------------
def NFileHandle_Summary(fileHnd, theInfo):

	thePath = getFilePath(fileHnd, "->mPath.mPath")

	if (thePath != "none"):
		fileAccess = getExpressionPathUInt(fileHnd, "->mAccess")
		thePath    = thePath + " (" + NFileAccessNames.get(fileAccess) + ")"

	return thePath





#==============================================================================
#		NFileInfo_Summary : NFileInfo summary.
#------------------------------------------------------------------------------
def NFileInfo_Summary(fileInfo, theInfo):

	return getFilePath(fileInfo, "->mPath.mPath")





#==============================================================================
#		NFilePath_Summary : NFilePath summary.
#------------------------------------------------------------------------------
def NFilePath_Summary(filePath, theInfo):

	return getFilePath(filePath, "->mPath")





#==============================================================================
#		NRange_Summary : NRange summary.
#------------------------------------------------------------------------------
def NRange_Summary(theRange, theInfo):

	try:
		theLocation = getMemberUInt(theRange, "mLocation")
		theSize     = getMemberUInt(theRange, "mSize")
	
		theLocation = "kNNotFound" if (theLocation == kNNotFound) else str(theLocation)
		theSize     = "kNNotFound" if (theSize     == kNNotFound) else str(theSize)

		return "location=" + theLocation + ", size=" + theSize

	except:
		return kInscrutable





#==============================================================================
#		NNumber_Summary : NNumber summary.
#------------------------------------------------------------------------------
def NNumber_Summary(theNumber, theInfo):

	try:
		theInfo  = str(theNumber.GetValueForExpressionPath("->mValue"))
		theMatch = re.search("Active Type = (.*?)\s+{.*?__value = (.*?)\)", theInfo, re.DOTALL)

		theType  = theMatch.group(1)
		theValue = theMatch.group(2)


		theType = NNumberTypes.get(theType)
		
		if (theType == "float64_t"):
			theValue = str(struct.unpack("d", struct.pack("Q", int(theValue)))[0])

		return "(" + theType + ") " + theValue

	except:
		return kInscrutable





#==============================================================================
#		NString_Summary : NString summary.
#------------------------------------------------------------------------------
def NString_Summary(theString, theInfo):

	try:
		theFlags = getExpressionPathUInt(theString, "->mString.theFlags")
		isLarge  = (theFlags & kNStringFlagIsLarge)
		theInfo  = ""

		if (isLarge):
			# Decode the string
			#
			# Large strings are stored as a null-terminated buffer.
			theEncoding = getExpressionPathUInt(   theString, "->mString.Large.theState->stringData.theEncoding")
			theData     = getExpressionPathSBValue(theString, "->mString.Large.theState->stringData.theData")

			nullSize = getEncodingSize(theEncoding)
			theBytes = getNDataBytes(theData)

			theBytes = theBytes[:-nullSize]
			theInfo  = getBytesString(theBytes, theEncoding)



			# Extract sub-strings
			#
			# A sub-string must be converted to UTF32, sliced by code points, then re-decoded.
			stringSize = getExpressionPathUInt(theString, "->mString.Large.theState->theSize")
			sliceSize  = getExpressionPathUInt(theString, "->mString.Large.theSlice.mSize")

			if (sliceSize != stringSize):
				sliceOffset = getExpressionPathUInt(theString, "->mString.Large.theSlice.mLocation")

				bytesUTF32 = theInfo.encode("utf-32")	
				theFormat  = "=" + "I" * int(len(bytesUTF32) / 4)
				codePoints = struct.unpack(theFormat, bytesUTF32)

				sliceFirst = sliceOffset + 1
				sliceLast  = sliceFirst + sliceSize
				codePoints = codePoints[sliceFirst:sliceLast]

				theFormat  = "=" + "I" * len(codePoints)
				bytesUTF32 = struct.pack(theFormat, *codePoints)
				theInfo    = getBytesString(bytesUTF32, NStringEncoding_UTF32)

		else:
			theSize = (theFlags & kNStringFlagSmallSizeMask)

			if (theSize != 0):
				sbData  = getExpressionPathSBData(theString, "->mString.Small.theData")
				isUTF16 = (theFlags & kNStringFlagIsSmallUTF16)

				if (isUTF16):
					theEncoding = NStringEncoding_UTF16
				else:
					theEncoding = NStringEncoding_UTF8

				byteSize = theSize * getEncodingSize(theEncoding)
				theBytes = getSBDataBytes(sbData, 0, byteSize)
				theInfo  = getBytesString(theBytes, theEncoding)

		return '"' + theInfo + '"'

	except:
		return kInscrutable





#==============================================================================
#		NThreadID_Summary : NThreadID summary.
#------------------------------------------------------------------------------
def NThreadID_Summary(threadID, theInfo):

	theID = getMemberUInt(threadID, "mValue")
	if (theID == 0):
		return "none";

	return hex(theID)





#==============================================================================
#		NTime_Summary : NTime summary.
#------------------------------------------------------------------------------
def NTime_Summary(theTime, theInfo):

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

	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NAny_Summary           NAny')
	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NArray_Summary         NArray')
	theDebugger.HandleCommand('type synthetic add -w Nano -l Nano.NArray_Contents        NArray')
	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NData_Summary          NData')
	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NDictionary_Summary    NDictionary')
	theDebugger.HandleCommand('type synthetic add -w Nano -l Nano.NDictionary_Contents   NDictionary')
	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NFile_Summary          NFile')
	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NFileHandle_Summary    NFileHandle')
	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NFileInfo_Summary      NFileInfo')
	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NFilePath_Summary      NFilePath')
	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NNumber_Summary        NNumber')
	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NRange_Summary         NRange')
	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NString_Summary        NString')
	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NThreadID_Summary      NThreadID')
	theDebugger.HandleCommand('type summary   add -w Nano -F Nano.NTime_Summary          NTime')
	theDebugger.HandleCommand('type category enable Nano')





#==============================================================================
#		__lldb_init_module : Entry point.
#------------------------------------------------------------------------------
def __lldb_init_module(theDebugger, theInfo):

	loadNano(theDebugger)


