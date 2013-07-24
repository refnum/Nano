// Written by Andrew Carter (2008)

#ifndef BLOWFISH_H_
#define BLOWFISH_H_

#ifdef __cplusplus
extern "C" {
#endif



typedef struct blowfish_context_t_ {
	uint32_t pbox[256];
	uint32_t sbox[4][256];
} blowfish_context_t;

void blowfish_encryptblock (blowfish_context_t *ctx, uint32_t  *hi,  uint32_t *lo);
void blowfish_decryptblock (blowfish_context_t *ctx, uint32_t  *hi,  uint32_t *lo);
void blowfish_initiate     (blowfish_context_t *ctx, void *keyparam,      uint32_t  keybytes);
void blowfish_clean        (blowfish_context_t *ctx);

extern const uint32_t ORIG_P[18];
extern const uint32_t ORIG_S[4][256];



#ifdef __cplusplus
}
#endif

#endif

