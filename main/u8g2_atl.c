#include "atldef.h"
#include <string.h>
#include "u8g2_atl.h"
#include <u8g2.h>
// #include <stdio.h>

#define V	(void)		      /* Force result to void */

extern u8g2_t u8g2;

prim P_u8g2_SendBuffer()
{ 
	u8g2_SendBuffer(&u8g2);
}

prim P_u8g2_ClearBuffer()
{ 
	u8g2_ClearBuffer(&u8g2);
}

prim P_u8g2_DrawStr()
{ // xpos ypos test --
	Sl(3);
	Hpc(S0);
	
	u8g2_DrawStr(&u8g2, S2, S1, (char*)S0);

	Pop2;
	Pop;
}


struct primfcn u8g2_fcns[] = {	
	{"0OLED.SEND",	P_u8g2_SendBuffer},
	{"0OLED.CLEAR",	P_u8g2_ClearBuffer},
	{"0OLED.DRAWSTR",	P_u8g2_DrawStr},
	{NULL, (codeptr) 0}
};