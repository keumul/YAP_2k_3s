#include "stdafx.h"
#include "Lex.h"
#include "Sep.h"
#include "FST.h"
#define DIV '|'
#define SPACE ' '
#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define DIRSLASH '/'
#define EQUAL '='

using namespace std;

namespace Lex
{
	LEX LexichAnaliz(Log::LOG log, In::IN in)
	{
		LEX lex; //объект структуры lex
				 //создаем таблицы
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);

		//динамически выделяем память под массив слов
		char** word = new char* [max_world];

		for (int i = 0; i < max_world; i++)
		{
			word[i] = new char[size_world] {NULL};
		}

		do { word = sepText(in.text, in.size); } 
		while (word == NULL);


		int LexInd = 0;		// индекс лексемы
		int IDInd = 0;		// индекс идентификатора
		int lcount = 1;		// счетчик литералов
		int line = 1;		// номер строки
		int position = 0;	// номер позиции в исходном файле

		bool findMain = false;
		int countMain = 0;
		bool findDec = false;
		bool findType = false;

		char empstr[] = "";						// пустая строка
		char* prefix = new char[10]{ "" };		// текущий префикс
		char* bufprefix = new char[10]{ "" };	// буфер для префикса
		char* backprefix = new char[10]{ "" };	// предыдущий префикс
		char* L = new char[2]{ "L" };
		char* bufL = new char[TI_STR_MAXSIZE];
		char* nameLiteral = new char[10]{ "" };
		char* chcount = new char[10]{ "" };	// для строковой записи счетчика

		bool findFun = false;
		bool findParm = false;

		IT::Entry entryIT;

		for (int i = 0; word[i] != NULL; i++, LexInd++)
		{

			bool findSameID = false; //если нашли такой же id

			//если распознано слово 
			FST::FST fstDeclare(word[i], FST_DECLARE);
			if (FST::execute(fstDeclare))
			{
				//записываем соответствующие данные (само слово, лексема, нулевое значение (ессли нет id, то остается просто 0), строку)
				LT::Entry entryLT = writeEntry(entryLT, LEX_DECLARE, LT_TI_NULLIDX, line);
				//добавляется в таблицу
				LT::Add(lextable, entryLT);
				//флаг о том, что слово найдено
				findDec = true;
				continue;
			}
			FST::FST fstTypeInteger(word[i], FST_INTEGER);
			if (FST::execute(fstTypeInteger))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_INTEGER, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findType = true;
				entryIT.iddatatype = IT::INT;
				continue;
			}
			FST::FST fstTypeString(word[i], FST_STRING);
			if (FST::execute(fstTypeString))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_STRING, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findType = true;
				entryIT.iddatatype = IT::STR;
				strcpy(entryIT.value.vstr.str, empstr);
				continue;
			}
			FST::FST fstFunction(word[i], FST_FUNCTION);
			if (FST::execute(fstFunction))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_FUNCTION, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::F;
				findFun = true;
				continue;
			}
			FST::FST fstReturn(word[i], FST_RETURN);
			if (FST::execute(fstReturn))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RETURN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstPrint(word[i], FST_PRINT);
			if (FST::execute(fstPrint))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_PRINT, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstMain(word[i], FST_MAIN);
			if (FST::execute(fstMain))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_MAIN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				strcpy(backprefix, prefix);
				strcpy(prefix, word[i]);
				strcpy(entryIT.visibility, empstr);
				findMain = true;
				countMain++;
				continue;
			}
			FST::FST fstIdentif(word[i], FST_ID);
			if (FST::execute(fstIdentif))
			{
				// поиск такого же
				if (findFun) // если функция
				{
					int idx = IT::IsId(idtable, word[i]); // ищем без префикса
					if (idx != TI_NULLIDX) // если такой идентификатор уже есть
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFun = false;
						continue;
					}
				}
				else
				{
					int idx = IT::IsId(idtable, word[i]); // ищем без префикса, а потом с префиксом
					if (idx == TI_NULLIDX && findDec == true && findType == false)
						throw ERROR_THROW_IN(123, line, position);
					if (idx != TI_NULLIDX) // если такой идентификатор уже есть
					{
						if (findDec == true) //Проверка на повторное объявление
							throw ERROR_THROW_IN(122, line, position);
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFun = false;
						continue;
					}
					strcpy(bufprefix, prefix);
					word[i] = strcat(bufprefix, word[i]);
					idx = IT::IsId(idtable, word[i]);
					if (idx != TI_NULLIDX) // если такой идентификатор уже есть
					{
						if (findDec == true) //Проверка на повторное объявление
							throw ERROR_THROW_IN(122, line, position);
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						continue;
					}
				}
				LT::Entry entryLT = writeEntry(entryLT, LEX_ID, IDInd++, line);
				LT::Add(lextable, entryLT);
				findDec = false;
				if (findParm) { // если параметр
					entryIT.idtype = IT::P;
					strcpy(entryIT.visibility, prefix);
				}
				else if (!findFun) { // если переменная
					entryIT.idtype = IT::V;
					strcpy(entryIT.visibility, prefix);
					if (entryIT.iddatatype == IT::INT)
						entryIT.value.vint = TI_INT_DEFAULT;
					if (entryIT.iddatatype == IT::STR) {
						entryIT.value.vstr.len = 0;
						memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
					}
				}
				else { // если функция
					strcpy(backprefix, prefix);
					strcpy(prefix, word[i]);
					strcpy(entryIT.visibility, empstr);
				}

				entryIT.idxfirstLE = LexInd;
				strcpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				findFun = false;
				findType = false;
				continue;
			}
			FST::FST fstLiteralInt(word[i], FST_INTLIT);
			if (FST::execute(fstLiteralInt))
			{
				int value = atoi((char*)word[i]); // значение литерала
				// поиск такого же
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L)
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;
						break;
					}
				}
				if (findSameID) // если был найден такой же
					continue;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, IDInd++, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = value;
				entryIT.idxfirstLE = LexInd;
				// формирование имени литерала
				_itoa_s(lcount++, chcount, sizeof(char) * 10, 10); // преобразуем значение счетчика в строку(charclit)
				strcpy(bufL, L); // помещаем в буфер "L"
				word[i] = strcat(bufL, (char*)chcount); // формируем имя для литерала
				strcpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstLiteralString(word[i], FST_STRLIT);
			if (FST::execute(fstLiteralString))
			{
				// запись значения
				int length = strlen(word[i]);
				for (int k = 0; k < length; k++) // перезапись массива, убираем кавычки
					word[i][k] = word[i][k + 1];
				word[i][length - 2] = 0;
				// поиск такого же
				for (int k = 0; k < idtable.size; k++)
				{
					if (!(strcmp(idtable.table[k].value.vstr.str, word[i])))
					{
						findSameID = true;
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID) // если был найден такой же
					continue;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, IDInd++, line);
				LT::Add(lextable, entryLT);
				strcpy(entryIT.value.vstr.str, word[i]); // запись значиния строкового литерала
				entryIT.value.vstr.len = length - 2; // запись длины строкового литерала
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::STR;
				entryIT.idxfirstLE = LexInd;
				// формирование имени литерала
				_itoa_s(lcount++, chcount, sizeof(char) * 10, 10); // преобразуем значение счетчика в строку(charclit)
				strcpy(bufL, L); // помещаем в буфер "L"
				nameLiteral = strcat(bufL, (char*)chcount); // формируем имя для литерала (L + charclit)
				strcpy(entryIT.id, nameLiteral);
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstOperator(word[i], FST_OPERATOR);
			if (FST::execute(fstOperator))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_OPERATOR, IDInd++, line);
				switch (word[i][0])
				{
				case PLUS: case MINUS:
					entryLT.priority = 2;
					break;
				case DIRSLASH: case STAR:
					entryLT.priority = 3;
					break;
				}
				LT::Add(lextable, entryLT);
				strcpy(entryIT.id, word[i]);
				entryIT.idxfirstLE = LexInd;
				entryIT.idtype = IT::OP;
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstSemicolon(word[i], FST_SEMICOLON);
			if (FST::execute(fstSemicolon))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_SEMICOLON, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstComma(word[i], FST_COMMA);
			if (FST::execute(fstComma))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line);
				entryLT.priority = 1;
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstLeftBrace(word[i], FST_LEFTBRACE);
			if (FST::execute(fstLeftBrace))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstRightBrace(word[i], FST_BRACELET);
			if (FST::execute(fstRightBrace))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstLeftThesis(word[i], FST_LEFTTHESIS);
			if (FST::execute(fstLeftThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				LT::Add(lextable, entryLT);
				if (idtable.table[IDInd - 1].idtype == IT::F)
					findParm = true;
				continue;
			}
			FST::FST fstRightThesis(word[i], FST_RIGHTTHESIS);
			if (FST::execute(fstRightThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				if (findParm && word[i + 1][0] != LEX_LEFTBRACE && word[i + 2][0] != LEX_LEFTBRACE) // если после функции нет {
					strcpy(prefix, backprefix); // возвращаем предыдущую обл. видимости
				findParm = false;
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstEqual(word[i], FST_EQUAL);
			if (FST::execute(fstEqual))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_EQUAL, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			position += strlen(word[i]);
			if (word[i][0] == DIV) {
				line++;
				position = 0;
				LexInd--;
				continue;
			}
			throw ERROR_THROW_IN(108, line, position);
		}
		if (!findMain) throw ERROR_THROW(2);
		if (countMain > 1) throw ERROR_THROW(3);

		lex.idtable = idtable;
		lex.lextable = lextable;
		return lex;
	}
}