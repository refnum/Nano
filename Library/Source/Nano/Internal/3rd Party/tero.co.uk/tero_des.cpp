/*	NAME:
		tero_des.c

	DESCRIPTION:
		DES/DES3 encryption.

		Originally from a C implementation at:

			http://www.shopable.co.uk/des.html

		That web site no longer exists, however it appears to have been derived
		from the JavaScript implementation at:

			http://tero.co.uk/des/

		Which was itself derived from an SSL implementation by Eric Young. As such,
		Eric Young's original copyright notice is reproduced below.

	COPYRIGHT:
		Copyright (C) 1995 Eric Young (eay@mincom.oz.au)
		All rights reserved.

		This file is part of an SSL implementation written
		by Eric Young (eay@mincom.oz.au).
		The implementation was written so as to conform with Netscapes SSL
		specification.  This library and applications are
		FREE FOR COMMERCIAL AND NON-COMMERCIAL USE
		as long as the following conditions are aheared to.

		Copyright remains Eric Young's, and as such any Copyright notices in
		the code are not to be removed.  If this code is used in a product,
		Eric Young should be given attribution as the author of the parts used.
		This can be in the form of a textual message at program startup or
		in documentation (online or textual) provided with the package.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		1. Redistributions of source code must retain the copyright
		   notice, this list of conditions and the following disclaimer.
		2. Redistributions in binary form must reproduce the above copyright
		   notice, this list of conditions and the following disclaimer in the
		   documentation and/or other materials provided with the distribution.
		3. All advertising materials mentioning features or use of this software
		   must display the following acknowledgement:
		   This product includes software developed by Eric Young (eay@mincom.oz.au)

		THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
		ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
		IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
		ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
		FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
		DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
		OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
		HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
		LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
		OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.

		The licence and distribution terms for any publically available version or
		derivative of this code cannot be changed.  i.e. this code cannot simply be
		copied and put under another distribution licence
		[including the GNU Public Licence.]
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "tero_des.h"





//=============================================================================
//		des_gen_keys : Generate the DES keys.
//-----------------------------------------------------------------------------
//		Note :	A DES key is 64 bits (8 bytes), and a DES3 key is 192 bits (24
//				bytes). Excessive key bits are ignored.
//-----------------------------------------------------------------------------
static void des_gen_keys(size_t keySize, const void *keyPtr, bool tripleDES, UInt32 *outKeys)
{	UInt32	m, n, lefttemp, righttemp, left, right, temp, iterations;
	UInt32	pc2bytes0[]  = {0,0x4,0x20000000,0x20000004,0x10000,0x10004,0x20010000,0x20010004,0x200,0x204,0x20000200,0x20000204,0x10200,0x10204,0x20010200,0x20010204};
	UInt32	pc2bytes1[]  = {0,0x1,0x100000,0x100001,0x4000000,0x4000001,0x4100000,0x4100001,0x100,0x101,0x100100,0x100101,0x4000100,0x4000101,0x4100100,0x4100101};
	UInt32	pc2bytes2[]  = {0,0x8,0x800,0x808,0x1000000,0x1000008,0x1000800,0x1000808,0,0x8,0x800,0x808,0x1000000,0x1000008,0x1000800,0x1000808};
	UInt32	pc2bytes3[]  = {0,0x200000,0x8000000,0x8200000,0x2000,0x202000,0x8002000,0x8202000,0x20000,0x220000,0x8020000,0x8220000,0x22000,0x222000,0x8022000,0x8222000};
	UInt32	pc2bytes4[]  = {0,0x40000,0x10,0x40010,0,0x40000,0x10,0x40010,0x1000,0x41000,0x1010,0x41010,0x1000,0x41000,0x1010,0x41010};
	UInt32	pc2bytes5[]  = {0,0x400,0x20,0x420,0,0x400,0x20,0x420,0x2000000,0x2000400,0x2000020,0x2000420,0x2000000,0x2000400,0x2000020,0x2000420};
	UInt32	pc2bytes6[]  = {0,0x10000000,0x80000,0x10080000,0x2,0x10000002,0x80002,0x10080002,0,0x10000000,0x80000,0x10080000,0x2,0x10000002,0x80002,0x10080002};
	UInt32	pc2bytes7[]  = {0,0x10000,0x800,0x10800,0x20000000,0x20010000,0x20000800,0x20010800,0x20000,0x30000,0x20800,0x30800,0x20020000,0x20030000,0x20020800,0x20030800};
	UInt32	pc2bytes8[]  = {0,0x40000,0,0x40000,0x2,0x40002,0x2,0x40002,0x2000000,0x2040000,0x2000000,0x2040000,0x2000002,0x2040002,0x2000002,0x2040002};
	UInt32	pc2bytes9[]  = {0,0x10000000,0x8,0x10000008,0,0x10000000,0x8,0x10000008,0x400,0x10000400,0x408,0x10000408,0x400,0x10000400,0x408,0x10000408};
	UInt32	pc2bytes10[]  = {0,0x20,0,0x20,0x100000,0x100020,0x100000,0x100020,0x2000,0x2020,0x2000,0x2020,0x102000,0x102020,0x102000,0x102020};
	UInt32	pc2bytes11[]  = {0,0x1000000,0x200,0x1000200,0x200000,0x1200000,0x200200,0x1200200,0x4000000,0x5000000,0x4000200,0x5000200,0x4200000,0x5200000,0x4200200,0x5200200};
	UInt32	pc2bytes12[]  = {0,0x1000,0x8000000,0x8001000,0x80000,0x81000,0x8080000,0x8081000,0x10,0x1010,0x8000010,0x8001010,0x80010,0x81010,0x8080010,0x8081010};
	UInt32	pc2bytes13[]  = {0,0x4,0x100,0x104,0,0x4,0x100,0x104,0x1,0x5,0x101,0x105,0x1,0x5,0x101,0x105};
	UInt32	key[24];


	// Prepare the key
	if (keySize >= sizeof(key))
		keySize = sizeof(key);
	
	memset(key, 0x00, sizeof(key));
	memcpy(key, keyPtr, keySize);


	// how many iterations (1 for des, 3 for triple des)
	iterations = tripleDES ? 3 : 1;
	

	// now define the left shifts which need to be done
	UInt8	shifts[] = {0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0};
	UInt32	shiftCount = sizeof(shifts);


	m = n = lefttemp = righttemp = left = right = temp = 0;

	for (UInt32 j=0; j < iterations; j++)
	{
		// either 1 or 3 iterations
		left	= (key[m]);
		right	= (key[m+1]);
		
		m+=2;

		temp = ((left >> 4) ^  right) & 0x0f0f0f0f; right ^= temp; left  ^= (temp << 4);
		temp = ((right >>  16)^ left) & 0x0000ffff; left ^=  temp; right ^= (temp <<  16);
		temp = ((left >> 2) ^  right) & 0x33333333; right ^= temp; left  ^= (temp << 2);
		temp = ((right >>  16)^ left) & 0x0000ffff; left ^=  temp; right ^= (temp <<  16);
		temp = ((left >> 1) ^  right) & 0x55555555; right ^= temp; left  ^= (temp << 1);
		temp = ((right >> 8) ^  left) & 0x00ff00ff; left ^=  temp; right ^= (temp << 8);
		temp = ((left >> 1) ^  right) & 0x55555555; right ^= temp; left  ^= (temp << 1);

		// the right side needs to be shifted and to get the last four bits of the left side
		temp = (left << 8) | ((right >> 20) & 0x000000f0);
		// left needs to be put upside down
		left = (right << 24) | ((right << 8) & 0xff0000) | ((right >> 8) & 0xff00) | ((right >> 24) & 0xf0);
		right = temp;

		// now go through and perform these shifts on the left and right keys
		for (UInt32 i = 0; i < shiftCount; i++)
		{
			// shift the keys either one or two bits to the left
			if (shifts[i])
			{
				left = (left << 2) | (left >> 26); 
				right = (right << 2) | (right >> 26);
				left<<=0;right<<=0;
			}
			else
			{
				left = (left << 1) | (left >> 27); 
				right = (right << 1) | (right >> 27);
				left<<=0;right<<=0;
			}
			left &= 0xfffffff0; right &= 0xfffffff0;

			// now apply PC-2, in such a way that E is easier when encrypting or decrypting
			// this conversion will look like PC-2 except only the last 6 bits of each byte are used
			// rather than 48 consecutive bits and the order of lines will be according to 
			// how the S selection functions will be applied: S2, S4, S6, S8, S1, S3, S5, S7
			lefttemp = pc2bytes0[left >> 28] | pc2bytes1[(left >> 24) & 0xf]
							| pc2bytes2[(left >> 20) & 0xf] | pc2bytes3[(left >> 16) & 0xf]
							| pc2bytes4[(left >> 12) & 0xf] | pc2bytes5[(left >> 8) & 0xf]
							| pc2bytes6[(left >> 4) & 0xf];
			righttemp = pc2bytes7[right >> 28] | pc2bytes8[(right >> 24) & 0xf]
							| pc2bytes9[(right >> 20) & 0xf] | pc2bytes10[(right >> 16) & 0xf]
							| pc2bytes11[(right >> 12) & 0xf] | pc2bytes12[(right >> 8) & 0xf]
							| pc2bytes13[(right >> 4) & 0xf];
			temp = ((righttemp >> 16) ^ lefttemp) & 0x0000ffff; 
			outKeys[n++] = lefttemp ^ temp; outKeys[n++] = righttemp ^ (temp << 16);

		}
	}
}





//=============================================================================
//		des_convert : Encrypt/Decrypt data.
//-----------------------------------------------------------------------------
static void des_convert(size_t keySize, const void *keyPtr, size_t dataSize, void *dataPtr, bool doEncrypt, bool tripleDES)
{
	UInt32	*message = (UInt32 *)dataPtr;
	UInt32	m, i, j, temp, right1, right2, left, right;
	UInt32	endloop, loopinc, iterations;
	UInt32	spfunction1[]	= {0x1010400,0,0x10000,0x1010404,0x1010004,0x10404,0x4,0x10000,0x400,0x1010400,0x1010404,0x400,0x1000404,0x1010004,0x1000000,0x4,0x404,0x1000400,0x1000400,0x10400,0x10400,0x1010000,0x1010000,0x1000404,0x10004,0x1000004,0x1000004,0x10004,0,0x404,0x10404,0x1000000,0x10000,0x1010404,0x4,0x1010000,0x1010400,0x1000000,0x1000000,0x400,0x1010004,0x10000,0x10400,0x1000004,0x400,0x4,0x1000404,0x10404,0x1010404,0x10004,0x1010000,0x1000404,0x1000004,0x404,0x10404,0x1010400,0x404,0x1000400,0x1000400,0,0x10004,0x10400,0,0x1010004};
	UInt32	spfunction2[]	= {0x80108020,0x80008000,0x8000,0x108020,0x100000,0x20,0x80100020,0x80008020,0x80000020,0x80108020,0x80108000,0x80000000,0x80008000,0x100000,0x20,0x80100020,0x108000,0x100020,0x80008020,0,0x80000000,0x8000,0x108020,0x80100000,0x100020,0x80000020,0,0x108000,0x8020,0x80108000,0x80100000,0x8020,0,0x108020,0x80100020,0x100000,0x80008020,0x80100000,0x80108000,0x8000,0x80100000,0x80008000,0x20,0x80108020,0x108020,0x20,0x8000,0x80000000,0x8020,0x80108000,0x100000,0x80000020,0x100020,0x80008020,0x80000020,0x100020,0x108000,0,0x80008000,0x8020,0x80000000,0x80100020,0x80108020,0x108000};
	UInt32	spfunction3[]	= {0x208,0x8020200,0,0x8020008,0x8000200,0,0x20208,0x8000200,0x20008,0x8000008,0x8000008,0x20000,0x8020208,0x20008,0x8020000,0x208,0x8000000,0x8,0x8020200,0x200,0x20200,0x8020000,0x8020008,0x20208,0x8000208,0x20200,0x20000,0x8000208,0x8,0x8020208,0x200,0x8000000,0x8020200,0x8000000,0x20008,0x208,0x20000,0x8020200,0x8000200,0,0x200,0x20008,0x8020208,0x8000200,0x8000008,0x200,0,0x8020008,0x8000208,0x20000,0x8000000,0x8020208,0x8,0x20208,0x20200,0x8000008,0x8020000,0x8000208,0x208,0x8020000,0x20208,0x8,0x8020008,0x20200};
	UInt32	spfunction4[]	= {0x802001,0x2081,0x2081,0x80,0x802080,0x800081,0x800001,0x2001,0,0x802000,0x802000,0x802081,0x81,0,0x800080,0x800001,0x1,0x2000,0x800000,0x802001,0x80,0x800000,0x2001,0x2080,0x800081,0x1,0x2080,0x800080,0x2000,0x802080,0x802081,0x81,0x800080,0x800001,0x802000,0x802081,0x81,0,0,0x802000,0x2080,0x800080,0x800081,0x1,0x802001,0x2081,0x2081,0x80,0x802081,0x81,0x1,0x2000,0x800001,0x2001,0x802080,0x800081,0x2001,0x2080,0x800000,0x802001,0x80,0x800000,0x2000,0x802080};
	UInt32	spfunction5[]	= {0x100,0x2080100,0x2080000,0x42000100,0x80000,0x100,0x40000000,0x2080000,0x40080100,0x80000,0x2000100,0x40080100,0x42000100,0x42080000,0x80100,0x40000000,0x2000000,0x40080000,0x40080000,0,0x40000100,0x42080100,0x42080100,0x2000100,0x42080000,0x40000100,0,0x42000000,0x2080100,0x2000000,0x42000000,0x80100,0x80000,0x42000100,0x100,0x2000000,0x40000000,0x2080000,0x42000100,0x40080100,0x2000100,0x40000000,0x42080000,0x2080100,0x40080100,0x100,0x2000000,0x42080000,0x42080100,0x80100,0x42000000,0x42080100,0x2080000,0,0x40080000,0x42000000,0x80100,0x2000100,0x40000100,0x80000,0,0x40080000,0x2080100,0x40000100};
	UInt32	spfunction6[]	= {0x20000010,0x20400000,0x4000,0x20404010,0x20400000,0x10,0x20404010,0x400000,0x20004000,0x404010,0x400000,0x20000010,0x400010,0x20004000,0x20000000,0x4010,0,0x400010,0x20004010,0x4000,0x404000,0x20004010,0x10,0x20400010,0x20400010,0,0x404010,0x20404000,0x4010,0x404000,0x20404000,0x20000000,0x20004000,0x10,0x20400010,0x404000,0x20404010,0x400000,0x4010,0x20000010,0x400000,0x20004000,0x20000000,0x4010,0x20000010,0x20404010,0x404000,0x20400000,0x404010,0x20404000,0,0x20400010,0x10,0x4000,0x20400000,0x404010,0x4000,0x400010,0x20004010,0,0x20404000,0x20000000,0x400010,0x20004010};
	UInt32	spfunction7[]	= {0x200000,0x4200002,0x4000802,0,0x800,0x4000802,0x200802,0x4200800,0x4200802,0x200000,0,0x4000002,0x2,0x4000000,0x4200002,0x802,0x4000800,0x200802,0x200002,0x4000800,0x4000002,0x4200000,0x4200800,0x200002,0x4200000,0x800,0x802,0x4200802,0x200800,0x2,0x4000000,0x200800,0x4000000,0x200800,0x200000,0x4000802,0x4000802,0x4200002,0x4200002,0x2,0x200002,0x4000000,0x4000800,0x200000,0x4200800,0x802,0x200802,0x4200800,0x802,0x4000002,0x4200802,0x4200000,0x200800,0,0x2,0x4200802,0,0x200802,0x4200000,0x800,0x4000002,0x4000800,0x800,0x200002};
	UInt32	spfunction8[]	= {0x10001040,0x1000,0x40000,0x10041040,0x10000000,0x10001040,0x40,0x10000000,0x40040,0x10040000,0x10041040,0x41000,0x10041000,0x41040,0x1000,0x40,0x10040000,0x10000040,0x10001000,0x1040,0x41000,0x40040,0x10040040,0x10041000,0x1040,0,0,0x10040040,0x10000040,0x10001000,0x41040,0x40000,0x41040,0x40000,0x10041000,0x1000,0x40,0x10040040,0x1000,0x41040,0x10001000,0x40,0x10000040,0x10040000,0x10040040,0x10000000,0x40000,0x10001040,0,0x10041040,0x40040,0x10000040,0x10040000,0x10001000,0x10001040,0,0x10041040,0x41000,0x41000,0x1040,0x1040,0x40040,0x10000000,0x10041000};
	UInt32	keys[96];



	// theData must be a multiple of 64 bits long
	NN_ASSERT((dataSize % 8) == 0);
	
	
	// Generate the DES keys
	des_gen_keys(keySize, keyPtr, tripleDES, keys);
	
	
	// set up the loops for single and triple des
	iterations = tripleDES ? 9 : 3;
	
	SInt32	*looping;
	
	SInt32	loop3Enc[] = {0, 32, 2}, loop9Enc[] = {0, 32, 2, 62, 30, -2, 64, 96, 2};
	SInt32	loop3Dec[] = {30, -2, -2}, loop9Dec[] = {94, 62, -2, 32, 64, 2, 30, -2, -2};
	
	
	looping = doEncrypt ?	(tripleDES ? loop9Enc : loop3Enc) :
							(tripleDES ? loop9Dec : loop3Dec);
	
	m = i = j = temp = right1 = right2 = left = right = 0;
	

	// loop through each 64 bit chunk of the message
	while (m < (dataSize / sizeof(UInt32)))
	{
		left = message[m];
		right = message[m+1];

		// first each 64 but chunk of the message must be permuted according to IP
		temp = ((left >> 4) ^ right) & 0x0f0f0f0f; right ^= temp; left ^= (temp << 4);
		temp = ((left >> 16) ^ right) & 0x0000ffff; right ^= temp; left ^= (temp << 16);
		temp = ((right >> 2) ^ left) & 0x33333333; left ^= temp; right ^= (temp << 2);
		temp = ((right >> 8) ^ left) & 0x00ff00ff; left ^= temp; right ^= (temp << 8);
		temp = ((left >> 1) ^ right) & 0x55555555; right ^= temp; left ^= (temp << 1);

		left = ((left << 1) | (left >> 31)); 
		right = ((right << 1) | (right >> 31)); 

		// do this either 1 or 3 times for each chunk of the message
		for (j=0; j<iterations; j+=3)
		{
			endloop =looping[j+1]; loopinc =looping[j+2]; // now go through and perform the encryption or decryption  
			
			for (i=looping[j]; i!=endloop; i+=loopinc)
			{
				right1 =right ^ keys[i]; 
				right2 =((right >> 4) | (right << 28)) ^ keys[i+1];
				// the result is attained by passing these bytes through the S selection functions
				temp = left;
				left = right;
				right = temp ^ (spfunction2[(right1 >> 24) & 0x3f] | spfunction4[(right1 >> 16) & 0x3f]
									| spfunction6[(right1 >>  8) & 0x3f] | spfunction8[right1 & 0x3f]
									| spfunction1[(right2 >> 24) & 0x3f] | spfunction3[(right2 >> 16) & 0x3f]
									| spfunction5[(right2 >>  8) & 0x3f] | spfunction7[right2 & 0x3f]);
			}
			temp = left; left = right; right = temp; // unreverse left and right
		}

		// move then each one bit to the right
		left = ((left >> 1) | (left << 31)); 
		right = ((right >> 1) | (right << 31)); 

		// now perform IP-1, which is IP in the opposite direction
		temp = ((left >> 1) ^ right) & 0x55555555; right ^= temp; left ^= (temp << 1);
		temp = ((right >> 8) ^ left) & 0x00ff00ff; left ^= temp; right ^= (temp << 8);
		temp = ((right >> 2) ^ left) & 0x33333333; left ^= temp; right ^= (temp << 2);
		temp = ((left >> 16) ^ right) & 0x0000ffff; right ^= temp; left ^= (temp << 16);
		temp = ((left >> 4) ^ right) & 0x0f0f0f0f; right ^= temp; left ^= (temp << 4);
		
		message[m] = left;
		message[m+1] = right;
		
		m+=2;
	}
}





//=============================================================================
//		DES_Encrypt : Encrypt with DES.
//-----------------------------------------------------------------------------
void DES_Encrypt(size_t keySize, const void *keyPtr, size_t dataSize, void *dataPtr)
{


	// Encrypt the data
	des_convert(keySize, keyPtr, dataSize, dataPtr, true, false);
}





//=============================================================================
//		DES_Decrypt : Decrypt with DES.
//-----------------------------------------------------------------------------
void DES_Decrypt(size_t keySize, const void *keyPtr, size_t dataSize, void *dataPtr)
{


	// Encrypt the data
	des_convert(keySize, keyPtr, dataSize, dataPtr, false, false);
}





//=============================================================================
//		DES3_Encrypt : Encrypt with DES3.
//-----------------------------------------------------------------------------
void DES3_Encrypt(size_t keySize, const void *keyPtr, size_t dataSize, void *dataPtr)
{


	// Encrypt the data
	des_convert(keySize, keyPtr, dataSize, dataPtr, true, true);
}





//=============================================================================
//		DES3_Decrypt : Decrypt with DES3.
//-----------------------------------------------------------------------------
void DES3_Decrypt(size_t keySize, const void *keyPtr, size_t dataSize, void *dataPtr)
{


	// Encrypt the data
	des_convert(keySize, keyPtr, dataSize, dataPtr, false, true);
}



