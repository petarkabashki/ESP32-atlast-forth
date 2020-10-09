#include "atldef.h"
#include "bip39.h"
#include <string.h>
#include "crypto_atl.h"
#include <stdio.h>

#include "common.h"

prim P_mnemonic_generate()
{ /* strength dest -- */
	Sl(2);
	Hpc(S1);
	char * res = mnemonic_generate(S0);
	printf("Mnemonic generated: %s", res);
	V strcpy((char *) S1, res);
	Pop2;
}

struct primfcn crypto_fcns[] = {	
	{"0MNE.GEN",	P_mnemonic_generate},
	{NULL, (codeptr) 0}
};
