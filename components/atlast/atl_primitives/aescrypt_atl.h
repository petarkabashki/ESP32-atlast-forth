#ifndef aescrypt_atl_h
#define aescrypt_atl_h

#include "atldef.h"

extern struct primfcn aescrypt_fcns[];

#endif

/*

variable s
10 allot
"tralalabi" s s!
variable sum 
32 allot

sum 9 s aes.sha256

///

variable pass
10 allot    
"Password" pass s!
pass type
Passwordpass strlen
.
8 

variable text 
200 allot
"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod" text s!
text type
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod

variable cif
500 allot

text strlen .
71 

cif 0 71 text pass aes.crypt

Error in iconv_open: Success
Error converting the password to UTF-16LE


*/