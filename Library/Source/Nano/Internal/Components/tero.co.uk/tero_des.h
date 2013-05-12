/*	NAME:
		tero_des.h

	DESCRIPTION:
		DES/DES3 encryption.

	COPYRIGHT:
		Copyright (c) 1995 Eric Young
	__________________________________________________________________________
*/
//============================================================================
//		Function prototypes
//----------------------------------------------------------------------------
void DES_Encrypt(size_t keySize, const void *keyPtr, size_t dataSize, void *dataPtr);
void DES_Decrypt(size_t keySize, const void *keyPtr, size_t dataSize, void *dataPtr);

void DES3_Encrypt(size_t keySize, const void *keyPtr, size_t dataSize, void *dataPtr);
void DES3_Decrypt(size_t keySize, const void *keyPtr, size_t dataSize, void *dataPtr);
