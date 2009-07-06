/*	NAME:
		TB64Encoder.cpp

	DESCRIPTION:
		NB64Encoder tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NB64Encoder.h"
#include "NChecksum.h"

#include "TB64Encoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt8 kTestDataBinary[]					= { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
															0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x40, 0x08, 0x03, 0x00, 0x00, 0x00, 0x75, 0x37, 0x34,
															0xC7, 0x00, 0x00, 0x00, 0x54, 0x50, 0x4C, 0x54, 0x45, 0x94, 0x8C, 0xFF, 0x4A, 0x29, 0xFF, 0x6B,
															0x63, 0xFF, 0x6B, 0x5A, 0xFF, 0x5A, 0x4A, 0xFF, 0x52, 0x39, 0xFF, 0x4A, 0x39, 0xFF, 0x7B, 0x6B,
															0xFF, 0x39, 0x18, 0xFF, 0x39, 0x08, 0xFF, 0x29, 0x00, 0xFF, 0x7B, 0x73, 0xFF, 0x63, 0x4A, 0xFF,
															0x42, 0x18, 0xFF, 0x18, 0x00, 0xFF, 0x8C, 0x7B, 0xFF, 0x18, 0x00, 0xEF, 0x10, 0x00, 0xCE, 0x18,
															0x00, 0xDE, 0x08, 0x00, 0xA5, 0x8C, 0x84, 0xFF, 0x08, 0x00, 0x9C, 0x00, 0x00, 0x00, 0x10, 0x00,
															0xAD, 0x08, 0x00, 0x6B, 0x18, 0x00, 0xB5, 0x08, 0x00, 0x8C, 0x18, 0x00, 0x8C, 0xAF, 0xF2, 0x70,
															0x41, 0x00, 0x00, 0x07, 0x2B, 0x49, 0x44, 0x41, 0x54, 0x78, 0x5E, 0x55, 0x96, 0xE7, 0x82, 0xEB,
															0xB8, 0x0E, 0x83, 0x45, 0xD5, 0x62, 0x3B, 0x6D, 0x4E, 0xDB, 0xBD, 0xEF, 0xFF, 0x9E, 0xF7, 0x03,
															0x3C, 0xE7, 0xC7, 0x32, 0xE3, 0xD8, 0x52, 0x04, 0x56, 0x90, 0xE3, 0xB4, 0x1F, 0xAF, 0xB9, 0xF7,
															0x03, 0x79, 0xBD, 0x7F, 0x7F, 0x7E, 0xBE, 0xBF, 0xDE, 0xEF, 0xAF, 0x7F, 0xFE, 0xFD, 0x9F, 0x76,
															0xF6, 0x3F, 0x5F, 0x9F, 0x7F, 0x7F, 0x7D, 0xF1, 0x30, 0x1F, 0xEB, 0xF1, 0x78, 0x7F, 0x1E, 0xEF,
															0xDF, 0x5F, 0xEF, 0xC7, 0xEF, 0xF7, 0x1F, 0xFD, 0xB8, 0x7E, 0x7D, 0xFD, 0x7C, 0xA7, 0xBD, 0x63,
															0xAF, 0xB5, 0xD1, 0xF2, 0x00, 0x22, 0xF9, 0xF9, 0xF5, 0x36, 0x78, 0xCD, 0x17, 0x9B, 0x9F, 0x98,
															0x8F, 0xC7, 0xE4, 0xF3, 0x7A, 0xA3, 0xE3, 0xA5, 0x53, 0xFB, 0xFD, 0x5A, 0xEF, 0x3F, 0xF1, 0xEB,
															0xFD, 0x79, 0xA5, 0x98, 0x1D, 0x24, 0xF2, 0x11, 0xFC, 0xF5, 0x79, 0x7C, 0x3E, 0xAF, 0x17, 0x47,
															0x22, 0x7A, 0xCC, 0xE8, 0xAD, 0xF5, 0xDE, 0xDB, 0xDE, 0x73, 0x2E, 0xCE, 0xBC, 0xDF, 0xEF, 0xBD,
															0xBF, 0x5E, 0x5F, 0x00, 0xDE, 0x73, 0x73, 0x36, 0x45, 0x54, 0x8E, 0xCD, 0x35, 0xD7, 0xE2, 0xDA,
															0xD1, 0x91, 0x56, 0x1B, 0x52, 0xFF, 0x0A, 0x2A, 0xF8, 0x34, 0x1D, 0x93, 0x2C, 0x64, 0xCE, 0x1D,
															0x28, 0xDD, 0x69, 0xA2, 0x1B, 0x59, 0xFA, 0x5B, 0x32, 0xD7, 0x03, 0xBC, 0x10, 0xE0, 0xA4, 0xE8,
															0xBF, 0x9A, 0x22, 0xB6, 0x04, 0x0D, 0x51, 0xF1, 0x36, 0xAD, 0xEE, 0x0D, 0xD4, 0xB1, 0x73, 0xCB,
															0x5F, 0xBC, 0x34, 0xE1, 0xFB, 0xED, 0x90, 0xB7, 0x24, 0x56, 0xB0, 0x40, 0xD4, 0x29, 0xEB, 0x25,
															0x16, 0x2B, 0x76, 0xA3, 0x59, 0x22, 0x6C, 0x06, 0xF1, 0x97, 0xFD, 0xF0, 0xE2, 0xDE, 0xD6, 0xB1,
															0xB9, 0x30, 0x16, 0x25, 0x0C, 0x6F, 0x81, 0xC8, 0x50, 0x2D, 0x08, 0xFE, 0x55, 0x89, 0xE3, 0xE5,
															0xDE, 0x41, 0xF6, 0xCE, 0x15, 0xD7, 0xBC, 0x7A, 0xAD, 0x5E, 0xC4, 0x75, 0x01, 0x6F, 0x6B, 0xA7,
															0x55, 0x64, 0x78, 0x6A, 0x33, 0x1B, 0x6E, 0xAB, 0xBD, 0x83, 0x8F, 0x19, 0x76, 0x9B, 0x4C, 0x45,
															0xAC, 0xA5, 0xE8, 0xD0, 0xE5, 0x03, 0x20, 0xA2, 0xD4, 0x35, 0x53, 0x0C, 0xF4, 0xD5, 0xB3, 0xD4,
															0x23, 0x97, 0x6C, 0xEF, 0x10, 0xB9, 0xCC, 0xB3, 0x8E, 0x07, 0x76, 0x75, 0x63, 0xC3, 0x81, 0xE1,
															0x63, 0x3D, 0xED, 0x5C, 0xC9, 0x01, 0x3C, 0xD7, 0xD9, 0x71, 0xFB, 0xC0, 0x70, 0x35, 0xF4, 0xBE,
															0x26, 0x90, 0x49, 0x5E, 0x91, 0x3D, 0x83, 0x50, 0xFF, 0x5B, 0x4D, 0x62, 0x2A, 0xA3, 0xCF, 0xD4,
															0x47, 0x8D, 0x52, 0xC6, 0xB0, 0xE3, 0x7F, 0xAB, 0x66, 0x54, 0x29, 0x67, 0x65, 0xAF, 0xDF, 0x3B,
															0xD7, 0xB4, 0x69, 0x4E, 0xF9, 0x0A, 0x56, 0x25, 0x22, 0xB5, 0xDC, 0x42, 0x68, 0x6F, 0xF5, 0x79,
															0x35, 0x4B, 0xB4, 0x35, 0xAB, 0xB4, 0x0E, 0x7E, 0xEE, 0x55, 0xD5, 0xBB, 0xB0, 0x5C, 0xCE, 0x5E,
															0xBA, 0xF1, 0x0D, 0x97, 0x73, 0x8D, 0x54, 0x47, 0x6F, 0x79, 0x1C, 0xC0, 0x63, 0x46, 0x2B, 0x2E,
															0xD9, 0x2D, 0x75, 0x1C, 0xC8, 0xA8, 0x55, 0x69, 0x74, 0xB4, 0x32, 0xC3, 0xD2, 0x70, 0x6C, 0x8D,
															0x72, 0xA5, 0x86, 0x0B, 0x59, 0xD6, 0xBB, 0xDC, 0xAD, 0x86, 0x3B, 0xF8, 0xA8, 0x60, 0x51, 0x0C,
															0x2E, 0xAE, 0xAE, 0x54, 0x2B, 0x70, 0xF0, 0xAE, 0xB0, 0x0E, 0x1D, 0x2D, 0x52, 0x14, 0x34, 0x67,
															0x72, 0x3E, 0x31, 0x21, 0xB4, 0x99, 0xB9, 0xA0, 0x77, 0x7D, 0xA2, 0x34, 0xE7, 0x62, 0xA6, 0xB5,
															0x45, 0x08, 0xC4, 0x33, 0x4A, 0xF4, 0xB0, 0x91, 0x6E, 0xB8, 0xE9, 0x34, 0xF2, 0x8D, 0x2C, 0x76,
															0xE2, 0xE2, 0xF4, 0xC4, 0x97, 0x23, 0xD7, 0x92, 0x0B, 0xE6, 0x95, 0x95, 0x73, 0x0C, 0x30, 0xC0,
															0x6B, 0xD1, 0x1D, 0x74, 0xE4, 0xB3, 0xA7, 0x7E, 0x12, 0x4C, 0x2E, 0xD5, 0x62, 0x07, 0xE4, 0x27,
															0xE8, 0x56, 0x0E, 0x9D, 0x45, 0xC1, 0xB7, 0xE6, 0x5C, 0x1D, 0x7A, 0x71, 0xF8, 0xEA, 0x85, 0xD2,
															0xAE, 0x04, 0x26, 0x7A, 0xE6, 0x8B, 0xCA, 0xB2, 0x26, 0x84, 0x92, 0xD1, 0x5C, 0xA3, 0x8D, 0x5C,
															0x7C, 0xDA, 0x81, 0x4A, 0x21, 0xA0, 0xE0, 0x5C, 0xC3, 0xBC, 0x79, 0x44, 0xFA, 0x0D, 0x8F, 0xAC,
															0x35, 0x96, 0x4F, 0x65, 0xCD, 0xE6, 0x66, 0xDC, 0x14, 0x56, 0xBF, 0x8C, 0x22, 0xFE, 0x02, 0x70,
															0x18, 0xC4, 0x9F, 0xF5, 0x40, 0x04, 0x86, 0xD3, 0x00, 0x22, 0x8C, 0x69, 0x65, 0x83, 0x7D, 0x21,
															0xD8, 0xAC, 0x3C, 0x47, 0x6F, 0x17, 0x88, 0xB3, 0x60, 0xFC, 0x5A, 0xD0, 0x62, 0x40, 0x6E, 0x89,
															0x7B, 0x70, 0x25, 0x15, 0xDF, 0x7D, 0x60, 0xCE, 0xAA, 0x59, 0x6E, 0x47, 0x49, 0x4E, 0xB5, 0x7D,
															0xE1, 0x8A, 0x19, 0x30, 0xC6, 0xF1, 0x54, 0x3E, 0x90, 0x53, 0x71, 0xF4, 0xB6, 0x52, 0x3B, 0x1A,
															0x16, 0x6C, 0xAA, 0xAB, 0x91, 0x8D, 0x1E, 0xF2, 0x2F, 0x80, 0x57, 0x1C, 0xA4, 0x0C, 0xFC, 0x9A,
															0xD9, 0xCB, 0x19, 0xB0, 0xCC, 0x22, 0x1E, 0x25, 0x91, 0xE6, 0x51, 0x67, 0x0D, 0x4A, 0x24, 0x6A,
															0x2B, 0xC7, 0xA2, 0x0B, 0xAA, 0x82, 0x4E, 0xC2, 0x45, 0x59, 0xC7, 0xCE, 0x59, 0x2C, 0xCA, 0xAD,
															0x39, 0xC5, 0x59, 0xAE, 0x70, 0xC3, 0x06, 0x38, 0xD2, 0x5C, 0x33, 0x2C, 0x7D, 0xFE, 0x80, 0xA6,
															0xED, 0xBB, 0x47, 0x8B, 0x08, 0x2E, 0x0A, 0xA1, 0x40, 0xB4, 0x57, 0x4D, 0x67, 0x28, 0xD7, 0x57,
															0xE8, 0x48, 0x5F, 0xC0, 0x4F, 0x3D, 0x10, 0x0D, 0x0E, 0x8A, 0x3D, 0xC2, 0xC5, 0x9E, 0xA0, 0x59,
															0xF0, 0x05, 0xD6, 0xED, 0x17, 0x6B, 0x83, 0x70, 0xF5, 0x6E, 0xEC, 0x64, 0x3B, 0xAD, 0x9C, 0xE7,
															0xAA, 0x4E, 0x19, 0x08, 0x8B, 0xF4, 0x5B, 0x09, 0xBA, 0x04, 0xEF, 0x72, 0xCF, 0x45, 0xEC, 0x9E,
															0x34, 0xDA, 0x71, 0x04, 0x1D, 0xE7, 0xCB, 0x68, 0x38, 0x24, 0xC4, 0x9D, 0xF8, 0x66, 0xEE, 0x21,
															0xE1, 0x64, 0x9C, 0xA1, 0x3C, 0x14, 0x8B, 0x55, 0x48, 0x1B, 0x7F, 0xDC, 0x4B, 0xC7, 0x7A, 0x39,
															0xFA, 0x06, 0x5E, 0x5D, 0x2E, 0xA5, 0x14, 0xF0, 0x7D, 0xB4, 0x79, 0xA6, 0xC8, 0xAD, 0x02, 0x55,
															0xB2, 0x36, 0xE4, 0xA6, 0x73, 0x29, 0x0D, 0xB9, 0xF7, 0xB4, 0xEA, 0x93, 0xEC, 0x28, 0x66, 0x4D,
															0xC4, 0xD6, 0x8C, 0xCD, 0x22, 0xC7, 0xA9, 0x7E, 0x19, 0xAC, 0xCE, 0x76, 0x02, 0xD7, 0x93, 0x53,
															0x03, 0x16, 0x41, 0xC7, 0x68, 0x0D, 0xF8, 0x81, 0x61, 0x69, 0xF5, 0x96, 0xC0, 0xD0, 0x03, 0x18,
															0xD7, 0xF3, 0x96, 0xE3, 0x59, 0xDC, 0xFB, 0x34, 0x8D, 0x44, 0x31, 0x09, 0x5E, 0x07, 0xD6, 0xC5,
															0x71, 0xE0, 0x55, 0xD8, 0xCB, 0xF0, 0x5C, 0xC4, 0x2F, 0x61, 0x2A, 0x8F, 0x42, 0x49, 0xCF, 0x3D,
															0x0F, 0x1D, 0x80, 0xA3, 0xC2, 0xB8, 0x66, 0xDD, 0x6E, 0x19, 0xE2, 0xB0, 0xE1, 0x4C, 0x49, 0xB2,
															0x20, 0x69, 0x88, 0x3B, 0x0E, 0x52, 0x13, 0x33, 0x57, 0xB7, 0x8A, 0x9B, 0xD0, 0xA2, 0x61, 0xD4,
															0x26, 0x99, 0xEF, 0xDC, 0x59, 0x38, 0xDB, 0x05, 0x11, 0x98, 0xDE, 0xA9, 0x1E, 0x32, 0xE6, 0xB3,
															0x28, 0x60, 0x81, 0x84, 0x96, 0xB3, 0x50, 0x9F, 0x7C, 0x30, 0x3D, 0xD3, 0xC6, 0x8B, 0x7A, 0x2D,
															0xA1, 0x8B, 0xE1, 0x25, 0x9B, 0xD7, 0x2E, 0x10, 0x97, 0xA1, 0x0D, 0x5C, 0x5D, 0x1A, 0xAE, 0x50,
															0xCB, 0xED, 0xA6, 0xA9, 0x77, 0xED, 0x9D, 0xF6, 0xC2, 0x7B, 0x58, 0xD8, 0xBB, 0x35, 0x48, 0x3A,
															0x5C, 0xE0, 0x04, 0x3B, 0x2D, 0x56, 0x9F, 0xEB, 0xE6, 0x43, 0xB9, 0xD9, 0x23, 0x11, 0x33, 0xA0,
															0xCB, 0x7E, 0xEC, 0xC4, 0x8F, 0xE5, 0xD9, 0x63, 0xDE, 0x3F, 0x7D, 0xB7, 0x38, 0x4F, 0x76, 0x9D,
															0xFD, 0x7A, 0x5D, 0x7A, 0xD2, 0x28, 0x12, 0x33, 0xB6, 0x9C, 0x14, 0x91, 0xF2, 0xE8, 0xFB, 0xB1,
															0xD2, 0xDE, 0x2B, 0x68, 0xC3, 0x8B, 0x64, 0x7A, 0x84, 0x79, 0x2E, 0x51, 0x8C, 0x58, 0xFC, 0xE9,
															0xEB, 0xBA, 0xC2, 0xE2, 0x19, 0x68, 0x3D, 0x4E, 0xC3, 0x01, 0xD7, 0x37, 0x99, 0x47, 0xCA, 0x51,
															0x4C, 0xC4, 0xEE, 0xC6, 0xF2, 0x3C, 0x0A, 0xF9, 0x3C, 0xE7, 0x12, 0x42, 0x44, 0x23, 0xFF, 0xA5,
															0x9A, 0xE8, 0xE1, 0x39, 0xCD, 0x00, 0xC6, 0xF7, 0x9A, 0x02, 0xFD, 0x78, 0x42, 0x71, 0x3A, 0x11,
															0xFA, 0x23, 0xDF, 0x5B, 0x48, 0x1A, 0xA2, 0x60, 0xEA, 0x34, 0x05, 0xA8, 0x81, 0x3E, 0xB6, 0x3E,
															0x80, 0x47, 0x4B, 0x7A, 0x9F, 0x5A, 0x0D, 0x56, 0x54, 0xCF, 0x3B, 0x57, 0x89, 0x05, 0x4A, 0x63,
															0x22, 0x11, 0x6E, 0xF4, 0x6E, 0x48, 0x6B, 0xD3, 0xBF, 0x73, 0x60, 0x3C, 0x2B, 0x6E, 0x47, 0x02,
															0x0D, 0x7E, 0xD8, 0xE0, 0x44, 0x36, 0xCE, 0x88, 0x30, 0xD3, 0x33, 0xC1, 0xCE, 0xAA, 0x8C, 0x05,
															0x05, 0x52, 0xDA, 0xA8, 0x54, 0xE8, 0x79, 0xB4, 0xC7, 0x43, 0x85, 0xDB, 0x2F, 0xEE, 0xF5, 0x99,
															0x55, 0xB0, 0x20, 0xD8, 0xEB, 0xB2, 0xA9, 0xEF, 0xB6, 0xB8, 0xCD, 0x8A, 0xB7, 0x83, 0x6A, 0xD6,
															0x39, 0x55, 0x23, 0x57, 0x7D, 0x1A, 0x4E, 0xFE, 0xB8, 0xCD, 0x72, 0x88, 0x6B, 0x5B, 0xF0, 0x10,
															0xF5, 0xF0, 0x5F, 0xDE, 0x38, 0xDB, 0x88, 0xE8, 0xEF, 0xAE, 0xEC, 0xDF, 0xAB, 0xA3, 0x4C, 0xCC,
															0xAE, 0x04, 0x1A, 0xD9, 0x9E, 0x8F, 0x55, 0xD6, 0x1D, 0x1B, 0x46, 0x5D, 0xC1, 0xEA, 0xC0, 0x85,
															0xCE, 0x00, 0x59, 0xB1, 0xB7, 0xA6, 0x96, 0x05, 0xAF, 0x37, 0xF0, 0x65, 0x78, 0x0C, 0x65, 0x2F,
															0x08, 0x1D, 0x78, 0xE7, 0x2A, 0x59, 0x4E, 0xD0, 0xA6, 0x5C, 0x72, 0x26, 0x3B, 0x05, 0xBD, 0x2B,
															0x61, 0x55, 0x7B, 0xFB, 0x83, 0x51, 0xAC, 0x4B, 0xE6, 0xCA, 0x74, 0x74, 0x9D, 0xB1, 0x9D, 0xA9,
															0x13, 0x8E, 0x12, 0xCE, 0x0D, 0x46, 0xB2, 0x47, 0xBE, 0x3B, 0x70, 0xAE, 0x2E, 0xE3, 0xA3, 0x02,
															0xAF, 0x35, 0xED, 0x90, 0x82, 0x85, 0xF7, 0xE3, 0xA8, 0x21, 0x45, 0x13, 0xD2, 0x9A, 0x57, 0x32,
															0x0A, 0x14, 0xB8, 0x5B, 0xDD, 0xEF, 0xAB, 0xA2, 0x13, 0x9E, 0x3D, 0x73, 0xDB, 0xAF, 0x59, 0x46,
															0x9A, 0x40, 0x94, 0xBC, 0x76, 0xD8, 0xFB, 0x3D, 0x71, 0x2E, 0xA6, 0x9A, 0x4B, 0xD6, 0x35, 0x72,
															0xB8, 0x0E, 0xF0, 0x80, 0x43, 0xA4, 0xDC, 0xC0, 0x8F, 0x34, 0xFA, 0xE3, 0x01, 0x15, 0x53, 0x2C,
															0x14, 0x2E, 0x6D, 0x6A, 0xA2, 0x4C, 0xBD, 0x3A, 0xF6, 0x76, 0x5D, 0x32, 0xA1, 0x88, 0xEF, 0x7F,
															0xEB, 0xF2, 0xCC, 0x03, 0xA5, 0xB7, 0x39, 0x35, 0x38, 0x93, 0xEB, 0x36, 0x23, 0x99, 0x5D, 0x2B,
															0x1E, 0xBB, 0xE9, 0xB8, 0x4B, 0x45, 0xE2, 0x51, 0x41, 0x67, 0x3B, 0x7B, 0xF9, 0x9E, 0x7D, 0x94,
															0x82, 0xA8, 0xA2, 0xB9, 0x25, 0xB0, 0xB4, 0x80, 0x47, 0x4F, 0xEB, 0x82, 0x09, 0x7B, 0x3E, 0x76,
															0xF7, 0xCB, 0xC8, 0x22, 0x7A, 0x77, 0xAD, 0xC9, 0x82, 0xA0, 0x53, 0x31, 0x78, 0x6C, 0x47, 0x9F,
															0x8D, 0xD4, 0xE0, 0xFC, 0xB3, 0x6F, 0xE0, 0xBB, 0xA7, 0x8D, 0x6D, 0xBC, 0x07, 0x3E, 0x7E, 0x8C,
															0x51, 0x03, 0x45, 0x01, 0x3E, 0x8A, 0xCA, 0x8F, 0xA8, 0x82, 0x35, 0xE3, 0xB6, 0xD9, 0x1B, 0xDD,
															0xCE, 0x3F, 0x47, 0x08, 0xBE, 0x26, 0x70, 0xD9, 0x57, 0x35, 0xD9, 0xD4, 0xF0, 0x84, 0x77, 0x64,
															0xBE, 0x17, 0xE2, 0x2C, 0x51, 0x25, 0x2D, 0x6A, 0x70, 0x19, 0x1E, 0x21, 0xD2, 0x3C, 0x69, 0xD7,
															0x0D, 0x3A, 0x54, 0x77, 0x8D, 0x05, 0x16, 0x55, 0xFF, 0xFC, 0xDB, 0x45, 0x22, 0x7A, 0x0D, 0x80,
															0xDC, 0xF4, 0xB6, 0x2C, 0x88, 0x2C, 0x3B, 0x75, 0xEA, 0xA9, 0xDE, 0x19, 0xDC, 0x0D, 0xD7, 0x03,
															0x78, 0xF8, 0x46, 0xF8, 0x9A, 0xD8, 0x47, 0x6E, 0x84, 0x32, 0xBB, 0x47, 0xEF, 0x14, 0x59, 0x45,
															0x14, 0x8E, 0x29, 0x69, 0xA8, 0xEA, 0x7B, 0xAD, 0xDD, 0x1B, 0xC5, 0x6C, 0xD3, 0x83, 0x24, 0xF9,
															0xDD, 0x1B, 0x36, 0x20, 0x45, 0xC4, 0x17, 0x2F, 0x80, 0xB3, 0x47, 0x49, 0xB8, 0xB7, 0x7E, 0x8F,
															0x0D, 0x47, 0x31, 0x37, 0xD2, 0xCB, 0x13, 0xB8, 0x5F, 0x9E, 0x6B, 0xAA, 0x8D, 0x8F, 0x3B, 0x95,
															0xA0, 0x68, 0xCB, 0xAA, 0xE0, 0xB0, 0x55, 0x65, 0x16, 0x38, 0x8A, 0x41, 0xBB, 0xE3, 0x5A, 0x37,
															0xBF, 0x05, 0x2F, 0x82, 0x10, 0x5C, 0x2A, 0x51, 0x08, 0x29, 0x96, 0x96, 0x62, 0x53, 0x8E, 0xF0,
															0x98, 0x27, 0xE7, 0x0E, 0x1B, 0xB8, 0x2A, 0x70, 0xC2, 0x81, 0x86, 0x46, 0x0A, 0xD5, 0x2A, 0x99,
															0xAB, 0x62, 0xE6, 0x89, 0xF5, 0xA8, 0xA1, 0x30, 0x11, 0x82, 0xC2, 0x2D, 0x3B, 0x7C, 0x36, 0xCC,
															0x65, 0xF0, 0x72, 0xF2, 0x7B, 0x82, 0xDB, 0xCB, 0xA5, 0x2E, 0x4E, 0x03, 0x0B, 0xE8, 0x69, 0xC4,
															0xAE, 0xE8, 0x56, 0x6F, 0x86, 0x97, 0x9C, 0x0E, 0x12, 0x81, 0xED, 0x06, 0x66, 0x54, 0xB4, 0xB4,
															0xF8, 0xAE, 0xBF, 0x64, 0xFA, 0x54, 0xFF, 0xA1, 0xA1, 0x4B, 0x4C, 0x57, 0x24, 0xB0, 0x11, 0xDB,
															0xAF, 0x15, 0xA1, 0x8A, 0x12, 0x3D, 0x62, 0x7B, 0x43, 0x26, 0x19, 0xC1, 0x2D, 0x6B, 0xC9, 0x02,
															0x08, 0xD2, 0x82, 0xE6, 0x30, 0x0D, 0x62, 0x26, 0xCA, 0x02, 0x1C, 0x17, 0x59, 0x35, 0xD1, 0x71,
															0x80, 0xB9, 0xDF, 0x92, 0xC6, 0x39, 0x9A, 0x07, 0x17, 0x61, 0xF0, 0x51, 0x51, 0x14, 0xD9, 0xD5,
															0x46, 0x36, 0x99, 0x28, 0x72, 0xDA, 0xB8, 0x03, 0xFC, 0x92, 0xCB, 0x21, 0xF8, 0xF3, 0x2F, 0xDC,
															0xEE, 0x0B, 0x2E, 0xE2, 0xE7, 0x62, 0xB8, 0x7A, 0x69, 0xB6, 0x31, 0xC4, 0x28, 0x70, 0xF3, 0xFF,
															0xEE, 0xC8, 0x55, 0x6D, 0xEE, 0x69, 0xD4, 0x75, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44,
															0xAE, 0x42, 0x60, 0x82 };

static const char *kTestDataBase64						=	"iVBORw0KGgoAAAANSUhEUgAAAD4AAABACAMAAAB1NzTHAAAAVFBMVEWUjP9KKf9rY/9rWv9a\n"
															"Sv9SOf9KOf97a/85GP85CP8pAP97c/9jSv9CGP8YAP+Me/8YAO8QAM4YAN4IAKWMhP8IAJwA\n"
															"AAAQAK0IAGsYALUIAIwYAIyv8nBBAAAHK0lEQVR4XlWW54LruA6DRdViO21O273v/573Azzn\n"
															"xzLj2FIEVpDjtB+vufcDeb1/f36+v97vr3/+/Z929j9fn39/ffEwH+vxeH8e799f78fv9x/9\n"
															"uH59/XynvWOvtdHyACL5+fU2eM0Xm5+Yj8fk83qj46VT+/1a7z/x6/15pZgdJPIR/PV5fD6v\n"
															"F0cieszorfXe295zLs683++9v15fAN5zczZFVI7NNdfi2tGRVhtS/woq+DQdkyxkzh0o3Wmi\n"
															"G1n6WzLXA7wQ4KTov5oitgQNUfE2re4N1LFzy1+8NOH77ZC3JFawQNQp6yUWK3ajWSJsBvGX\n"
															"/fDi3taxuTAWJQxvgchQLQj+VYnj5d5B9s4V17x6rV7EdQFva6dVZHhqMxtuq72Djxl2m0xF\n"
															"rKXo0OUDIKLUNVMM9NWz1COXbO8Qucyzjgd2dWPDgeFjPe1cyQE819lx+8BwNfS+JpBJXpE9\n"
															"g1D/W01iKqPP1EeNUsaw43+rZlQpZ2Wv3zvXtGlO+QpWJSK13EJob/V5NUu0Nau0Dn7uVdW7\n"
															"sFzOXrrxDZdzjVRHb3kcwGNGKy7ZLXUcyKhVaXS0MsPScGyNcqWGC1nWu9ythjv4qGBRDC6u\n"
															"rlQrcPCusA4dLVIUNGdyPjEhtJm5oHd9ojTnYqa1RQjEM0r0sJFuuOk08o0sduLi9MSXI9eS\n"
															"C+aVlXMMMMBr0R105LOnfhJMLtViB+Qn6FYOnUXBt+ZcHXpx+OqF0q4EJnrmi8qyJoSS0Vyj\n"
															"jVx82oFKIaDgXMO8eUT6DY+sNZZPZc3mZtwUVr+MIv4CcBjEn/VABIbTACKMaWWDfSHYrDxH\n"
															"bxeIs2D8WtBiQG6Je3AlFd99YM6qWW5HSU61feGKGTDG8VQ+kFNx9LZSOxoWbKqrkY0e8i+A\n"
															"VxykDPya2csZsMwiHiWR5lFnDUokaivHoguqgk7CRVnHzlksyq05xVmucMMGONJcMyx9/oCm\n"
															"7btHiwguCqFAtFdNZyjXV+hIX8BPPRANDoo9wsWeoFnwBdbtF2uDcPVu7GQ7rZznqk4ZCIv0\n"
															"Wwm6BO9yz0XsnjTacQQd58toOCTEnfhm7iHhZJyhPBSLVUgbf9xLx3o5+gZeXS6lFPB9tHmm\n"
															"yK0CVbI25KZzKQ2597Tqk+woZk3E1ozNIsepfhmsznYC15NTAxZBx2gN+IFhafWWwNADGNfz\n"
															"luNZ3Ps0jUQxCV4H1sVx4FXYy/BcxC9hKo9CSc89Dx2Ao8K4Zt1uGeKw4UxJsiBpiDsOUhMz\n"
															"V7eKm9CiYdQmme/cWTjbBRGY3qkeMuazKGCBhJazUJ98MD3Txot6LaGL4SWb1y4Ql6ENXF0a\n"
															"rlDL7aapd+2d9sJ7WNi7NUg6XOAEOy1Wn+vmQ7nZIxEzoMt+7MSP5dlj3j99tzhPdp39el16\n"
															"0igSM7acFJHy6Pux0t4raMOLZHqEeS5RjFj86eu6wuIZaD1OwwHXN5lHylFMxO7G8jwK+Tzn\n"
															"EkJEI/+lmujhOc0AxveaAv14QnE6Efoj31tIGqJg6jQFqIE+tj6AR0t6n1oNVlTPO1eJBUpj\n"
															"IhFu9G5Ia9O/c2A8K25HAg1+2OBENs6IMNMzwc6qjAUFUtqoVOh5tMdDhdsv7vWZVbAg2Ouy\n"
															"qe+2uM2Kt4Nq1jlVI1d9Gk7+uM1yiGtb8BD18F/eONuI6O+u7N+ro0zMrgQa2Z6PVdYdG0Zd\n"
															"werAhc4AWbG3ppYFrzfwZXgMZS8IHXjnKllO0KZcciY7Bb0rYVV7+4NRrEvmynR0nbGdqROO\n"
															"Es4NRrJHvjtwri7jowKvNe2QgoX346ghRRPSmlcyChS4W93vq6ITnj1z269ZRppAlLx22Ps9\n"
															"cS6mmkvWNXK4DvCAQ6TcwI80+uMBFVMsFC5taqJMvTr2dl0yoYjvf+vyzAOltzk1OJPrNiOZ\n"
															"XSseu+m4S0XiUUFnO3v5nn2UgqiiuSWwtIBHT+uCCXs+dvfLyCJ6d63JgqBTMXhsR5+N1OD8\n"
															"s2/gu6eNbbwHPn6MUQNFAT6Kyo+ogjXjttkb3c4/Rwi+JnDZVzXZ1PCEd2S+F+IsUSUtanAZ\n"
															"HiHSPGnXDTpUd40FFlX//NtFInoNgNz0tiyILDt16qneGdwN1wN4+Eb4mthHboQyu0fvFFlF\n"
															"FI4paajqe63dG8Vs04Mk+d0bNiBFxBcvgLNHSbi3fo8NRzE30ssTuF+ea6qNjzuVoGjLquCw\n"
															"VWUWOIpBu+NaN78FL4IQXCpRCCmWlmJTjvCYJ+cOG7gqcMKBhkYK1SqZq2LmifWooTARgsIt\n"
															"O3w2zGXwcvJ7gtvLpS5OAwvoacSu6FZvhpecDhKB7QZmVLS0+K6/ZPpU/6GhS0xXJLAR268V\n"
															"oYoSPWJ7QyYZwS1ryQII0oLmMA1iJsoCHBdZNdFxgLnfksY5mgcXYfBRURTZ1UY2mShy2rgD\n"
															"/JLLIfjzL9zuCy7i52K4emm2McQocPP/7shVbe5p1HUAAAAASUVORK5CYII=";





//============================================================================
//		TB64Encoder::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TB64Encoder::Execute(void)
{	UInt32			adlerData, adlerSrc;
	NB64Encoder		theEncoder;
	NChecksum		checkSum;
	NString			theText;
	NData			theData;



	// Encode
	theData = NData(GET_ARRAY_SIZE(kTestDataBinary), kTestDataBinary, true);
	theText = theEncoder.Encode(theData);
	NN_ASSERT(theText == kTestDataBase64);



	// Decode
	theData   = theEncoder.Decode(theText);
	adlerData = checkSum.GetAdler32(theData.GetSize(), theData.GetData());
	adlerSrc  = checkSum.GetAdler32(GET_ARRAY_SIZE(kTestDataBinary), kTestDataBinary);
	NN_ASSERT(adlerData == adlerSrc);
}





