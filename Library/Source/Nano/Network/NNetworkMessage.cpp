/*	NAME:
		NNetworkMessage.cpp

	DESCRIPTION:
		Network message.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNetworkMessage.h"





//============================================================================
//		Build constants
//----------------------------------------------------------------------------
#define DEBUG_COMPRESSION											0





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NCompression kCompressionType							= kNCompressionZLib;
static const NIndex       kCompressionHeaderTrim					= sizeof(UInt32) + sizeof(UInt32);





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
NBYTESWAP_BEGIN_NO_DECLARE(NMessageHeader)
	NBYTESWAP_B_SInt16		(msgType)
	NBYTESWAP_B_UInt8		(msgSrcID)
	NBYTESWAP_B_UInt8		(msgDstID)
	NBYTESWAP_B_UInt8		(msgAttributes)
	NBYTESWAP_B_UInt8		(reserved1)
	NBYTESWAP_B_UInt8		(reserved2)
	NBYTESWAP_B_UInt8		(reserved3)
	NBYTESWAP_B_UInt32		(bodySize)
NBYTESWAP_END





//============================================================================
//		NNetworkMessage::NNetworkMessage : Constructor.
//----------------------------------------------------------------------------
NNetworkMessage::NNetworkMessage(void)
{


	// Initialise ourselves
	memset(&mHeader, 0x00, sizeof(mHeader));

	mHeader.msgType  = kNMessageInvalidMsg;
	mHeader.msgSrcID = kNEntityInvalid;
	mHeader.msgDstID = kNEntityInvalid;
}





//============================================================================
//		NNetworkMessage::~NNetworkMessage : Destructor.
//----------------------------------------------------------------------------
NNetworkMessage::~NNetworkMessage(void)
{
}





//============================================================================
//		NNetworkMessage::GetType : Get the type.
//----------------------------------------------------------------------------
NMessageType NNetworkMessage::GetType(void) const
{


	// Get the type
	return(mHeader.msgType);
}





//============================================================================
//		NNetworkMessage::SetType : Set the type.
//----------------------------------------------------------------------------
void NNetworkMessage::SetType(NMessageType theType)
{


	// Set the type
	mHeader.msgType = theType;
}





//============================================================================
//		NNetworkMessage::GetSource : Get the source.
//----------------------------------------------------------------------------
NEntityID NNetworkMessage::GetSource(void) const
{


	// Get the source
	return(mHeader.msgSrcID);
}





//============================================================================
//		NNetworkMessage::SetSource : Set the source.
//----------------------------------------------------------------------------
void NNetworkMessage::SetSource(NEntityID theID)
{


	// Set the source
	mHeader.msgSrcID = theID;
}





//============================================================================
//		NNetworkMessage::GetDestination : Get the destination.
//----------------------------------------------------------------------------
NEntityID NNetworkMessage::GetDestination(void) const
{


	// Get the destination
	return(mHeader.msgDstID);
}





//============================================================================
//		NNetworkMessage::SetDestination : Set the destination.
//----------------------------------------------------------------------------
void NNetworkMessage::SetDestination(NEntityID theID)
{


	// Set the destination
	mHeader.msgDstID = theID;
}





//============================================================================
//		NNetworkMessage::GetPayload : Get the payload.
//----------------------------------------------------------------------------
NData NNetworkMessage::GetPayload(void) const
{	NData				theBody, theMsg;
	NDictionary			theProperties;
	NBitfield			theAttributes;
	NPropertyList		propertyList;
	NMessageHeader		theHeader;



	// Validate our state
	NN_ASSERT(mHeader.msgSrcID != kNEntityEveryone);
	NN_ASSERT(mHeader.msgDstID != kNEntityInvalid);



	// Get the state we need
	theAttributes = GetAttributes();



	// Encode the body
	theProperties = GetProperties();

	if (!theProperties.IsEmpty())
		{
		if (theProperties.GetSize() == 1 && theProperties.HasKey(kNMessageDataKey))
			{
			theBody        = theProperties.GetValueData(kNMessageDataKey);
			theAttributes &= ~kNMessageHasProperties;
			}
		else
			{
			theBody        = propertyList.Encode(theProperties);
			theAttributes |=  kNMessageHasProperties;
			}
		}

	if (!HasAttribute(kNMessageNeverCompress))
		{
		if (CompressBody(theBody))
			theAttributes |= kNMessageIsCompressed;
		}



	// Prepare the header
	theHeader               = mHeader;
	theHeader.msgAttributes = (UInt8) (theAttributes & kNMessageAttributeMask);
	theHeader.bodySize      = theBody.GetSize();



	// Build the payload
	theMsg.Reserve(   sizeof(theHeader) + theHeader.bodySize);
	theMsg.AppendData(sizeof(theHeader), &theHeader);
	theMsg.AppendData(theBody);

	NBYTESWAP_ENCODE(1, NMessageHeader, theMsg.GetData());

	return(theMsg);
}





//============================================================================
//		NNetworkMessage::SetPayload : Set the payload.
//----------------------------------------------------------------------------
void NNetworkMessage::SetPayload(const NMessageHeader &theHeader, const NData &theBody)
{	NDictionary			theProperties;
	NPropertyList		propertyList;
	NData				bodyData;



	// Validate our parameters
	NN_ASSERT(theHeader.msgSrcID != kNEntityEveryone);
	NN_ASSERT(theHeader.msgDstID != kNEntityInvalid);
	NN_ASSERT(theHeader.bodySize == (UInt32) theBody.GetSize());



	// Decode the body
	bodyData = theBody;
	
	if (theHeader.msgAttributes & kNMessageIsCompressed)
		bodyData = DecompressBody(bodyData);

	if (!bodyData.IsEmpty())
		{
		if (theHeader.msgAttributes & kNMessageHasProperties)
			theProperties = propertyList.Decode(     bodyData);
		else
			theProperties.SetValue(kNMessageDataKey, bodyData);
		}



	// Set the message
	mHeader = theHeader;

	SetAttributes(theHeader.msgAttributes, kNAttributesAll);
	SetProperties(theProperties);
}





#pragma mark private
//============================================================================
//		NNetworkMessage::CompressBody : Compress the body.
//----------------------------------------------------------------------------
bool NNetworkMessage::CompressBody(NData &theBody) const
{	NIndex					compressedSize, bodySize;
	NData					compressedBody;
	NDataCompressor			theCompressor;
	bool					didCompress;

#if DEBUG_COMPRESSION
	NDate	startTime;
	NTime	theTime;
#endif



	// Get the state we need
	bodySize = theBody.GetSize();
	
	if (bodySize == 0)
		return(false);



	// Compress the body
	//
	// The fixed parts of the header are trimmed to save space during transmit.
	compressedBody = theCompressor.Compress(theBody, kCompressionType);
	compressedSize = compressedBody.GetSize() - kCompressionHeaderTrim;
	didCompress    = (!compressedBody.IsEmpty() && compressedSize < bodySize);

	if (didCompress)
		theBody = NData(compressedSize, compressedBody.GetData(kCompressionHeaderTrim));



	// Debug hook
#if DEBUG_COMPRESSION
	theTime = NDate() - startTime;
	
	if (didCompress)
		NN_LOG("NNetworkMessage compressing body from %ld to %ld in %.3f", bodySize, compressedSize, theTime);
	else
		NN_LOG("NNetworkMessage unable to compress body from %ld in %.3f, set kNMessageNeverCompress?", bodySize, theTime);
#endif

	return(didCompress);
}





//============================================================================
//		NNetworkMessage::DecompressBody : Decompress the body.
//----------------------------------------------------------------------------
NData NNetworkMessage::DecompressBody(const NData &theBody) const
{	NData					finalBody, compressedData;
	NCompressionHeader		compressedHeader;
	NDataCompressor			theCompressor;
	NIndex					theOffset;

#if DEBUG_COMPRESSION
	NDate	startTime;
	NTime	theTime;
#endif



	// Get the state we need
	finalBody = theBody;
	theOffset = sizeof(compressedHeader) - kCompressionHeaderTrim;



	// Decompress the body
	//
	// To handle tainted data, we check as well as assert.
	NN_ASSERT(theBody.GetSize() >= theOffset);

	if (theBody.GetSize() >= theOffset)
		{
		// Get the state we need
		//
		// The header is trimmed to save space during transmit.
		memset(&compressedHeader, 0x00, sizeof(compressedHeader));

		compressedHeader.compression = kCompressionType;
		compressedHeader.origSize    = NSwapUInt32_BtoN( *((const UInt32 *) theBody.GetData()) );



		// Decompress the body
		compressedData = NData(theBody.GetSize() - theOffset, theBody.GetData(theOffset), false);
		finalBody      = theCompressor.Decompress(compressedData, &compressedHeader);



		// Debug hook
#if DEBUG_COMPRESSION
		theTime = NDate() - startTime;
	
		NN_LOG("NNetworkMessage decompressed body from %ld to %ld in %.3f", theBody.GetSize(), finalBody.GetSize(), theTime);
#endif
		}
	
	return(finalBody);
}








