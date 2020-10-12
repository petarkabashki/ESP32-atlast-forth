#include "atldef.h"
#include "bip39.h"
#include <string.h>
#include "crypto_atl.h"
#include <stdio.h>
#include <stdlib.h>
#include "bip32.h"

#define V	(void)		      /* Force result to void */

prim P_mnemonic_generate()
{ /* strength dest -- */
	Sl(2);
	Hpc(S1);
	const char * res = mnemonic_generate(S0);
	V strcpy((char *) S1, res);
	Pop2;
}

prim P_mnemonic_from_data()
{ /* dest data len -- */
	Sl(3);
	Hpc(S1);
	const char * res = mnemonic_from_data((uint8_t*)S1, S0);
	V strcpy((char *) S2, res);
	Npop(3);
	// Push = res;
}

prim P_mnemonic_check()
{ /* mne -- res */
	Sl(1);
	Hpc(S0);
	int res = mnemonic_check((char *)S0);
	S0 = res;
	// Push = res;
}

prim P_mnemonic_to_seed()
{ /* dest pass mnemonic -- */
	Sl(3);
	Hpc(S0);
	Hpc(S1);
	Hpc(S2);
	V mnemonic_to_seed(/*const char *mnemonic*/(char*)S0, 
		/*const char *passphrase*/(char*)S1,
                      /*uint8_t seed[512 / 8]*/(uint8_t*)S2,
                      NULL);
	Npop(3);
}

prim P_mnemonic_get_word()
{ /* dest index -- */
	Sl(2);
	Hpc(S1);
	const char * res = mnemonic_get_word(S0);
	V strcpy((char *) S1, res);
	Pop2;
}

prim P_hdnode_from_xprv()
{ /* node curve private_key chain_code child_num dept -- res */
	Sl(6);
	Hpc(S2);Hpc(S3);Hpc(S4);Hpc(S5);
	int res = hdnode_from_xprv(/*uint32_t depth*/S0, 
		/*uint32_t child_num*/S1,
		/*const uint8_t *chain_code*/(uint8_t *)S2, 
		/*const uint8_t *private_key*/(uint8_t *)S3,
		/*const char *curve*/(char *)S4,
		/*HDNode *out*/(HDNode *)S5);
	
	Npop(6);
	Push = res;
}


prim P_hdnode_from_seed()
{ /* node curve seed_len seed -- res */
	Sl(4);
	Hpc(S0);Hpc(S2);Hpc(S3);
	int res = hdnode_from_seed(/*const uint8_t *seed*/(uint8_t *)S0, 
	/*int seed_len*/S1, 
	/*const char *curve*/(char *)S2,
	/*HDNode *out*/(HDNode *)S3);
	
	Npop(4);
	Push = res;
}

prim P_hdnode_fingerprint()
{ /* node -- res */
	Sl(1);
	Hpc(S0);
	int res = hdnode_fingerprint(/*HDNode *out*/(HDNode *)S0);
	
	Pop;
	Push = res;
}

prim P_hdnode_private_ckd()
{ /* i node -- res */
	Sl(2);
	Hpc(S0);
	int res = hdnode_private_ckd(/*HDNode *inout*/(HDNode *)S0, S1);
	
	Pop2;
	Push = res;
}

prim P_hdnode_public_ckd()
{ /* i node-- res */
	Sl(2);
	Hpc(S0);
	int res = hdnode_public_ckd(/*HDNode *inout*/(HDNode *)S0, S1);
	
	Pop2;
	Push = res;
}

prim P_hdnode_fill_public_key()
{ /* i node-- res */
	Sl(1);
	Hpc(S0);
	V hdnode_fill_public_key(/*HDNode *inout*/(HDNode *)S0);
	
	Pop;
}

prim P_hdnode_get_address()
{ /* node version addr addrsize -- */
	Sl(4);
	Hpc(S1);Hpc(S0);
	V hdnode_get_address(/*HDNode *node*/(HDNode *)S3, 
	/*uint32_t version*/S2, 
	/*char *addr*/(char*)S1,
	/*int addrsize*/S0);
	
	Npop(4);
}


prim P_hdnode_sign()
{ /* node msg msg_len hsher_sign sig pby -- res*/
	Sl(7);
	Hpc(S0);Hpc(S1);Hpc(S5);Hpc(S6);
	int res = hdnode_sign(/*HDNode *node*/(HDNode *)S6, 
		/*const uint8_t *msg*/(const uint8_t *)S5, 
		/*uint32_t msg_len*/S4,
		/*HasherType hasher_sign*/(HasherType)S2, 
		/*uint8_t *sig*/(uint8_t *)S1, 
		/*uint8_t *pby*/(uint8_t *)S0,
		/*int (*is_canonical)(uint8_t by, uint8_t sig[64])*/NULL);
	
	Npop(7);
	Push = res;
}

struct primfcn crypto_fcns[] = {	
	{"0MNE.GEN",	P_mnemonic_generate},
	{"0MNE<DATA",	P_mnemonic_from_data},
	{"0MNE.CHECK",	P_mnemonic_check},
	{"0MNE>SEED",	P_mnemonic_to_seed},
	{"0MNE.GWD",	P_mnemonic_get_word},
	
	{"0HND<XPRV",	P_hdnode_from_xprv},
	{"0HND<SEED",	P_hdnode_from_seed},
	{"0HND.FGPR",	P_hdnode_fingerprint},
	{"0HND.PRCKD",	P_hdnode_private_ckd},
	{"0HND.PBCKD",	P_hdnode_public_ckd},
	{"0HND.FILL-PBK",	P_hdnode_fill_public_key},
	{"0HND.ADDR",	P_hdnode_get_address},
	{"0HND.SIGN",	P_hdnode_sign},

	{NULL, (codeptr) 0}
};