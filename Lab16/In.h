#pragma once
#define IN_MAX_LEN_TEXT 1024 * 1024	// максимальный размер исходного кода = 1MB
#define IN_CODE_ENDL '\n'	// символ конца строки
#define IN_SEPARATOR '|'	// символ сепаратора
// таблица проверки входной информации, индекс = код (Windows-1251) символа
// значения IN::F - завершенный символ, IN::Т - разрешенный символ, IN::I -игнорировать (не вводить), IN::L - лексический символ //	если 0 <= значение < 256 - то вводится данное значение
#define IN_CODE_TABLE {\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,		IN::F, IN::F, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,		IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::S, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::L,		IN::L, IN::L, IN::L, IN::L, IN::L, IN::L, IN::F, IN::L, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,		IN::T, IN::T, IN::T, IN::L, IN::F, IN::L, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,		IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,		IN::F, IN::F, IN::F, IN::L, IN::F, IN::L, IN::F, IN::F, \
	IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,		IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,		IN::T, IN::T, IN::T, IN::L, IN::L, IN::L, IN::F, IN::F, \
																														\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,		IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,		IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,		IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,		IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,		IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,		IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,		IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,		IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T  \
}

namespace In
{
	struct IN							// исходные данные
	{
		enum { T = 1024, F = 2048, I = 4096, L = 8192, S = 16384 };	//  T - допустимый символ, F - недопустимый, I - игнорировать, L - лексический символ, S - пробел(табуляция), иначе заменить
		int size = 0;										//	размер исходного кода
		int lines = 0;										//	количество строк
		int ignor = 0;										//	количество проигнорированных символов
		unsigned char* text;								//	исходный код (Windoius -	1251)
		int code[256] = IN_CODE_TABLE;						//	таблица проверки: T, F,	I, L новое	значение
	};
	IN getin(wchar_t infile[], LT::LexTable& lexTable, IT::IdTable& idTable);	// ввести и проверить входной поток
}