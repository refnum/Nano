/*	NAME:
		TDataEncoder.cpp

	DESCRIPTION:
		NDataEncoder tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDataEncoder.h"
#include "NTestFixture.h"
#include "NDataDigest.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt8 kTestDataBinary[]							= { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
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

static const char *kTestDataHex									=	"89504E470D0A1A0A0000000D494844520000003E000000400803000000753734C700000054"
																	"504C5445948CFF4A29FF6B63FF6B5AFF5A4AFF5239FF4A39FF7B6BFF3918FF3908FF2900FF"
																	"7B73FF634AFF4218FF1800FF8C7BFF1800EF1000CE1800DE0800A58C84FF08009C00000010"
																	"00AD08006B1800B508008C18008CAFF270410000072B49444154785E5596E782EBB80E8345"
																	"D5623B6D4EDBBDEFFF9EF7033CE7C732E3D852045690E3B41FAFB9F70379BD7F7F7EBEBFDE"
																	"EFAF7FFEFD9F76F63F5F9F7F7F7DF1301FEBF1787F1EEFDF5FEFC7EFF71FFDB87E7DFD7CA7"
																	"BD63AFB5D1F20022F9F9F53678CD179B9F988FC7E4F37AA3E3A553FBFD5AEF3FF1EBFD79A5"
																	"981D24F211FCF5797C3EAF1747227ACCE8ADF5DEDBDE732ECEBCDFEFBDBF5E5F00DE737336"
																	"45548ECD35D7E2DAD191561B52FF0A2AF8341D932C64CE1D28DD69A21B59FA5B32D703BC10"
																	"E0A4E8BF9A22B6040D51F136ADEE0DD4B173CB5FBC34E1FBED90B72456B040D429EB25162B"
																	"76A359226C06F197FDF0E2DED6B1B93016250C6F81C8502D08FE5589E3E5DE41F6CE15D7BC"
																	"7AAD5EC475016F6BA75564786A331B6EABBD838F19769B4C45ACA5E8D0E50320A2D435530C"
																	"F4D5B3D423976CEF10B9CCB38E07767563C381E1633DED5CC9013CD7D971FBC07035F4BE26"
																	"90495E913D8350FF5B4D622AA3CFD4478D52C6B0E37FAB6654296765AFDF3BD7B4694EF90A"
																	"562522B5DC42686FF579354BB435ABB40E7EEE55D5BBB05CCE5EBAF10D97738D54476F791C"
																	"C063462B2ED92D751CC8A8556974B432C3D2706C8D72A5860B59D6BBDCAD863BF8A860510C"
																	"2EAEAE542B70F0AEB00E1D2D52143467723E3121B499B9A0777DA234E762A6B54508C4334A"
																	"F4B0916EB8E934F28D2C76E2E2F4C49723D7920BE69595730C30C06BD11D74E4B3A77E124C"
																	"2ED56207E427E8560E9D45C1B7E65C1D7A71F8EA85D2AE04267AE68BCAB2268492D15CA38D"
																	"5C7CDA814A21A0E05CC3BC7944FA0D8FAC35964F65CDE666DC1456BF8C22FE027018C49FF5"
																	"400486D300228C6965837D21D8AC3C476F1788B360FC5AD062406E897B702515DF7D60CEAA"
																	"596E47494EB57DE18A1930C6F1543E905371F4B6523B1A166CAAAB918D1EF22F80571CA40C"
																	"FC9AD9CB19B0CC221E2591E651670D4A246A2BC7A20BAA824EC24559C7CE592CCAAD39C559"
																	"AE70C30638D25C332C7DFE80A6EDBB478B082E0AA140B4574D6728D757E8485FC04F3D100D"
																	"0E8A3DC2C59EA059F005D6ED176B8370F56EEC643BAD9CE7AA4E19088BF45B09BA04EF72CF"
																	"45EC9E34DA71041DE7CB683824C49DF866EE21E1649CA13C148B55481B7FDC4BC77A39FA06"
																	"5E5D2EA514F07DB479A6C8AD0255B236E4A673290DB9F7B4EA93EC28664DC4D68CCD22C7A9"
																	"7E19ACCE7602D79353031641C7680DF8816169F596C0D00318D7F396E359DCFB348D443109"
																	"5E07D6C571E055D8CBF05CC42F612A8F4249CF3D0F1D80A3C2B866DD6E19E2B0E14C49B220"
																	"69883B0E52133357B78A9BD0A261D42699EFDC5938DB051198DEA91E32E6B32860818496B3"
																	"509F7C303DD3C68B7A2DA18BE1259BD72E1097A10D5C5D1AAE50CBEDA6A977ED9DF6C27B58"
																	"D8BB35483A5CE0043B2D569FEBE643B9D9231133A0CB7EECC48FE5D963DE3F7DB7384F769D"
																	"FD7A5D7AD2281233B69C1491F2E8FBB1D2DE2B68C38B647A84792E518C58FCE9EBBAC2E219"
																	"683D4EC301D7379947CA514CC4EEC6F23C0AF93CE712424423FFA59AE8E139CD00C6F79A02"
																	"FD7842713A11FA23DF5B481AA260EA3405A8813EB63E80474B7A9F5A0D5654CF3B5789054A"
																	"6322116EF46E486BD3BF73603C2B6E47020D7ED8E04436CE8830D333C1CEAA8C050552DAA8"
																	"54E879B4C74385DB2FEEF59955B020D8EBB2A9EFB6B8CD8AB7836AD6395523577D1A4EFEB8"
																	"CD72886B5BF010F5F05FDE38DB88E8EFAEECDFABA34CCCAE041AD99E8F55D61D1B465DC1EA"
																	"C085CE0059B1B7A69605AF37F065780C652F081D78E72A594ED0A65C72263B05BD2B61557B"
																	"FB8351AC4BE6CA74749DB19DA9138E12CE0D46B247BE3B70AE2EE3A302AF35ED908285F7E3"
																	"A8214513D29A57320A14B85BDDEFABA2139E3D73DBAF59469A4094BC76D8FB3D712EA69A4B"
																	"D63572B80EF08043A4DCC08F34FAE30115532C142E6D6AA24CBD3AF6765D32A188EF7FEBF2"
																	"CC03A5B739353893EB3623995D2B1EBBE9B84B45E25141673B7BF99E7D9482A8A2B925B0B4"
																	"80474FEB82097B3E76F7CBC8227A77ADC982A05331786C479F8DD4E0FCB36FE0BBA78D6DBC"
																	"073E7E8C510345013E8ACA8FA88235E3B6D91BDDCE3F4708BE2670D95735D9D4F0847764BE"
																	"17E22C51252D6A70191E21D23C69D70D3A54778D051655FFFCDB45227A0D80DCF4B62C882C"
																	"3B75EAA9DE19DC0DD70378F846F89AD8476E8432BB47EF145945148E2969A8EA7BADDD1BC5"
																	"6CD38324F9DD1B362045C4172F80B34749B8B77E8F0D473137D2CB13B85F9E6BAA8D8F3B95"
																	"A068CBAAE0B0556516388A41BBE35A37BF052F82105C2A510829969662538EF09827E70E1B"
																	"B82A70C28186460AD52A99AB62E689F5A8A1301182C22D3B7C36CC65F072F27B82DBCBA52E"
																	"4E030BE869C4AEE8566F86979C0E1281ED066654B4B4F8AEBF64FA54FFA1A14B4C5724B011"
																	"DBAF15A18A123D627B432619C12D6BC90208D282E6300D6226CA021C175935D17180B9DF92"
																	"C6399A071761F0515114D9D54636992872DAB803FC92CB21F8F32FDCEE0B2EE2E762B87A69"
																	"B631C42870F3FFEEC8556DEE69D4750000000049454E44AE426082";

static const char *kTestDataB64									=	"iVBORw0KGgoAAAANSUhEUgAAAD4AAABACAMAAAB1NzTHAAAAVFBMVEWUjP9KKf9rY/9rWv9a\n"
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
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NDATAENCODER(...)										TEST_NANO(TDataEncoder, ##__VA_ARGS__)

FIXTURE_NANO(TDataEncoder)
{
	UInt32				srcAdler, dstAdler;
	NData				srcData, dstData;
	NDataEncoder		theEncoder;
	NDataDigest			theDigest;
	NString				theText;
	
	SETUP
	{
		srcData  = NData(NN_ARRAY_SIZE(kTestDataBinary), kTestDataBinary, true);
		srcAdler = theDigest.GetAdler32(srcData);
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATAENCODER("Hex")
{


	// Perform the test
	theText = theEncoder.Encode(srcData, kNDataEncodingHex);
	REQUIRE(theText == kTestDataHex);
	
	dstData  = theEncoder.Decode(theText, kNDataEncodingHex);
	dstAdler = theDigest.GetAdler32(dstData);
	REQUIRE(srcAdler == dstAdler);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATAENCODER("B64")
{


	// Perform the test
	theText = theEncoder.Encode(srcData, kNDataEncodingB64);
	REQUIRE(theText == kTestDataB64);
	
	dstData  = theEncoder.Decode(theText, kNDataEncodingB64);
	dstAdler = theDigest.GetAdler32(dstData);
	REQUIRE(srcAdler == dstAdler);
}



