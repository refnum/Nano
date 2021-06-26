/*	NAME:
		NCFObject.inl

	DESCRIPTION:
		CFTypeRef wrapper.

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
// System
#include <algorithm>





//=============================================================================
//		NCFObject::NCFObject : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
inline NCFObject<T>::NCFObject()
	: mObject(nullptr)
{
}





//=============================================================================
//		NCFObject::~NCFObject : Destructor.
//-----------------------------------------------------------------------------
template<typename T>
inline NCFObject<T>::~NCFObject()
{


	// Clean up
	CFSafeRelease(mObject);
}





//=============================================================================
//		NCFObject::NCFObject : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
inline NCFObject<T>::NCFObject(const NCFObject<T>& otherObject)
	: mObject(otherObject.mObject)
{


	// Initialise ourselves
	CFSafeRetain(mObject);
}





//=============================================================================
//		NCFObject::operator= : Assignment operator.
//-----------------------------------------------------------------------------
template<typename T>
inline NCFObject<T>& NCFObject<T>::operator=(const NCFObject<T>& otherObject)
{


	// Assign the string
	if (this != &otherObject)
	{
		CFSafeRelease(mObject);

		mObject = otherObject.mObject;

		CFSafeRetain(mObject);
	}

	return *this;
}





//=============================================================================
//		NCFObject::NCFObject : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
inline NCFObject<T>::NCFObject(NCFObject<T>&& otherObject)
	: mObject(std::exchange(otherObject.mObject, nullptr))
{
}





//=============================================================================
//		NCFObject::operator= : Assignment operator.
//-----------------------------------------------------------------------------
template<typename T>
inline NCFObject<T>& NCFObject<T>::operator=(NCFObject<T>&& otherObject)
{


	// Move the string
	if (this != &otherObject)
	{
		mObject = std::exchange(otherObject.mObject, nullptr);
	}

	return *this;
}





//=============================================================================
//		NCFObject::IsValid : Is the object valid?
//-----------------------------------------------------------------------------
template<typename T>
inline bool NCFObject<T>::IsValid() const
{


	// Check our state
	return mObject != nullptr;
}





//=============================================================================
//		NCFObject::Set : Set the object.
//-----------------------------------------------------------------------------
template<typename T>
inline bool NCFObject<T>::Set(T cfObject)
{


	// Set the object
	CFSafeRelease(mObject);
	mObject = cfObject;

	return mObject != nullptr;
}





//=============================================================================
//		NCFObject::operator T : Cast operator.
//-----------------------------------------------------------------------------
template<typename T>
inline NCFObject<T>::operator T() const
{


	// Get the object
	return mObject;
}
