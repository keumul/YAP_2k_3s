#include "stdafx.h"

namespace IT
{
	IdTable Create(int size)
	{
		if (size <= TI_MAXSIZE)
		{
			IdTable idtable;
			idtable.maxsize = size;
			idtable.current_size = 0;
			idtable.table = new Entry[TI_MAXSIZE];
			return idtable;
		}
		else
		{
			throw ERROR_THROW(128)
		}
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		if (n < idtable.maxsize && n >= 0)
		{
			return idtable.table[n];
		}
		else
		{
			throw ERROR_THROW(123)
		}
	}

	void Delete(IdTable& idtable)
	{
		delete idtable.table;
		idtable.table = nullptr;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (strlen(entry.id) > ID_MAXSIZE_INTEGRAL)
		{
			throw ERROR_THROW(128);
		}

		if (idtable.current_size < idtable.maxsize)
		{
			idtable.table[idtable.current_size] = entry;

			switch (entry.iddatatype)
			{
			case IT::IDDATATYPE::INT:
				idtable.table[idtable.current_size].value.vint = TI_INT_DEFAULT;
				break;

			case IT::IDDATATYPE::STR:
				idtable.table[idtable.current_size].value.vstr->str[0] = TI_STR_DEFAULT;
				idtable.table[idtable.current_size].value.vstr->len = 0;
				break;
			}

			idtable.current_size++;
		}
		else
		{
			throw ERROR_THROW(125);
		}
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE_INTEGRAL])
	{
		for (int i = idtable.current_size - 1; i >= 0; i--)
		{
			if (strcmp(idtable.table[i].id, id) == 0)
			{
				return i;
			}
		}

		return TI_NULLIDX;
	}

	int IsLiteral(IdTable& idtable, char str[TI_STR_MAXSIZE - 1])
	{
		for (int i = idtable.current_size - 1; i >= 0; i--)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::L)
			{
				if (strlen(str) - 2 != idtable.table[i].value.vstr->len)
				{
					continue;
				}

				bool isEqual = true;

				for (size_t j = 0; j < idtable.table[i].value.vstr->len; j++)
				{
					if (str[j + 1] != idtable.table[i].value.vstr->str[j])
					{
						isEqual = false;
						break;
					}
				}

				if (isEqual)
				{
					return i;
				}
			}
		}

		return TI_NULLIDX;
	}

	int IsLiteral(IdTable& idtable, int n)
	{
		for (int i = 0; i < idtable.current_size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::L && idtable.table[i].value.vint == n)
			{
				return i;
			}
		}

		return TI_NULLIDX;
	}

	void PrintIdTable(IdTable& idtable, wchar_t file[])
	{
		std::cout << " №" << "	| " << "ID   " << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Значение " << std::endl;
		for (int i = 0; i < idtable.current_size; i++)
		{
			std::cout << i << "\t";

			switch (idtable.table[i].idtype)
			{
			case IT::IDTYPE::V:
				if (idtable.table[i].iddatatype == 1)
				{
					std::cout << idtable.table[i].parentBlockName << idtable.table[i].id << "\t|\tinteger\t|\tV\t|\t" << idtable.table[i].value.vint << std::endl;
				}
				else
				{
					std::cout << idtable.table[i].parentBlockName << idtable.table[i].id << "\t|\tstring\t|\tV\t|\t[" << (int)idtable.table[i].value.vstr->len << "]" << idtable.table[i].value.vstr->str << std::endl;
				}
				break;

			case IT::IDTYPE::F:
				if (idtable.table[i].iddatatype == 1)
				{
					std::cout << idtable.table[i].id << "\t|\tinteger\t|\tF\t|\t" << std::endl;
				}
				else
				{
					std::cout << idtable.table[i].id << "\t|\tstring\t|\tF\t|\t" << std::endl;
				}
				break;

			case IT::IDTYPE::P:
				if (idtable.table[i].iddatatype == 1)
				{
					std::cout << idtable.table[i].parentBlockName << idtable.table[i].id << "\t|\tinteger\t|\tP\t|\t" << idtable.table[i].value.vint << std::endl;
				}
				else
				{
					std::cout << idtable.table[i].parentBlockName << idtable.table[i].id << "\t|\tstring\t|\tP\t|\t[" << (int)idtable.table[i].value.vstr->len << "]" << idtable.table[i].value.vstr->str << std::endl;
				}
				break;

			case IT::IDTYPE::L:
				if (idtable.table[i].iddatatype == INT)
				{
					std::cout << "L" << idtable.table[i].idxfirstLE << "\t|\tinteger\t|\t\t|\t" << idtable.table[i].value.vint << std::endl;
				}
				else
				{
					std::cout << "L" << idtable.table[i].idxfirstLE << "\t|\tstring\t|\t\t|\t[" << (int)idtable.table[i].value.vstr->len << "]" << idtable.table[i].value.vstr->str << std::endl;
				}
				break;

			case IT::IDTYPE::O:
				std::cout << idtable.table[i].id << std::endl;
				break;

			default:
				break;
			}
		}
	}
}