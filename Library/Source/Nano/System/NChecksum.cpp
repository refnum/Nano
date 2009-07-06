/*	NAME:
		NChecksum.cpp

	DESCRIPTION:
		Nano checksum support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NChecksum.h"





//============================================================================
//		NChecksum::NChecksum : Constructor.
//----------------------------------------------------------------------------
NChecksum::NChecksum(void)
{
}





//============================================================================
//		NChecksum::~NChecksum : Destructor.
//----------------------------------------------------------------------------
NChecksum::~NChecksum(void)
{
}





//============================================================================
//		NChecksum::GetInternet : Get the internet checksum.
//----------------------------------------------------------------------------
//		Note : From <http://www.rfc-editor.org/rfc/rfc1071.txt>.
//----------------------------------------------------------------------------
UInt16 NChecksum::GetInternet(NIndex theSize, const void *thePtr)
{	UInt32			count, sum, hi, lo;
	const UInt8		*addr;



	/* Compute Internet Checksum */
	count = theSize;
	addr  = (const UInt8 *) thePtr;
	sum   = 0;
	
	while (count > 1)
		{
		hi = *addr++;
		lo = *addr++;

		sum   += ((hi << 8) | lo);
		count -= 2;
		}


	/* Add left-over byte, if any */
	if (count > 0)
		sum += *addr++;


	/* Fold 32-bit sum to 16 bits */
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);

	sum  = ~sum;
	sum &= 0x0000FFFF;

	return(sum);
}





//============================================================================
//		NChecksum::GetDJB2 : Get a a DJB2 checksum.
//----------------------------------------------------------------------------
//		Note : From <http://www.cse.yorku.ca/~oz/hash.html>.
//----------------------------------------------------------------------------
UInt32 NChecksum::GetDJB2(NIndex theSize, const void *thePtr)
{	UInt32			theHash;
	const UInt8		*bytePtr;
	NIndex			n;



	// Get the state we need
	theHash = 5381;
	bytePtr = (const UInt8 *) thePtr;



	// Calculate the hash
	for (n = 0; n < theSize; n++)
		theHash = ((theHash << 5) + theHash) + bytePtr[n];
	
	return(theHash);
}





//============================================================================
//		NChecksum::GetAdler32 : Get an Adler32 checksum.
//----------------------------------------------------------------------------
//		Note : ZLib's adler32.c. For conditions of use, see zlib.h.
//----------------------------------------------------------------------------
UInt32 NChecksum::GetAdler32(NIndex theSize, const void *thePtr, UInt32 prevAdler)
{
	// Setup
	#define BASE 65521UL    /* largest prime smaller than 65536 */
	#define NMAX 5552		/* NMAX is the largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1 */

	#define DO1(buf,i)  {adler += (buf)[i]; sum2 += adler;}
	#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
	#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
	#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
	#define DO16(buf)   DO8(buf,0); DO8(buf,8);

	#define MOD(a) a %= BASE
	#define MOD4(a) a %= BASE

	UInt16      adler = prevAdler;
	const UInt8  *buf = (const UInt8 *) thePtr;
	UInt32        len = theSize;

    UInt32 sum2;
    UInt32 n;



    /* split Adler-32 into component sums */
    sum2 = (((UInt32) adler) >> 16) & 0xffff;
    adler &= 0xffff;

    /* in case user likes doing a byte at a time, keep it fast */
    if (len == 1) {
        adler += buf[0];
        if (adler >= BASE)
            adler -= BASE;
        sum2 += adler;
        if (sum2 >= BASE)
            sum2 -= BASE;
        return adler | (sum2 << 16);
    }

    /* initial Adler-32 value (deferred check for len == 1 speed) */
    if (buf == NULL)
        return 1L;

    /* in case short lengths are provided, keep it somewhat fast */
    if (len < 16) {
        while (len--) {
            adler += *buf++;
            sum2 += adler;
        }
        if (adler >= BASE)
            adler -= BASE;
        MOD4(sum2);             /* only added so many BASE's */
        return adler | (sum2 << 16);
    }

    /* do length NMAX blocks -- requires just one modulo operation */
    while (len >= NMAX) {
        len -= NMAX;
        n = NMAX / 16;          /* NMAX is divisible by 16 */
        do {
            DO16(buf);          /* 16 sums unrolled */
            buf += 16;
        } while (--n);
        MOD(adler);
        MOD(sum2);
    }

    /* do remaining bytes (less than NMAX, still just one modulo) */
    if (len) {                  /* avoid modulos if none remaining */
        while (len >= 16) {
            len -= 16;
            DO16(buf);
            buf += 16;
        }
        while (len--) {
            adler += *buf++;
            sum2 += adler;
        }
        MOD(adler);
        MOD(sum2);
    }

    /* return recombined sums */
    return adler | (sum2 << 16);
}





