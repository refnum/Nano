// Written by Andrew Carter (2008)

#ifndef BLOWFISH_H_
#define BLOWFISH_H_

#ifdef __cplusplus
extern "C" {
#endif



typedef struct blowfish_context_t_ {
	unsigned long pbox[256];
	unsigned long sbox[4][256];
} blowfish_context_t;

void blowfish_encryptblock (blowfish_context_t *ctx, unsigned long  *hi,  unsigned long *lo);
void blowfish_decryptblock (blowfish_context_t *ctx, unsigned long  *hi,  unsigned long *lo);
void blowfish_initiate     (blowfish_context_t *ctx, void *keyparam,      unsigned int  keybytes);
void blowfish_clean        (blowfish_context_t *ctx);

extern const unsigned long ORIG_P[18];
extern const unsigned long ORIG_S[4][256];



#ifdef __cplusplus
}
#endif

#endif

