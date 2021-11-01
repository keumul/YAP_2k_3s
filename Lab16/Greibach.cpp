#include "stdafx.h"

#define GRB_ERROR_SERIES 600

namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

	//представление цепочки - правой стороны правила
						//размер цепочки  //цепочка
	Rule::Chain::Chain(short symbolCount, GRBALPHABET s, ...)
	{
		this->size = symbolCount;
		this->chainOfTerm_nTerm = new GRBALPHABET[symbolCount];
		int* ptr = (int*)&s;
		for (int i = 0; i < symbolCount; i++)
		{
			this->chainOfTerm_nTerm[i] = (GRBALPHABET)ptr[i];
		}
	}

	//представление одного правила, имеющего вид : A->xxx | yyy | …
	//нетерминал(лев символ правила), идентификатор ошибки, связанной с правилом,  
	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) // цепочки - правые стороны правила
	{										//количество цепочек в правой стороне правила
		this->nn = pnn;										
		this->iderror = piderror;
		this->size = psize;
		this->chains = new Chain[psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++)
		{
			this->chains[i] = p[i];
		}
	}

	/*представление грамматики. Терминалы – положительные значения, нетерминалы – отрицательные.*/
	Greibach::Greibach(
		GRBALPHABET pstartN, // стартовый символ грамматики
		GRBALPHABET pstbottom, //служебный символ (дно стека и последняя лексема таблице лексем)
		short psize, //количество правил
		Rule r, ...) //множество правил
	{
		this->startN = pstartN;
		this->stbottomT = pstbottom;
		this->size = psize;
		this->rules = new Rule[psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++)
		{
			rules[i] = p[i];
		}
	}

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule) //получить номер правила или - 1 (к точке возврата)
	{//							нетерменал		
		short rc = -1, k = 0;
		while (k < this->size && rules[k].nn != pnn)
		{
			k++;
		}
		if (k < this->size)
		{
			rc = k;
			prule = rules[k];
		}
		return rc;
	}

	// позволяет получить правило(возвращает к точке вызова параметр типа Rule) по его номеру
	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < this->size)
		{
			rc = rules[n];
		}
		return rc;
	}

	//позволяет получить строку - цепочку в символьном виде для отображения
	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < this->size; i++)
		{
			b[i] = Chain::alphabet_to_char(this->chainOfTerm_nTerm[i]);
		}
		b[this->size] = 0x00;
		return b;
	}

	// позволяет получить правило в виде строки вида N->цепочка(в символьном ASCII - виде, для отображения)
	char* Rule::getCRule(char* b, short nchain)
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(this->nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		this->chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	}

	// позволяет получить строку - цепочку в символьном виде для отображения
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		while (j < this->size && this->chains[j].chainOfTerm_nTerm[0] != t)
		{
			++j;
		}
		rc = (j < this->size ? j : -1);
		if (rc >= 0)
		{
			pchain = chains[rc];
		}
		return rc;
	}

	Greibach greibach(
		NS('S'), TS('$'), // стартовый символ, дно стека
		6,			      //количество правил
		Rule(NS('S'), GRB_ERROR_SERIES + 0, // Неверная структура программы	
			4,								// S-> m{NrE;}; | tfi(F){NrE;};S | m{NrE;};S | tfi(F){NrE;};
			Rule::Chain(8, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
			Rule::Chain(14, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(9, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'))
		),

		Rule(NS('N'), GRB_ERROR_SERIES + 1, //ошибочный оператор
			10,								// N-> dti; | rE; | i=E; | dtfi(F); | dti;N | rE;N | i=E;N | dtfi(F);N | pE; | pE;N 
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(8, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')),
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(9, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 2, //ошибка в выражении
			8,								// E-> i| l | (E) | i(W) | iM | lM | (E)M | i(W)M
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		),

		Rule(NS('M'), GRB_ERROR_SERIES + 2,
			2,						// M-> vE | vEM
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		),

		Rule(NS('F'), GRB_ERROR_SERIES + 3,
			2,						// F-> ti | ti,F
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),

		Rule(NS('W'), GRB_ERROR_SERIES + 4,
			4,						// W-> i | l | i,W | l,W
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		)
	);

	Greibach getGreibach()
	{
		return greibach;
	}
}