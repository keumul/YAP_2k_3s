#pragma once

typedef short GRBALPHABET;	// символы алфавита грамматики терминалы > 0,нетерминалы < 0

namespace GRB
{
	struct Rule //правило в грамматике 
	{
		GRBALPHABET nn; //нетерминал(левый символ правила) <0
		int iderror; //идентификатор диагностического сообщения
		short size; //количество цепочек - правых частей правила

		struct Chain //цепочка(правая часть правила)
		{
			short size; //длина цепочки
			GRBALPHABET* chainOfTerm_nTerm; //цепочка терминалов(>0 и нетерминалов (<0)

			Chain()
			{
				this->size = 0;
				this->chainOfTerm_nTerm = 0;
			};
			            //количество символов в цепочке, символы (терминал или нетерминал)
			Chain(short symbolCount, GRBALPHABET s, ...);

			char* getCChain(char* b);		//получить правую сторону правила
			static GRBALPHABET T(char t) { return GRBALPHABET(t); } //терминал
			static GRBALPHABET N(char n) { return -GRBALPHABET(n); } //нетерминал
			static bool isT(GRBALPHABET s) { return s > 0; } //терминал?
			static bool isN(GRBALPHABET s) { return s < 0; } //нетерминал?
			static char alphabet_to_char(GRBALPHABET s) { return (isT(s) ? char(s) : char(-s)); } //GRBALPHABET->char
		}*chains;

		Rule()
		{
			this->nn = 0x00;
			this->size = 0;
			this->chains = nullptr;
			this->iderror = -1;
		}
		Rule(GRBALPHABET pnn, //нетерминал (<0)
			int iderror,  //идентификатор диагностического сообщения
			short psize,   //количество цепочек - правых частей правила
			Chain c, ...);  //множество цепочек - правых частей правила

		char* getCRule(//получить правило в виде N->цепочка(для распечатки)
			char* b,   //буфер
			short nchain);//номер цепочки(правой части) в правиле

		short getNextChain(//получить следующую за j подходящую цепочку, вернуть её номер или -1
			GRBALPHABET t,  //первый символ цепочки
			Rule::Chain& pchain,  //возвращаемая цепочка
			short j);         //номер цепочки
	};

	struct Greibach // грамматика Грейбах
	{
		short size; //количество правил
		GRBALPHABET startN;		//стартовый символ
		GRBALPHABET stbottomT;  //дно стека
		Rule* rules; //количество правил

		Greibach()
		{
			this->size = 0;
			this->startN = 0;
			this->stbottomT = 0;
			this->rules = 0;
		};
		Greibach(
			GRBALPHABET pstartN,  //стартовый символ
			GRBALPHABET pstbottomT,  //дно стека
			short psize,  //количество правил
			Rule r, ...); //правила
		 
		short getRule( //получить правило, возвращается номер правила или -1
			GRBALPHABET pnn, //левый символ правила
			Rule& prule); //возвращаемое правило грамматики
		Rule getRule(short n); //получить правило по номеру
	};

	Greibach getGreibach(); //получить грамматику
}