/*	NAME:
		NanoNotes.h

	DESCRIPTION:
		Nano release notes.
		
		This file contains information about important changes between
		releases of Nano, particularly for changes that will not produce
		a compiler or linker error.
		
		To turn off these messages, your prefix header should include a
		definition of NN_SEEN_RELEASE_NOTES_FOR that matches the current
		version of Nano.
		
		For example:
		
		    #define NN_SEEN_RELEASE_NOTES_FOR			0x00200000
	
		By defining this prior to including Nano.h, the warnings related to
		this (or older) versions of Nano can be suppressed while warnings
		for future versions of Nano will still be seen.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_NOTES_HDR
#define NANO_NOTES_HDR
//============================================================================
//		Build Constants
//----------------------------------------------------------------------------
#define NN_RELEASE_NOTES_2_0									0x00200000
#define NN_RELEASE_NOTES_1_5									0x00100500
#define NN_RELEASE_NOTES_1_4									0x00100400
#define NN_RELEASE_NOTES_1_3									0x00100300
#define NN_RELEASE_NOTES_1_2									0x00100200
#define NN_RELEASE_NOTES_1_1									0x00100100

#define NN_RELEASE_NOTES_SHOW_NONE								0xFFFFFFFF
#define NN_RELEASE_NOTES_SHOW_ALL								0x00000000

#ifndef NN_SEEN_RELEASE_NOTES_FOR
#define NN_SEEN_RELEASE_NOTES_FOR								NN_RELEASE_NOTES_SHOW_ALL
#endif





//============================================================================
//		Release Notes
//----------------------------------------------------------------------------
#if NN_SEEN_RELEASE_NOTES_FOR < NN_RELEASE_NOTES_2_0

	// Misc
	#pragma message("2.0: Review all source - not backwards compatible with 1.x")

#endif



#if NN_SEEN_RELEASE_NOTES_FOR < NN_RELEASE_NOTES_1_5

	// NIcon
	//
	// See NIcon.h
	#pragma message("1.5: NIcon(OSType, OSType) has swapped type/creator order")


	// NXMLTree
	//
	// See NXMLTree.h
	#pragma message("1.5: NXMLTree::GetElementContents renamed to NXMLTree::GetElementValue")
	
	
	// NPropertyStore
	//
	// See NPropertyStore.h
	#pragma message("1.5: NPropertyStore coder interface replaced with storage model")


	// NDocument
	//
	// See NDocument.h
	#pragma message( "1.5: NDocument::EncodeProperties/DecodeProperties renamed to EncodeState/DecodeState")


	// NDataBrowser
	//
	// See NDataBrowser.h
	#pragma message( "1.5: NDataBrowser::EncodeProperties/DecodeProperties renamed to EncodeState/DecodeState")


	// Array accessors
	//
	// See NArray.h/NDictionary.h
	#pragma message( "1.5: NArray/NDictionary removed [] accessors")


	// NPropertyFile
	//
	// See NPropertyFile.h
	#pragma message( "1.5: NPropertyFile::GetProperties no longer returns a reference")

#endif



#if NN_SEEN_RELEASE_NOTES_FOR < NN_RELEASE_NOTES_1_4

	// NColorPicker
	//
	// See NColorPicker.h/NColorPanel.h
	#pragma message( "1.4: NColorPicker messages now defined by NColorPanel")


	// Strings
	//
	// See Nano Build.h
	#pragma message( "1.4: kStringToolbarShow/Hide renamed to kStringShow/HideToolbar")


	// Help tags
	//
	// See NHelpTag/NHIView/NMenuItem/NHelpUtilities
	#pragma message( "1.4: NHIView/NMenuItem help tags now accessed through NHelpTag")

#endif



#if NN_SEEN_RELEASE_NOTES_FOR < NN_RELEASE_NOTES_1_3

	// NString
	//
	// See NString.h
	#pragma message( "1.3: NString::EqualTo is now case-INsensitive by default")
	#pragma message( "1.3: NString::IsMatch        replaced with NString::Contains(kCFComparePattern)")
	#pragma message( "1.3: NString::FindPattern    replaced with NString::FindAll( kCFComparePattern)")
	#pragma message( "1.3: NString::TrimWhitespace replaced with NString::Trim()")


	// NUTI
	//
	// See NUTI.h
	#pragma message( "1.3: NUTI::EqualTo replaced with operator ==")


	// NCFObject
	//
	// See NCFObject.h
	#pragma message( "1.3: NCFObject now provides copy-on-write; review any custom sub-classes of NCFObject")


	// NTextUtilities
	//
	// See NTextUtilities.h
	#pragma message( "1.3: NTextUtilities Get/FromUInt32 replaced with SInt64")


	// NSystemUtilities
	//
	// See NSTLUtilities.h
	#pragma message( "1.3: CREATE_VECTOR and STL helpers moved from NSystemUtilities to NSTLUtilities")


	// NTimerTask
	//
	// See NTimer.h
	#pragma message( "1.3: NTimerTask replaced wth functor-based NTimer")


	// NAutoReleasePool
	//
	// See NAppKitUtilities.h
	#pragma message( "1.3: NAutoReleasePool replaced wth StAutoReleasePool")

#endif



#if NN_SEEN_RELEASE_NOTES_FOR < NN_RELEASE_NOTES_1_2

	// PrefPanelInfo
	//
	// See NWindowPanels.h
	#pragma message( "1.2: PrefPanelInfo renamed to PanelInfo to support NWindowPanels; rename usage to match")


	// NCFDate
	//
	// See NDate.h
	#pragma message( "1.2: NCFDate was replaced with NDate")
	
	
	// NEditText
	//
	// See NEditText.h
	#pragma message( "1.2: NEditText::[GS]etSelection now uses CFRange rather than ControlEditTextSelectionRec")


	// NSystemUtilities
	//
	// See NSystemUtilities.h
	#pragma message( "1.2: NSystemUtilities::ShowEffect moved to NThemeUtilities")


	// NString
	//
	// See NString.h
	#pragma message( "1.2: NString::Find/FindAll now treats range length of -1 as end-of-string (was 0)")


	// Projects
	#pragma message( "1.2: Add Security.framework, SystemConfiguration.framework, WebKit.framework, and Nano.nib to your project - required for NSoftwareUpdate")


	// Properties
	//
	// See NApplication::InitializeProperties.
	#pragma message( "1.2: NApplication::InitializeProperties now adds kAppDefaultPrefsKey, update rather than replacing this dictionary in your InitializeProperties")

#endif



#if NN_SEEN_RELEASE_NOTES_FOR < NN_RELEASE_NOTES_1_1

	// kHICommandNToggleToolbar
	//
	// See NCarbonEventHandler.h.
	#pragma message( "1.1: kHICommandNToggleToolbar changed from 'Ntog' to 'tbtg' to match 10.5; update .nib usage")

#endif






#endif // NANO_NOTES_HDR


