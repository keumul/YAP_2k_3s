#pragma once
#define IN_MAX_LEN_TEXT 1024 * 1024	
#define IN_CODE_ENDL '\n'	
#define IN_SEPARATOR '|'	
// таблица проверки входной информации, индекс = код (Windows-1251) символа значение
#define IN_CODE_TABLE {\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::S, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::L,	IN::L, IN::L, IN::L, IN::L, IN::L, IN::L, IN::F, IN::L, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::L, IN::F, IN::L, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::F, IN::L, IN::F, IN::L, IN::F, IN::F, \
	IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::L, IN::L, IN::L, IN::F, IN::F, \
																													\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,	IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T  \
}

namespace In
{
	struct IN							
	{
		enum { T = 1024, F = 2048, I = 4096, L = 8192, S = 16384 };	//  T - допустимый символ, F - недопустимый, I - игнорировать, L - лексический символ, S - пробел(табул€ци€), иначе заменить
		int size = 0;												//	размер исходного кода
		int lines = 0;												//	количество строк
		int ignor = 0;												//	количество проигнорированных символов
		unsigned char* text;										//	исходный код (Windoius -	1251)
		int code[256] = IN_CODE_TABLE;								//	таблица проверки: T, F,	I, L новое	значение
	};
	IN getin(wchar_t infile[], LT::LexTable& lexTable, IT::IdTable& idTable);	// ввести и проверить входной поток
}