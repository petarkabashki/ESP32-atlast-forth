#include "atldef.h"
#include "bip39.h"
#include <string.h>
#include "crypto_atl.h"
#include <stdio.h>

#define V	(void)		      /* Force result to void */

prim P_mnemonic_generate()
{ /* strength dest -- */
	Sl(1);
	Hpc(S1);
	char * res = mnemonic_generate(S0);
	// fprintf(stdout, "Generating mnemonic");
	// fprintf(stdout, "Generating mnemonic:\n %s", res);
	V strcpy((char *) S1, res);
	Pop;
}

struct primfcn crypto_fcns[] = {	
	{"0??MNE",	P_mnemonic_generate},
	{NULL, (codeptr) 0}
};