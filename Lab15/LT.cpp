#include "stdafx.h"

namespace LT
{
	LexTable Create(int size)
	{
		LT::LexTable lextable;
		lextable.maxsize = size;
		lextable.current_size = 0;
		lextable.table = new LT::Entry[size];
		return lextable;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.current_size < lextable.maxsize)
		{
			lextable.table[lextable.current_size] = entry;
			lextable.current_size++;
		}
		else
		{
			throw ERROR_THROW(120);
		}
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		if (n < lextable.maxsize && n >= 0)
		{
			return lextable.table[n];
		}
		else
		{
			throw ERROR_THROW(121);
		}
	}

	void Delete(LexTable& lextable)
	{
		delete lextable.table;
		lextable.table = nullptr;
	}

	void LexTablePrint(LexTable& lextable, wchar_t file[])
	{
		std::ofstream out(file);
		int curLine = 0;

		out << "1" << ' ';
		for (int i = 0; i < lextable.current_size; i++)
		{
			if (curLine != lextable.table[i].sn)
			{
				curLine++;
				out << std::endl << curLine + 1 << ' ';
			}
			out << lextable.table[i].lexema;
		}
		out.close();
	}
}