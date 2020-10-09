#include "atldef.h"
#include <string.h>
#include "twofish2_atl.h"
#include <stdlib.h>
// #include <esp_system/esp_system.h>
#include "twofish2/aes.h"
#include "twofish2/table.h"

#include "common.h"

// /* return 0 iff test passes */
// keyInstance ki;		 /* key information, including tables */
// cipherInstance ci; /* keeps mode (ECB, CBC) and IV */
// BYTE plainText[MAX_BLK_CNT * (BLOCK_SIZE / 8)];
// BYTE cipherText[MAX_BLK_CNT * (BLOCK_SIZE / 8)];
// BYTE decryptOut[MAX_BLK_CNT * (BLOCK_SIZE / 8)];
// BYTE iv[BLOCK_SIZE / 8];
// int i, byteCnt;

// int rand() {
// 	int i = millis();
// 	return i ^ (3 + (i * 1093));
// }

#define TEXT_SIZE (MAX_BLK_CNT * (BLOCK_SIZE / 8))

#define MAX_BLK_CNT 4									 /* max # blocks per call in TestTwofish */

prim P_twofish2_crypt(/*int mode, int keySize, char* in, char* cf*/) /* keySize must be 128, 192, or 256 */
{																			 /* return 0 iff test passes */
	Sl(5);
	Hpc(S0);
	Hpc(S4);

	char* in = (char*)S4;
	int dir_encrypt = S3;
	int mode = S2;
	int keySize = S1;
	char* keyMaterial = (char*)S0;

	twofish2_crypt(mode, keySize, in, keyMaterial, dir_encrypt);

	Npop(5);
}


struct primfcn twofish2_fcns[] = {
		{"0FISH2.crypt", P_twofish2_crypt},
		{NULL, (codeptr)0}};
