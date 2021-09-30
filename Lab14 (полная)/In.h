#pragma once
#include "Parm.h"
#define IN_MAX_LEN_TEXT 1024*1024	
#define IN_CODE_ENDL '\n'
#define IN_CODE_ENDS '\0'
#define IN_CODE_TAB '	'
#define IN_CODE_SPACE ' '
#define IN_CODE_SEP '|'
//===================================================================================================================\\
//		  0		 1		2	   3	  4		 5      6	   7	  8	     9	   10	  11	  12	 13	    14     15
#define IN_CODE_TABLE {\
/*0*/	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::P, IN::S, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*1*/	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,	\
/*2*/	IN::P, IN::F, IN::K, IN::F, IN::F, IN::F, IN::F, IN::K, IN::L, IN::L, IN::L, IN::L, IN::L, IN::L, IN::F, IN::L, \
/*3*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::L, IN::F, IN::L, IN::F, IN::F, \
/*4*/	IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*5*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*6*/	IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*7*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::L, IN::F, IN::L, IN::F, IN::F, \
																														\
/*8*/	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*9*/	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,	\
/*10*/	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*11*/	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*12*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*13*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*14*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*15*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
}
//===================================================================================================================\\
//		  0		 1		2	   3	  4		 5      6	   7	  8	     9	   10	  11	  12	 13	    14     15
namespace In
{
	struct IN
	{
		//символ подачи строки (LF) (0x0A, \n) перемещает курсор к следующей строке, не возвращаясь к началу линии
		// S - возврат строки [LF], P - пробел, L - [()*+,-./;=], K - ковычки [']
		// T - допустимый символ,   F - недопустимый символ,      I - игнорировать
 		enum { T = 1024, F = 2048, I = 4096, S = 3, P = 4, L = 5, K = 6 };
		int size;
		int lines;
		int ignor;
		char* text;
		int code[256] = IN_CODE_TABLE;
	};
	IN getin(Parm::PARM parm);
}
