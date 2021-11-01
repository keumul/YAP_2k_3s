#include "stdafx.h"

namespace FST
{
	RELATION::RELATION(char c, short nn)
	{
		symbol = c;
		nnode = nn;
	}

	NODE::NODE()
	{
		n_relation = 0;
		RELATION* relaions = NULL;
	}

	NODE::NODE(short n, RELATION rel, ...)
	{
		n_relation = n;
		RELATION* p = &rel;
		relations = new RELATION[n];

		for (short i = 0; i < n; i++)
		{
			relations[i] = p[i];
		}
	}

	FST::FST(char* s, short ns, NODE n, ...)
	{
		string = s;
		rstates = new short[ns] {};
		nstates = ns;
		nodes = new NODE[ns];
		NODE* p = &n;
		position = 0;

		for (int i = 0; i < ns; i++)
		{
			nodes[i] = p[i];
		}
	}

	bool step(FST& fst, short*& rstates)												//шаг конечного автомата
	{
		bool rc = false;
		std::swap(rstates, fst.rstates);

		for (short i = 0; i < fst.nstates; i++)
		{
			if (rstates[i] == fst.position)
			{
				for (short j = 0; j < fst.nodes[i].n_relation; j++)						//возможностей перехода								
				{
					if (fst.nodes[i].relations[j].symbol == fst.string[fst.position])	//инцидентное ребро равно текущей позиции
					{
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
					}
				}
			}
		}

		return rc;
	}

	bool execute(FST& fst)
	{
		short* rstates = new short[fst.nstates];
		short lstring = strlen(fst.string);
		bool rc = true;

		for (short i = 0; i < lstring && rc; i++)
		{
			rc = step(fst, rstates);
			fst.position++;
		}

		delete[] rstates;
		return (fst.rstates[fst.nstates - 1] == lstring) ? true : false;
	}
}