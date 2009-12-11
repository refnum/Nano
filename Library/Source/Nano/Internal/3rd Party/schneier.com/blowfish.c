// Written by Andrew Carter (2008)
#include <stdio.h>
#include <stdlib.h>
#include "blowfish.h"

#define BLOWFISH_F(x) \
	(((ctx->sbox[0][x >> 24] + ctx->sbox[1][(x >> 16) & 0xFF]) \
	^ ctx->sbox[2][(x >> 8) & 0xFF]) + ctx->sbox[3][x & 0xFF])

void blowfish_encryptblock(blowfish_context_t *ctx, unsigned long *hi, unsigned long *lo)
{
	int i, temp;

	for(i = 0; i < 16; i++) {
		*hi ^= ctx->pbox[i];
		*lo ^= BLOWFISH_F(*hi);
		temp = *hi, *hi = *lo, *lo = temp;
	}
	temp = *hi, *hi = *lo, *lo = temp;

	*lo ^= ctx->pbox[16];
	*hi ^= ctx->pbox[17];
}

void blowfish_decryptblock(blowfish_context_t *ctx, unsigned long *hi, unsigned long *lo)
{
	int i, temp;

	for(i = 17; i > 1; i--) {
		*hi ^= ctx->pbox[i];
		*lo ^= BLOWFISH_F(*hi);
		temp = *hi, *hi = *lo, *lo = temp;
	}
	temp = *hi, *hi = *lo, *lo = temp;

	*lo ^= ctx->pbox[1];
	*hi ^= ctx->pbox[0];
}

void blowfish_initiate(blowfish_context_t *ctx, void *keyparam, unsigned int keybytes)
{
	unsigned char *key = keyparam;
	int i, j, k;
	unsigned long calc;

	unsigned long hi = 0, lo = 0;

	keybytes %= 57;
	
	for(i = 0; i < 4; i++)
		memcpy(ctx->sbox[i], ORIG_S[i], sizeof(ORIG_S[i]));

	memcpy(ctx->pbox, ORIG_P, sizeof(ORIG_P));

	if(keybytes) {
		for(i = 0, j = 0; i < 18; i++) {
			for(k = 0, calc = 0; k < 4; k++) {
				calc <<= 8, calc |= key[j++];
				if(j == (int) keybytes)
					j = 0;
			}
			ctx->pbox[i] ^= calc;
		}
	}

	for(i = 0; i < 18; i += 2) {
		blowfish_encryptblock(ctx, &hi, &lo);
		ctx->pbox[i] = hi;
		ctx->pbox[i + 1] = lo;
	}

	for(i = 0; i < 4; i++) {
		for(j = 0; j < 256; j += 2) {
			blowfish_encryptblock(ctx, &hi, &lo);
			ctx->sbox[i][j] = hi;
			ctx->sbox[i][j + 1] = lo;
		}
	}
}

void blowfish_clean(blowfish_context_t *ctx)
{
	memset(ctx, 0, sizeof(blowfish_context_t));
}
