#pragma once

#define ID_MAXSIZE			5				// максимальное количество символов в идентификаторе
#define ID_MAXSIZE_INTEGRAL	20				// максимальное количество символов дл€ встроенных идентификаторов
#define TI_MAXSIZE			4096			// максимальное количество эл-ов в таблице идентификаторов 
#define TI_INT_DEFAULT		0x00000000		// значение по умолчанию дл€ типа integer 
#define TI_STR_DEFAULT		0x00			// значение по умолчанию дл€ типа string 
#define TI_NULLIDX			0xffffffff		// нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE		255
#define IND_MAIN			"main"
#define NEW_TABLE			"---------------------------------------------------------------------------------"

namespace IT
{
	enum IDDATATYPE { INT = 1, STR = 2 };					// типы данных идентификаторов: integer, string
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, O = 5 };		// типы идентификаторов: переменна€, функци€, параметр, литерал, операци€

	struct Entry		// строка таблицы идентификаторов
	{
		int	idxfirstLE;									// индекс первой строки в таблице лексем
		char id[ID_MAXSIZE_INTEGRAL];					// идентификатор (автоматически усекаетс€ до ID_MAXSIZE)
		char parentBlockName[ID_MAXSIZE_INTEGRAL];		// им€ родительского блока
		IDDATATYPE iddatatype;							// тип данных
		IDTYPE idtype;									// тип идентификатора

		union
		{
			int vint;							// значение integer
			struct
			{
				char len;						// количесво символов в string
				char str[TI_STR_MAXSIZE - 1];	// символы string
			} vstr[TI_STR_MAXSIZE];				// значение string
		} value;								// значение идентификатора
	};

	struct IdTable // экземпл€р таблицы идентификаторов
	{
		int maxsize;										// емкость таблицы идентификаторов < TI_MAXSIZE
		int current_size;									// текущий размер таблицы идентификаторов < maxsize
		int current_literals = 0;							// текущее кол-во литералов
		Entry* table;										// массив строк таблицы идентификаторов
	};

	IdTable Create(int size);								// создать таблицу ид
	void Add(IdTable& idtable, Entry entry);				// добавить строку в таблицу ид
	Entry GetEntry(IdTable& idtable, int n);				// получить строку таблицы ид
	int IsId(IdTable& idtable, char id[ID_MAXSIZE]);		// возврат: номер строки( если есть), TI_NULLIDX(если нет)
	void Delete(IdTable& idtable);							// удалить таблицу лексем
	void PrintIdTable(IdTable& idtable, wchar_t file[]);
	int IsLiteral(IdTable& idtable, char str[TI_STR_MAXSIZE - 1]);
	int IsLiteral(IdTable& idtable, int n);
};