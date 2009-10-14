/*	NAME:
		NIBView.h

	DESCRIPTION:
		Nib view loader.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NIBVIEW_HDR
#define NIBVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIWindow.h"
#include "NHIView.h"





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Declare a .nib view
//
// Declares a member variable of the appropriate type, and binds it to the
// corresponding view within the .nib.
//
// Example usage:
//
//		NIB_VIEW('chek', NCheckBox,					ShowDepth);
//		NIB_VIEW('slid', NSlider,					Strength);
//
// By placing this sequence in the class declaration of a NIBWindow object, the
// window will have access to the following private state:
//
//		NCheckBox									*mShowDepth;
//		NSlider										*mStrength;
//
//		OSType										kViewShowDepth = 'chek';
//		OSType										kViewStrength  = 'slid';
//
// If the window object is an NWindowController, this state will be available
// for use by the time the window's InitializeSelf method is called.
#define NIB_VIEW(_signature, _class, _name)											\
																					\
	enum { kView ## _name							= _signature };					\
																					\
	class Register ## _name {														\
	public:																			\
		Register ## _name(void)														\
		{	HIViewID	theID = { _signature, 0 };									\
																					\
			_class::Register();														\
			NIBView::SetNextViewID(theID);											\
		}																			\
	};																				\
																					\
	Register ## _name								mRegister ## _name;				\
	NIBView_T<_class>								m ## _name





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NIBView {
public:
										NIBView(void);
	virtual								~NIBView(void);


	// Is the view valid?
	bool								IsValid(void) const;
	

	// Clear the view
	void								Clear(void);


	// Get the view
	NHIView								*GetView(void) const;


	// Get the view ID
	HIViewID							GetViewID(void) const;


	// Find the view
	OSStatus							FindView(NHIWindow *theWindow);


	// Set the next view ID
	static void							SetNextViewID(const HIViewID &theID);
	

protected:
	NHIView								*mView;
	HIViewID							mViewID;
	
	static HIViewID						mNextViewID;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
template <class T> class NIBView_T : public NIBView {
public:
	// Operators
	inline 								operator T*(void) { return( GetView()); }
	inline T*							operator ->(void) { return( GetView()); }
	inline T&							operator * (void) { return(*GetView()); }

	inline 								operator const T*(void) const { return( GetView()); }
	inline const T*						operator       ->(void) const { return( GetView()); }
	inline const T&						operator       * (void) const { return(*GetView()); }


private:
	inline T*							GetView(void) const { return(dynamic_cast<T*>(mView)); }
};





#endif // NIBVIEW_HDR


