#include "stdafx.h"

namespace LT
{
	LexTable Create(int size)
	{
		if (size <= LT_MAXSIZE)
		{
			LT::LexTable lextable;
			lextable.maxsize = size;
			lextable.current_size = 0;
			lextable.table = new LT::Entry[size];
			return lextable;
		}
		else
		{
			throw ERROR_THROW(122);
		}
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
			throw ERROR_THROW_IN(121, lextable.table[n].sn, -1);
		}
	}

	void Delete(LexTable& lextable)
	{
		delete lextable.table;
		lextable.table = nullptr;
	}

	void LexTablePrint(LexTable& lextable, wchar_t LTfile[])
	{
		std::ofstream fout(LTfile);
		int curLine = 0;
#if DEBUGGING == 0
		fout << "1" << '\t';
		for (int i = 0; i < lextable.current_size; i++)
		{
			if (curLine != lextable.table[i].sn)
			{
				curLine++;
				fout << std::endl << curLine + 1 << '\t';
			}
			fout << lextable.table[i].lexema;
		}
		fout.close();
#else
		for (int i = 0; i < lextable.current_size; i++)
		{
			fout << i << "\t" << lextable.table[i].lexema << "\t";
			if (lextable.table[i].idxTI != LT_TI_NULLIDX) fout << lextable.table[i].idxTI;
			fout << "\n";
		}
#endif // DEBUGGING
	}
}