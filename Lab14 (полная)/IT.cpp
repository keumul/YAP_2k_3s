#include "stdafx.h"
#include "IT.h"
#include "Error.h"
#include <iomanip>

namespace IT
{
	IdTable Create(int size) // создать таблицу идентификаторов
	{
		IdTable* tabl = new IdTable; //выделяем память для таблицы
		if (size > TI_MAXSIZE) { throw ERROR_THROW(160); }

		tabl->maxsize = size;
		tabl->size = 0;
		tabl->table = new Entry[size];
		return*tabl;
	}

	void Add(IdTable& idtable, Entry entry) // добавить строку в таблицу идентификаторов
	{
		if (idtable.size + 1 > idtable.maxsize) { throw ERROR_THROW(161); }

		idtable.table[idtable.size] = entry; // строка таблицы идентификаторов
		idtable.size += 1; 
	}

	Entry GetEntry(IdTable& idtable, int n) { return idtable.table[n]; } // получить строку таблицы идентификаторов

	int IsId(IdTable& idtable, char id[ID_MAXSIZE]) // возврат: номер строки(если есть), TI_NULLIDX(если нет)
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == 0) { return i; }
			//Нулевое значение говорит о том, что обе строки равны.
			//Значение больше нуля указывает на то, что строка string1 больше строки string2, значение меньше нуля свидетельствует об обратном.
		}
		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable) // удалить таблицу лексем (освободить память)
	{
		delete[] idtable.table;
	}

	void showTable(IdTable& idtable)
	{
		int i, number = 0;
		std::cout << " №" << "	| " << "Идентификатор" << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Индекс (ТЛ)" << " | " << "Значение " << std::endl;
		for (i = 0; i < idtable.size; i++)
		{
			std::cout << std::setfill('0') << std::setw(4) << std::right << i << " | "; //выводится номер
			std::cout << std::setfill(' ') << std::setw(13) << std::left << idtable.table[i].id << " | "; //идентификатор
			switch (idtable.table[i].iddatatype)
			{
			case INT: std::cout << std::setw(10) << std::left; //если INT
				if (idtable.table[i].idtype == OP) std::cout << "-" << " | ";
				else std::cout << "integer" << " | "; break;
			case STR: std::cout << std::setw(10) << std::left; //если STRING
				if (idtable.table[i].idtype == OP) std::cout << "-" << " | ";
				else std::cout << "string" << " | "; break;
			default: std::cout << std::setw(10) << std::left << "?" << " | "; break; //если варианты не подходят
			}
			switch (idtable.table[i].idtype)
			{//меняется в зависимости от типа идентификаторов
			case V: std::cout << std::setw(18) << std::left << "переменная" << " | "; break;
			case F: std::cout << std::setw(18) << std::left << "функция" << " | "; break;
			case P: std::cout << std::setw(18) << std::left << "параметр" << " | "; break;
			case L: std::cout << std::setw(18) << std::left << "литерал" << " | "; break;
			case OP: std::cout << std::setw(18) << std::left << "оператор" << " | ";
				number++;
				break;
			default: std::cout << std::setw(18) << std::left << "?" << " | "; break; //если варианты не подходят
			}
			std::cout << std::setw(11) << std::left << idtable.table[i].idxfirstLE << " | "; //индексы в таблице лексем 

			//если значение равно int и является или переменной, или литералом, то выводим 
			if (idtable.table[i].iddatatype == INT && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				std::cout << std::setw(18) << std::left << idtable.table[i].value.vint;

			//если значение равно string и является или переменной, или литералом, то выводим 
			else if (idtable.table[i].iddatatype == STR && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				std::cout << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else
				std::cout << "-";
			std::cout << std::endl;
		}
		std::cout << "Количество идентификаторов: " << i - number << std::endl;
	}
};