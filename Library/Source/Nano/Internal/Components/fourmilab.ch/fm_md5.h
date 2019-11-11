#ifndef FM_MD5_H
#define FM_MD5_H

#ifdef __cplusplus
extern "C" {
#endif



/****************************************************************************
 *		Nano																*
 ****************************************************************************/
#if defined(NANO_TARGET_HDR)

#if NN_ENDIAN_BIG
	#define HIGHFIRST
#endif

#ifndef _UINT32
	typedef uint32_t uint32;
#endif



#else // !NANO_TARGET_HDR

/*  The following tests optimise behaviour on little-endian
    machines, where there is no need to reverse the byte order
    of 32 bit words in the MD5 computation.  By default,
    HIGHFIRST is defined, which indicates we're running on a
    big-endian (most significant byte first) machine, on which
    the byteReverse function in md5.c must be invoked. However,
    byteReverse is coded in such a way that it is an identity
    function when run on a little-endian machine, so calling it
    on such a platform causes no harm apart from wasting time. 
    If the platform is known to be little-endian, we speed
    things up by undefining HIGHFIRST, which defines
    byteReverse as a null macro.  Doing things in this manner
    insures we work on new platforms regardless of their byte
    order.  */

#define HIGHFIRST

#ifdef __i386__
#undef HIGHFIRST
#endif

/*  On machines where "long" is 64 bits, we need to declare
    uint32 as something guaranteed to be 32 bits.  */
#if NN_ARCH_64 || defined(__alpha)
typedef unsigned int uint32;
#else
typedef unsigned long uint32;
#endif

#endif // !NANO_TARGET_HDR



struct MD5Context {
        uint32 buf[4];
        uint32 bits[2];
        unsigned char in[64];
};

extern void MD5Init(struct MD5Context *ctx);
extern void MD5Update(struct MD5Context *ctx, const unsigned char *buf, size_t len);
extern void MD5Final(unsigned char digest[16], struct MD5Context *ctx);
extern void MD5Transform(uint32 buf[4], uint32 in[16]);

/*
 * This is needed to make RSAREF happy on some MS-DOS compilers.
 */
typedef struct MD5Context MD5_CTX;

/*  Define CHECK_HARDWARE_PROPERTIES to have main,c verify
    byte order and uint32 settings.  */
#define CHECK_HARDWARE_PROPERTIES



#ifdef    __cplusplus
}
#endif /* __cplusplus */



#endif /* !FM_MD5_H */
