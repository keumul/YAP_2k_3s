#include "stdafx.h"

namespace MFST
{
	/*для сохранения состояния автомата; сохранять состояние автомата необходимо для того, чтобы иметь
	возможность к этому состоянию вернуться и осуществить альтернативный вариант синтаксического разбора(в силу
	недетерминированности автомата)*/ 

	MfstState::MfstState()  // конструктор по умолчанию
	{
		this->lenta_position = 0;  //текущую позицию на входной ленте автомата
		this->nrule = -1; //номер текущего правила
		this->nrulechain = -1; //номер текущей цепочки
	}

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)
	{
		this->lenta_position = pposition; //позиция на ленте
		this->st = pst; //стек автомата
		this->nrulechain = pnrulechain; //номер текущего правила и цепочки
	}

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
	{
		this->lenta_position = pposition; //позиция в ленте
		this->st = pst; //стек автомата
		this->nrule = pnrule; //номер текущего правила
		this->nrulechain = pnrulechain; //номер текущей цепочки
	}

	//представление строки диагностики
	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		this->lenta_position = -1; //представление строки диагностики
		this->rc_step = SURPRISE;
		this->nrule = -1; //номер действующего на момент диагностики правила грамматики
		this->nrule_chain = -1; //номер текущей цепочки действующего на момент диагностики правила грамматики
	}

	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		this->lenta_position = plenta_position; //позиция на ленте
		this->rc_step = prc_step; //код завершения шага
		this->nrule = pnrule; //номер правила
		this->nrule_chain = pnrule_chain; //номер цепочки правила
	}

	//представление магазинного конечного автомата
	Mfst::Mfst() //конструктор по умолчанию 
	{
		this->lenta = 0;
		this->lenta_size = 0;
		this->lenta_position = 0;
	}
	//(результат работы лексического анализатора, грамматика Грейбах)
	Mfst::Mfst(LT::LexTable& plexTable, GRB::Greibach pgrebach, wchar_t syntfile[])
	{
		this->grebach = pgrebach; //Грейбах
		this->lexTable = plexTable; //результат работы лексического анализатора
		this->lenta_size = lexTable.current_size; //размер ленты = текущий размер таблицы лексем
		this->lenta = new short[lexTable.current_size]; 

		for (int k = 0; k < this->lenta_size; k++) //заносит в ленту терминалы
		{
			this->lenta[k] = GRB::Rule::Chain::T((GRBALPHABET)lexTable.table[k].lexema);
		}

		this->lenta_position = 0;
		this->st.push_back(grebach.stbottomT); //добавляет дно стека
		this->st.push_back(grebach.startN); //добавляет стартовый символ
		this->nrulechain = -1; //изначально правило равно -1

		this->synt = new std::ofstream;
		this->synt->open(syntfile);
		if (!this->synt->is_open())
		{
			throw ERROR_THROW(115);
		}
	}

	//выполняет такт работы автомата, формирует диагностические сообщения, осуществляет отладочный вывод на консоль
	Mfst::RC_STEP Mfst::step() 
	{
		RC_STEP rc = SURPRISE; //неожиданный код возврата (ошибка возврата)

		if (lenta_position < lenta_size) //если текущая позиция меньше размера ленты 
		{
			//извлекаем последний элемент стека и проверяем нетерминал ли он
			if (GRB::Rule::Chain::isN(st[st.size() - 1]))
			{
				GRB::Rule rule;
				if ((nrule = grebach.getRule(st[st.size() - 1], rule)) >= 0) //смотрим, если такое правило есть идём дальше
				{
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0) //получаем след цепочку и выводим её номер, или -1
					{
						MFST_TRACE1(*Mfst::synt) //вывод
						savestate();			 //сохранить состояние автомата
						st.pop_back();			 //извлекаем верхушку стека.
						push_chain(chain);		 //поместить цепочку правила в стек
						rc = NS_OK;				 //найдено правило и цепочка... цепочка записана в стек
						MFST_TRACE2(*Mfst::synt) //вывод
					}
					else
					{
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE", *Mfst::synt) //вывод
						savedDiagnosis(NS_NORULECHAIN);						  //код завершения
						rc = resetstate() ? NS_NORULECHAIN : NS_NORULE;       //восстановить состояние автомата
					};
				}
				else
				{
					rc = NS_ERROR; //неизвестный нетерминальный символ грамматики
				}
			}
			else if (st[st.size() - 1] == lenta[lenta_position]) //если словили $
			{
				lenta_position++;	//продвигаем ленту
				st.pop_back();		//вершина стека
				nrulechain = -1;	//номер текущего правила равен -1
				rc = TS_OK;
				MFST_TRACE3(*Mfst::synt)
			}
			else
			{   //нет правил для цепочки
				MFST_TRACE4("TS_NOK/NS_NORULECHAIN", *Mfst::synt)
				rc = resetstate() ? TS_NOK : NS_NORULECHAIN;
			}
		}
		else
		{
			rc = LENTA_END;
			MFST_TRACE4("LENTA_END", *Mfst::synt)
		}

		return rc;
	};

	//помещает реверс цепочки (единственный параметр) в стек автомата, всегда возвращает true
	bool Mfst::push_chain(GRB::Rule::Chain chain) //поместить цепочку правила в стек (цепочка правила)
	{
		for (int k = chain.size - 1; k >= 0; k--) //к = (длинне цепочке-1) + заносим цепочку в стек
		{
			st.push_back(chain.chainOfTerm_nTerm[k]);
		}
		return true;
	}

	//сохраняет текущее состояние автомата в storestate, всегда возвращает true
	bool Mfst::savestate() //сохранить состояние автомата(для отката)
	{
		storestate.push_back(MfstState(lenta_position, st, nrule, nrulechain)); //стек для сохранения состояния. заносим сохраняемое состояние
		MFST_TRACE6("SAVESTATE:", storestate.size(), *Mfst::synt)
		return true;
	}

	//восстанавливает последнее сохраненное состояние автомата из storestate, возвращает true, если восстановление выполнено(есть данные для восстановления)
	bool Mfst::resetstate() //восстановить состояние автомата
	{
		bool rc = false;
		MfstState state; 

		if (rc = (storestate.size() > 0))
		{
			state = storestate[storestate.size() - 1];
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop_back();

			MFST_TRACE5("RESTATE", *Mfst::synt)
			MFST_TRACE2(*Mfst::synt)
		}
		return rc;
	}

	//сохраняет в массиве diagnosis строку диагностики; в массиве diagnosis сохраняются диагностические 
	//сообщения в порядке убывания позиции ленты(вызвавшей диагностику) и только в равным длине массива(макрос MFST_DIAGN_NUMBER)
	bool Mfst::savedDiagnosis(RC_STEP prc_step)
	{
		bool rc = false;
		short k = 0;

		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
		{
			k++;
		}

		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);

			for (int i = k + 1; i < MFST_DIAGN_NUMBER; i++)
			{
				diagnosis[i].lenta_position = -1;
			}
		}
		return rc;
	}

	//запускает работу автомата, в цикле выполняет функцию step, осуществляет вывод диагностических сообщений
	bool Mfst::start()
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_step = step();

		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
		{
			rc_step = step();
		}

		switch (rc_step)
		{
		case LENTA_END:
			MFST_TRACE4("------>LENTA_END", *Mfst::synt)
				* Mfst::synt << lenta_size << " всего строк, синтаксический анализ выполнен без ошибок" << std::endl;
			rc = true;
			break;

		case NS_NORULE:
			MFST_TRACE4("------>NS_NORULE", *Mfst::synt)
				* Mfst::synt << "------------------------------------------------------------------------------------------   ------" << std::endl;
			*Mfst::synt << getDiagnosis(0, buf) << std::endl;
			*Mfst::synt << getDiagnosis(1, buf) << std::endl;
			*Mfst::synt << getDiagnosis(2, buf) << std::endl;
			break;

		case NS_NORULECHAIN:
			MFST_TRACE4("------>NS_NORULECHAIN", *Mfst::synt)
				break;

		case NS_ERROR:
			MFST_TRACE4("------>NS_ERROR", *Mfst::synt)
				break;

		case SURPRISE:
			MFST_TRACE4("------>NS_SURPRISE", *Mfst::synt)
				break;
		};

		return rc;
	}

	//принимает один параметр – буфер; заполняет буфер содержимым стека(в формате ASCIIстроки) для отображения, в конце 0x00; возвращает к точке
	//вызова указатель на буфер
	char* Mfst::getCSt(char* buf) //вывод стека
	{
		short p;
		int size = st.size();
		for (int i = size - 1; i >= 0; --i)
		{
			p = st[i];
			buf[size - 1 - i] = GRB::Rule::Chain::alphabet_to_char(p);
		}
		buf[size] = '\0';
		return buf;
	}

	//заполняет буфер(первый параметр) содержимым ленты с заданной позиции(второй параметр) заданное количество символов(третий параметр) в формате
	//ASCII - строки для отображения, в конце строки 0x00; возвращает к точке вызова указатель на строки буфер
	char* Mfst::getCLenta(char* buf, short pos, short n) //вывод ленты
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)
		{
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		}
		buf[i - pos] = '\0';
		return buf;
	}

	//заполняет буфер(первый параметр) содержимым ленты с заданной позиции(второй параметр) заданное количество символов(третий параметр) в формате ASCII - 
	//строки для отображения, в конце строки 0x00, возвращает к точке вызова указатель на строки буфер
	char* Mfst::getDiagnosis(short n, char* buf) //вывод ошибок
	{
		char* rc = new char[200]{};
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::geterror(errid);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,	%s", err.id, lexTable.table[lpos].sn, err.message);
			rc = buf;
		}
		return rc;
	}

	void Mfst::printRules() //вывод правил
	{
		MfstState state;
		GRB::Rule rule;
		int storestate_size = storestate.size();
		for (unsigned short i = 0; i < storestate_size; i++)
		{
			state = storestate[i];
			rule = grebach.getRule(state.nrule);
			MFST_TRACE7(*Mfst::synt)
		}
	}

	bool Mfst::savededucation() //вывод дерева разбора
	{
		MfstState state;
		GRB::Rule rule;
		int storestate_size = storestate.size();
		deducation.nrules = new short[deducation.size = storestate_size];
		deducation.nrulechains = new short[deducation.size];

		for (unsigned short i = 0; i < storestate_size; i++)
		{
			state = storestate[i];
			deducation.nrules[i] = state.nrule;
			deducation.nrulechains[i] = state.nrulechain;
		}

		return true;
	}
}