#pragma once
#include "Error.h"
#include "Parm.h"
#define LEXEMA_FIXSIZE 1			// фиксированный размер лексемы
#define LT_MAXSIZE 4096				// максимальное количество строк в таблице лексем
#define LT_TI_NULLIDX 0xfffffff		// нет элемента таблицы идентификаторов
#define LEX_INTEGER 't'				// лексема для integer
#define LEX_STRING 't'				// лексема для string
#define LEX_ID 'i'					// лексема для идентификатора
#define LEX_LITERAL 'l'				// лексема для литерала
#define LEX_FUNCTION 'f'			// лексема для function
#define LEX_DECLARE 'd'				// лексема для declare
#define LEX_RETURN 'r'				// лексема для return
#define LEX_PRINT 'p'				// лексема для print
#define LEX_MAIN 'm'				// лексема для main
#define LEX_SEMICOLON ';'			// лексема для ;
#define LEX_COMMA ','				// лексема для ,
#define LEX_LEFTBRACE '{'			// лексема для {
#define LEX_BRACELET '}'			// лексема для }
#define LEX_LEFTTHESIS '('			// лексема для (
#define LEX_RIGHTTHESIS ')'			// лексема для )
#define LEX_PLUS 'v'				// лексема для +
#define LEX_MINUS 'v'				// лексема для -
#define LEX_STAR 'v'				// лексема для *
#define LEX_DIRSLASH 'v'			// лексема для /
#define LEX_OPERATOR 'v'			// лексема для операторов
#define LEX_EQUAL '='

namespace LT {
	struct Entry {
		char lexema;	//лексема
		int sn;			//номер строки в исходном тексте
		int idxTI;
		int priority;	//индекс в ТИ
	};
	struct LexTable
	{
		int maxsize;
		int size;
		Entry* table;
	};

	LexTable Create(int size);
	void Add(LexTable& lextable, Entry entry);
	Entry GetEntry(LexTable& lextable, int n);
	void Delete(LexTable& lextable);
	Entry writeEntry(Entry& entry, char lexema, int indx, int line);
	void showTable(LexTable lextable, Parm::PARM parm);
}
