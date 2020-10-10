#include "atldef.h"
#include "aescrypt_atl.h"
#include "aescrypt/sha256.h"
#include "aescrypt/AESStringCrypt.h"
#include "aescrypt/password.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define V (void)/* Force result to void */

prim P_sha256()
{ /* dest len input -- */
	Sl(3);
	Hpc(S0);
	Hpc(S2);

	sha256_context ctx;
	// unsigned char sha256sum[32];

	sha256_starts(&ctx);
	sha256_update(&ctx, (uint8 *)S0, S1);
	sha256_finish(&ctx, (unsigned char *)S2);
	Npop(3);
}

prim P_crypt()
{ /* output e/d len input pass -- */
	Sl(5);
	Hpc(S0); // password
	Hpc(S1); // input
	//S2 inputh length
	//S3 : 0 for encrypt, 1 for decrypt
	Hpc(S4); // destination buffer

	char *pass_input = (char *)S0;

	char pass[MAX_PASSWD_LEN * 2 + 2];
	int passlen;
	// char pass_input[MAX_PASSWD_LEN + 1];

	passlen = passwd_to_utf16(pass_input,
							  strlen(pass_input),
							  MAX_PASSWD_LEN + 1,
							  pass);

	if (passlen <= 0)
	{
		printf("Error converting the password to UTF-16LE\n");
		Push = -1;
		return;
	}

	if (S3 == 0)
	{ //Encrypting
		printf("Encrypting...\n");
		long long unsigned int ciphertext_length = AESStringCrypt((unsigned char *)pass,
										   passlen,
										   (unsigned char *)S1,
										   S2,
										   (unsigned char *)S4);

		if (ciphertext_length == AESSTRINGCRYPT_ERROR)
		{
			printf("Error encrypting the string\n");
			return;
		}

		Npop(5);

		Push = ciphertext_length;
		printf("Ciphertext length: %llu\n", ciphertext_length);
	}
	else
	{ //Decrypting

		printf("Decrypting...\n");
		long long unsigned int plaintext_length = AESStringDecrypt((unsigned char *)pass,
											passlen,
											(unsigned char *)S1,
											S2,
											(unsigned char *)S4);

		if (plaintext_length == AESSTRINGCRYPT_ERROR)
		{
			printf("Error decrypting the string\n");
		}

		printf("Decrypted plaintext length: %llu\n", plaintext_length);

		((char*)S4)[plaintext_length] = '\0';
		/*
         * Let's insert a string terminator
         */

		Npop(5);

	}
}

struct primfcn aescrypt_fcns[] = {
	{"0AES.SHA256", P_sha256},
	{"0AES.CRYPT", P_crypt},
	{0, (codeptr)0}};