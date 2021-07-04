/*	NAME:
		NCGColor.inl

	DESCRIPTION:
		CoreGraphics color.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------





//=============================================================================
//		NCGColor::NCGColor : Constructor.
//-----------------------------------------------------------------------------
inline NCGColor::NCGColor(const NColor& theColor)
{


	// Initialise ourselves
	SetColor(theColor);
}





//=============================================================================
//		NCGColor::GetColor : Get the color.
//-----------------------------------------------------------------------------
inline NColor NCGColor::GetColor() const
{


	// Get the color
	//
	// For now we can only convert to/from the RGB color space.
	CGColorRef cgColor = *this;

	bool isValid = (CGColorGetNumberOfComponents(cgColor) == 4);
	NN_EXPECT(isValid);

	NColor theColor;

	if (isValid)
	{
		const CGFloat* theComponents = CGColorGetComponents(cgColor);

		theColor.SetColor(float32_t(theComponents[0]),
						  float32_t(theComponents[1]),
						  float32_t(theComponents[2]),
						  float32_t(theComponents[3]));
	}

	return theColor;
}





//=============================================================================
//		NCGColor::SetColor : Set the color.
//-----------------------------------------------------------------------------
inline bool NCGColor::SetColor(const NColor& theColor)
{


	// Set the color
	float32_t theComponents[4]{};

	theColor.GetColor(theComponents[0], theComponents[1], theComponents[2], theComponents[3]);

	return Assign(CGColorCreateGenericRGB(CGFloat(theComponents[0]),
										  CGFloat(theComponents[1]),
										  CGFloat(theComponents[2]),
										  CGFloat(theComponents[3])));
}





//=============================================================================
//		NCGColor::GetDeviceGray : Get the gray color space.
//-----------------------------------------------------------------------------
inline NCGColorSpace NCGColor::GetDeviceGray()
{


	// Get the color space
	NCGColorSpace cgColorSpace;

	cgColorSpace.Assign(CGColorSpaceCreateDeviceGray());

	return cgColorSpace;
}





//=============================================================================
//		NCGColor::GetDeviceRGB : Get the RGB color space.
//-----------------------------------------------------------------------------
inline NCGColorSpace NCGColor::GetDeviceRGB()
{


	// Get the color space
	NCGColorSpace cgColorSpace;

	cgColorSpace.Assign(CGColorSpaceCreateDeviceRGB());

	return cgColorSpace;
}
