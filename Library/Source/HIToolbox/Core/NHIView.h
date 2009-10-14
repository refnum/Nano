/*	NAME:
		NHIView.h

	DESCRIPTION:
		HIViewRef object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHIVIEW_HDR
#define NHIVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NHIObjectFactory.h"
#include "NPropertyStore.h"
#include "NHIWindow.h"
#include "NHIObject.h"
#include "NCGImage.h"
#include "NIcon.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Properties
static const OSType kPropertyNano									= 'Nano';


// Views
static const ControlButtonContentInfo kControlContentNone			= { kControlNoContent, 0 };

static const HIViewID		kHIViewIDNone							= { 0, 0 };
static const HILayoutInfo	kHILayoutNone							= { 0 };
static const HIViewFeatures kHIViewFeatureNone						= 0;

static const float kHIViewDisabledAlpha								= 0.5f;


// Options
static const OptionBits kHIViewOptionsNone							= 0;
static const OptionBits kHIViewOptionInvalidateOnActivate			= (1 << 0);
static const OptionBits kHIViewOptionInvalidateOnEnable				= (1 << 1);
static const OptionBits kHIViewOptionInvalidateOnHilite				= (1 << 2);
static const OptionBits kHIViewOptionInvalidateOnTitle				= (1 << 3);
static const OptionBits kHIViewOptionInvalidateOnValue				= (1 << 4);
static const OptionBits kHIViewOptionWebViewUpdateWindowTitle		= (1 << 5);
static const OptionBits kHIViewOptionWebViewUpdateWindowIcon		= (1 << 6);
static const OptionBits kHIViewOptionWebViewMakeLinksExternal		= (1 << 7);





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Register an HIView sub-class
//
// Registers an NHIView sub-class as a sub-class of, or a wrapper around, an
// existing HIView class.
//
// The base ID should be NULL if this class is a wrapper around some existing
// HIView class, or the identifier of the base class if this is a sub-class.
//
// Example usage:
//
//		REGISTER_HIVIEW_SUBCLASS(NFoo,       "com.nano.someview",  kHIViewClassID);
//		REGISTER_HIVIEW_SUBCLASS(NFooButton, kHIPushButtonClassID, NULL);
//
// The first case registers NFoo as a custom HIView that is a sub-class of the
// base HIView class; the corresponding view in Interface Builder would have
// its class ID set to "com.nano.someview".
//
// The second case registers NFooButton as a wrapper around kHIPushButtonClassID.
// This is not a true sub-class of kHIPushButtonClassID, but simply ensures that
// NHIObject:CreateObject will create an NFooButton if passed a push button.
#define REGISTER_HIVIEW_SUBCLASS(_class, _classID, _baseID)				\
	static NHIObject *__Factory ## _class(void)							\
	{																	\
		return(new _class);												\
	}																	\
																		\
	static OSStatus gFactoryStatus ## _class = NHIObjectFactory::AddFactory(__Factory ## _class, _classID, (CFStringRef) _baseID)



// Declare an HIView sub-class
//
// Declares a default constructor, and provides a static accessor to find an
// instance of a class in a window.
//
// Example usage:
//
//		class NFoo : public NHIView {
//		public:
//							DECLARE_HIVIEW_SUBCLASS(NFoo)
//
//							 NFoo(HIWindowRef hiWindow, const HIRect &theRect, ...);
//			virtual			~NFoo(void);
//			...
//		};
//
// To locate an instance of an NFoo within a window, use:
//
//		NFoo *theView = NFoo::Find(theWindow, theSignature, theID);
//
// A default constructor is provided to allow NFoos to be created through a
// factory callback, however additional constructors can be provided to allow
// the view to be created programmatically.
//
//
// Finding views is performed through a static accessor to work around an ld
// bug (rdar://4237972), which affects projects built as a static library.
//
// If a compilation unit is presented to the final link through a .o file,
// any static initializers in that unit will always be called (as you would
// expect).
//
// However if the same compilation unit is linked in via an intermediate .a
// library, static initializers in that compilation unit will only be called
// if there is at least 1 external reference to some other code in that unit.
//
// If there are no external references to that compilation unit, ld assumes
// that the initializer has no side-effects and can (incorrectly) be discarded.
//
// This is done independently of any dead-code stripping option exposed by
// the linker, and is a quasi-"dead-code stripping" phase that is performed
// automatically and can not currently be suppressed.
//
//
// To work around this, having code which wants to do "find the NFoo for this
// signature/id in this window" go through the static method ensures that ld
// never discards the initializer.
//
// If you have views which are self-contained and do not need to retreive a
// pointer to them, you can also use NFoo::Register to ensure that the view
// registration is not discarded by the linker.
#define DECLARE_HIVIEW_SUBCLASS(_class)															\
	public:																						\
							_class(void) { };													\
																								\
	static void				Register(void);														\
	static _class			*Find(HIWindowRef hiWindow, OSType theSignature, SInt32 theID=0)



// Define an HIView sub-class
//
// Implements the default constructor and static accessors for locating the view, and
// registers it as a sub-class or wrapper of the corresponding system class.
//
// Also provides a named constant for the class ID, kClassID.
//
// Example usage:
//
//		DEFINE_HIVIEW_SUBCLASS(NFoo,       "com.nano.someview",  kHIViewClassID);
//		DEFINE_HIVIEW_SUBCLASS(NFooButton, kHIPushButtonClassID, NULL);
//
// Note that REGISTER_HIVIEW_SUBCLASS is typically not called directly, as
// this macro will call it for you.
#define DEFINE_HIVIEW_SUBCLASS(_class, _classID, _baseID)								\
																						\
	REGISTER_HIVIEW_SUBCLASS(_class, _classID, _baseID);								\
																						\
	static const NString kClassID(_classID);											\
																						\
	void _class::Register(void)															\
	{																					\
		/* Silence any unused variable warnings */										\
		(void) gFactoryStatus ## _class;												\
		(void) kClassID;																\
	}																					\
																						\
	_class *_class::Find(HIWindowRef hiWindow, OSType theSignature, SInt32 theID)		\
	{	NHIWindow	theWindow(hiWindow);												\
																						\
		return(dynamic_cast<_class*>(theWindow.GetView(theSignature, theID)));			\
	}																					\
																						\
	void *kEatLastSemiColonForPedanticWarning ## _class



// Define an alternative HIView sub-class
//
// Registers an HIView sub-class under an alternative name.
//
// This can be used when a system view has been registered under different names
// on different OS releases, or the same class represents two system classes.
//
// Example usage:
//
//		DEFINE_HIVIEW_SUBCLASS(NFooButton,      kHIPushButtonClassID,     NULL);
//		DEFINE_HIVIEW_ALTCLASS(NFooButton_10_2, "com.apple.oldclassname", NULL, NFooButton);
//
// Since the alternative class is declared as a typedef of the real class, the
// register/find methods do not need to be re-implemented.
#define DEFINE_HIVIEW_ALTCLASS(_class, _classID, _baseID, _origclass)		\
																			\
	typedef _origclass _class;												\
																			\
	REGISTER_HIVIEW_SUBCLASS(_class, _classID, _baseID)



// Create an HIView sub-class
//
// HIView objects are created with HIObjectCreate, either as a sub-class of the
// HIView class or through a view-specific API such as CreatePushButtonControl.
//
// This inline function can be used to construct an HIView sub-class that should
// be associated with a C++ object.
//
// Typically it would be used in the constructor of an NHIView sub-class, to
// provide an HIViewRef for ConstructView.
inline HIViewRef HIViewCreate(const NString &classID, NHIObject *theObject)
{	HIViewRef		theView;
	OSStatus		theErr;



	// Create the view	
	NHIObjectFactory::SetNextInstance(theObject);
	
	theErr  = HIObjectCreate(classID, NULL, (HIObjectRef *) &theView);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		theView = NULL;
		
	return(theView);
}





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHIView : public NHIObject, public NPropertyStore {
public:
										NHIView(void);
	virtual								~NHIView(void);


	// Get the bounds
	HIRect								GetBounds(void) const;


	// Get/set the frame
	HIRect								GetFrame(void) const;
	void								SetFrame(const HIRect &theRect);


	// Get the ideal size
	HIRect								GetIdealSize(void) const;


	// Get/set the size constraints
	//
	// Size constraints can only be set for custom views.
	void								GetSizeConstraints(      HISize *minSize,       HISize *maxSize) const;
	void								SetSizeConstraints(const HISize *minSize, const HISize *maxSize);


	// Get/set the visibility
	bool								IsVisible(void) const;
	void								SetVisible(bool isVisible=true);


	// Get/set the enabled state
	bool								IsEnabled(void) const;
	void								SetEnabled(bool isEnabled);


	// Get/set the active state
	bool								IsActive(void) const;
	void								SetActive(bool isActive);


	// Get/set the hilite state
	HIViewPartCode						GetHilite(void) const;
	void								SetHilite(HIViewPartCode thePart);
	

	// Get/set the view ID
	HIViewID							GetViewID(void) const;
	void								SetViewID(const HIViewID &theID);


	// Get/set the command ID
	UInt32								GetCommandID(void) const;
	void								SetCommandID(UInt32 theCmd);


	// Send the command
	OSStatus							SendCommand(void) const;
	

	// Get/set the value
	//
	// The current value can also be accessed as a percentage, from 0.0 to 1.0.
	SInt32								GetMinimumValue(void) const;
	SInt32								GetMaximumValue(void) const;
	SInt32								GetCurrentValue(void) const;
	float								GetPercentValue(void) const;

	void								SetMinimumValue(SInt32 theValue);
	void								SetMaximumValue(SInt32 theValue);
	void								SetCurrentValue(SInt32 theValue);
	void								SetPercentValue(float  theValue);


	// Get/set the text value
	virtual NString						GetTextValue(void) const;
	virtual void						SetTextValue(const NString &theValue);


	// Get/set the control size
	ControlSize							GetControlSize(void) const;
	void								SetControlSize(ControlSize theSize);


	// Get/set the layout info
	HILayoutInfo						GetLayoutInfo(void) const;
	void								SetLayoutInfo(const HILayoutInfo &theInfo);


	// Get/set the features
	//
	// Features are typically determined by a view, and read by users of a view.
	HIViewFeatures						GetFeatures(void) const;
	void								SetFeatures(HIViewFeatures setThese, HIViewFeatures clearThese=kHIViewFeatureNone);


	// Get/set the attributes
	//
	// Attributes are typically modified by users of a view, and acted on by a view.
	OptionBits							GetAttributes(void) const;
	void								SetAttributes(OptionBits setThese, OptionBits clearThese=0);


	// Get/set the options
	//
	// Options are used to hold Nano-specific attributes/features.
	OptionBits							GetOptions(void) const;
	void								SetOptions(OptionBits setThese, OptionBits clearThese=0);


	// Get/set the help tag
	NString								GetHelpTag(                         bool isExtended=false) const;
	void								SetHelpTag(const NString &theValue, bool isExtended=false);


	// Get the window
	HIWindowRef							GetHIWindow(void) const;


	// Get the super-view
	NHIView								*GetSuperView(void) const;


	// Get the number of sub-views
	UInt32								GetSubViewCount(void) const;


	// Get a sub-view
	NHIView								*GetSubView(UInt32 theIndex) const;


	// Add a sub-view
	void								AddSubView(NHIView *theView);


	// Remove a sub-view
	void								RemoveSubView(NHIView *theView, bool andDelete=true);


	// Remove this view from its super-view
	//
	// The view will not be disposed of after removal.
	void								RemoveFromSuperView(void);


	// Remove all sub-views from this view
	//
	// The views will be disposed of after removal.
	void								RemoveSubViews(void);


	// Set the display state
	void								SetNeedsDisplay(bool needsDisplay=true, const HIRect *theRect=NULL);


	// Get/set the font state
	ControlFontStyleRec					GetFontStyle(void) const;
	void								SetFontStyle(const ControlFontStyleRec &theStyle);


	// Set the Z-order
	void								SetZOrder(HIViewZOrderOp zOrder, NHIView *relativeTo=NULL);


	// Render the view
	//
	// Causes an immediate render of the view, optionally blocking for a short delay (which
	// should be kept as brief as possible - typically a few hundred ms if simulating clicks).
	void								Render(EventTime waitFor=kEventDurationNoWait);


	// Simulate a click
	HIViewPartCode						SimulateClick(HIViewPartCode	partCode = kHIViewClickableMetaPart,
														UInt32			modKeys  = GetCurrentEventKeyModifiers());


	// Does a property exist?
	bool								HasProperty(OSType theCreator, OSType theTag) const;
	

	// Get/set a property
	//
	// Typically used to store custom state (e.g., with IB). A zero'd result is returned on error.
	bool								GetPropertyBoolean(OSType theCreator, OSType theTag) const;
	UInt16								GetPropertyUInt16( OSType theCreator, OSType theTag) const;
	UInt32								GetPropertyUInt32( OSType theCreator, OSType theTag) const;
	UInt64								GetPropertyUInt64( OSType theCreator, OSType theTag) const;
	Float32								GetPropertyFloat32(OSType theCreator, OSType theTag) const;
	Float64								GetPropertyFloat64(OSType theCreator, OSType theTag) const;
	NString								GetPropertyString( OSType theCreator, OSType theTag) const;
	OSType								GetPropertyOSType( OSType theCreator, OSType theTag) const;
	void							   *GetPropertyPointer(OSType theCreator, OSType theTag) const;

	void								SetPropertyBoolean(OSType theCreator, OSType theTag, bool           theValue);
	void								SetPropertyUInt16( OSType theCreator, OSType theTag, UInt16         theValue);
	void								SetPropertyUInt32( OSType theCreator, OSType theTag, UInt32         theValue);
	void								SetPropertyUInt64( OSType theCreator, OSType theTag, UInt64         theValue);
	void								SetPropertyFloat32(OSType theCreator, OSType theTag, Float32        theValue);
	void								SetPropertyFloat64(OSType theCreator, OSType theTag, Float64        theValue);
	void								SetPropertyString( OSType theCreator, OSType theTag, const NString &theValue);
	void								SetPropertyOSType( OSType theCreator, OSType theTag, OSType         theValue);
	void								SetPropertyPointer(OSType theCreator, OSType theTag, void          *theValue);


	// Get/set a control value
	//
	// Typically used to store control state. A zero'd result is returned on error.
	void								GetControlData(   ControlPartCode thePart, OSType theTag, UInt32 theSize, void *theData) const;
	bool								GetControlBoolean(ControlPartCode thePart, OSType theTag) const;
	UInt16								GetControlUInt16( ControlPartCode thePart, OSType theTag) const;
	UInt32								GetControlUInt32( ControlPartCode thePart, OSType theTag) const;
	UInt64								GetControlUInt64( ControlPartCode thePart, OSType theTag) const;
	Float32								GetControlFloat32(ControlPartCode thePart, OSType theTag) const;
	Float64								GetControlFloat64(ControlPartCode thePart, OSType theTag) const;
	NString								GetControlString( ControlPartCode thePart, OSType theTag) const;

	void								SetControlData(   ControlPartCode thePart, OSType theTag, UInt32 theSize, const void *theData);
	void								SetControlBoolean(ControlPartCode thePart, OSType theTag, bool           theValue);
	void								SetControlUInt16( ControlPartCode thePart, OSType theTag, UInt16         theValue);
	void								SetControlUInt32( ControlPartCode thePart, OSType theTag, UInt32         theValue);
	void								SetControlUInt64( ControlPartCode thePart, OSType theTag, UInt64         theValue);
	void								SetControlFloat32(ControlPartCode thePart, OSType theTag, Float32        theValue);
	void								SetControlFloat64(ControlPartCode thePart, OSType theTag, Float64        theValue);
	void								SetControlString( ControlPartCode thePart, OSType theTag, const NString &theValue);


	// Get/set the content
	NIcon								GetContentIcon( ControlPartCode thePart, OSType theTag) const;
	NCGImage							GetContentImage(ControlPartCode thePart, OSType theTag) const;

	void								SetContentIcon( ControlPartCode thePart, OSType theTag, const NIcon    &theIcon);
	void								SetContentImage(ControlPartCode thePart, OSType theTag, const NCGImage &theImage);


	// Get the view image
	NCGImage							GetViewImage(void) const;


	// Get/set the dragged types
	//
	// Views can receive dragged data by assigning an array of UTIs.
	NArray								GetDraggedTypes(void) const;
	void								SetDraggedTypes(const NArray &theTypes);


	// Operators
	operator							HIViewRef(void) const;


protected:
	// Initialize the view
	//
	// Sub-classes should override to initialize the view state. At this point any
	// properties applied to the view in Interface Builder are available, and can
	// be used to customize the appearance or behaviour of the view.
	virtual void						InitializeView(void);


	// Handle events
	virtual OSStatus					DoEventHIObjectInitialize(        NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlActivate(           NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDeactivate(         NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDragEnter(          NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDragLeave(          NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDragReceive(        NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDragWithin(         NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlEnabledStateChanged(NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetSizeConstraints( NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlHiliteChanged(      NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlHitTest(            NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlTitleChanged(       NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlTrack(              NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlValueFieldChanged(  NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNControlInitialized(       NCarbonEvent &theEvent);


	// Handle drags
	//
	// Views that wish to receive drags should use SetDraggedTypes to assign the UTIs
	// they can receive, then implement these methods to process and receive drags.
	virtual ThemeCursor					DoDragUpdate( DragRef theDrag, bool isWithin);
	virtual DragActions					DoDragReceive(DragRef theDrag);


	// Handle mouse tracking
	//
	// Views that wish to track the mouse should register for kEventControlHitTest
	// and kEventControlTrack, then implement this method to perform their mouse
	// tracking.
	//
	// The base class provides a default implementation that invokes this method
	// from a standard mouse tracking loop.
	//
	// Sub-classes should call through to inherit standard behaviour, such as
	// ending the track loop when the mouse is released or the user cancels.
	virtual bool						DoTrackMouse(NCarbonEvent				&theEvent,
														const HIPoint			&theMouse,
														MouseTrackingResult		theTrack);


	// Construct the view
	//
	// Sub-classes which support procedural creation (i.e., provide a constructor with
	// some configuration parameters, to allow the view to be instantiated at run-time)
	// must call use this method to finish constructing the view.
	//
	// This method can add the view to a window, set its bounds and visiblity, assign
	// the HIViewRef to this object, and invoke InitializeView.
	//
	// All parameters are optional, except for the view.
	void								ConstructView(HIViewRef			hiView,
														HIWindowRef		hiWindow  = NULL,
														const HIRect	*theFrame = NULL,
														bool			isVisible = true);


	// Set the HIObject
	void								SetHIObject(HIObjectRef hiObject);


	// Get/set the drag target state
	bool								IsDragTarget(void) const;
	void								SetDragTarget(bool isTarget);


	// Get/set the mouse tracking state
	bool								IsTrackingMouse(void) const;
	void								SetTrackingMouse(bool isTracking);


private:
	void								InitializeSelf(   void);
	void								InitializeSubView(void);



private:
	HISize								mSizeMin;
	HISize								mSizeMax;
	OptionBits							mOptions;

	bool								mIsDragTarget;
	bool								mIsTrackingMouse;
	
	NArray								mDraggedTypes;
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
inline bool operator ==(const HIViewID &value1, const HIViewID &value2)
{
	return(value1.signature == value2.signature && value1.id == value2.id);
}

inline bool operator !=(const HIViewID &value1, const HIViewID &value2)
{
	return(value1.signature != value2.signature || value1.id != value2.id);
}




#endif // NHIVIEW_HDR


