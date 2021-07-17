/*	NAME:
		NXMLEncoder.h

	DESCRIPTION:
		XML encoder.

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
#ifndef NXMLENCODER_H
#define NXMLENCODER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NProgressable.h"
#include "NXMLNode.h"
#include "NXMLParser.h"





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NXMLEncoder
{
public:
										NXMLEncoder();
									   ~NXMLEncoder() = default;

										NXMLEncoder(const NXMLEncoder& otherEncoder) = delete;
	NXMLEncoder&                        operator=(  const NXMLEncoder& otherEncoder) = delete;

										NXMLEncoder(NXMLEncoder&& otherEncoder) = delete;
	NXMLEncoder&                        operator=(  NXMLEncoder&& otherEncoder) = delete;


	// Encode/decode an XML document
	//
	// Returns an empty string/null node if an error occurs during parsing.
	NString                             Encode(const NSharedXMLNode& theNode, NStatus* parseErr = nullptr);

	NSharedXMLNode                      Decode(const NString&           theXML,
											   NStatus*                 parseErr    = nullptr,
											   const NFunctionProgress& theProgress = nullptr);


private:
	NString                             EncodeNode(      const NSharedXMLNode& theNode, const NString& theIndent);
	NString                             EncodeDocument(  const NSharedXMLNode& theNode, const NString& theIndent);
	NString                             EncodeDocType(   const NSharedXMLNode& theNode);
	NString                             EncodeElement(   const NSharedXMLNode& theNode, const NString& theIndent);
	NString                             EncodeAttributes(const NSharedXMLNode& theNode);
	NString                             EncodeComment(   const NSharedXMLNode& theNode);
	NString                             EncodeText(      const NSharedXMLNode& theNode);
	NString                             EncodeCData(     const NSharedXMLNode& theNode);

	bool                                DecodeDocType(     const NString& theName, const NXMLDocumentType& theInfo);
	bool                                DecodeElementStart(const NString& theName, const NDictionary& theAttributes);
	bool                                DecodeElementEnd(  const NString& theName);
	bool                                DecodeComment(     const NString& theValue);
	bool                                DecodeText(        const NString& theValue, bool isCData);

	bool                                ContainsElements(const NVectorSharedXMLNode& theNodes);

	NSharedXMLNode                      GetDecodeParent();


private:
	NSharedXMLNode                      mDecodeRoot;
	NVectorSharedXMLNode                mDecodeElements;
};



#endif // NXMLENCODER_H
