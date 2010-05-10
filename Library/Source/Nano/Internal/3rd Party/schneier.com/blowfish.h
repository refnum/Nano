// Written by Andrew Carter (2008)

#ifndef BLOWFISH_H_
#define BLOWFISH_H_

#ifdef __cplusplus
extern "C" {
#endif



typedef struct blowfish_context_t_ {
	UInt32 pbox[256];
	UInt32 sbox[4][256];
} blowfish_context_t;

void blowfish_encryptblock (blowfish_context_t *ctx, UInt32  *hi,  UInt32 *lo);
void blowfish_decryptblock (blowfish_context_t *ctx, UInt32  *hi,  UInt32 *lo);
void blowfish_initiate     (blowfish_context_t *ctx, void *keyparam,      UInt32  keybytes);
void blowfish_clean        (blowfish_context_t *ctx);

extern const UInt32 ORIG_P[18];
extern const UInt32 ORIG_S[4][256];



#ifdef __cplusplus
}
#endif

#endif

