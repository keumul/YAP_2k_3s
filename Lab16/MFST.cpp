#include "stdafx.h"

namespace MFST
{
	/*��� ���������� ��������� ��������; ��������� ��������� �������� ���������� ��� ����, ����� �����
	����������� � ����� ��������� ��������� � ����������� �������������� ������� ��������������� �������(� ����
	��������������������� ��������)*/ 

	MfstState::MfstState()  // ����������� �� ���������
	{
		this->lenta_position = 0;  //������� ������� �� ������� ����� ��������
		this->nrule = -1; //����� �������� �������
		this->nrulechain = -1; //����� ������� �������
	}

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)
	{
		this->lenta_position = pposition; //������� �� �����
		this->st = pst; //���� ��������
		this->nrulechain = pnrulechain; //����� �������� ������� � �������
	}

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
	{
		this->lenta_position = pposition; //������� � �����
		this->st = pst; //���� ��������
		this->nrule = pnrule; //����� �������� �������
		this->nrulechain = pnrulechain; //����� ������� �������
	}

	//������������� ������ �����������
	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		this->lenta_position = -1; //������������� ������ �����������
		this->rc_step = SURPRISE;
		this->nrule = -1; //����� ������������ �� ������ ����������� ������� ����������
		this->nrule_chain = -1; //����� ������� ������� ������������ �� ������ ����������� ������� ����������
	}

	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		this->lenta_position = plenta_position; //������� �� �����
		this->rc_step = prc_step; //��� ���������� ����
		this->nrule = pnrule; //����� �������
		this->nrule_chain = pnrule_chain; //����� ������� �������
	}

	//������������� ����������� ��������� ��������
	Mfst::Mfst() //����������� �� ��������� 
	{
		this->lenta = 0;
		this->lenta_size = 0;
		this->lenta_position = 0;
	}
	//(��������� ������ ������������ �����������, ���������� �������)
	Mfst::Mfst(LT::LexTable& plexTable, GRB::Greibach pgrebach, wchar_t syntfile[])
	{
		this->grebach = pgrebach; //�������
		this->lexTable = plexTable; //��������� ������ ������������ �����������
		this->lenta_size = lexTable.current_size; //������ ����� = ������� ������ ������� ������
		this->lenta = new short[lexTable.current_size]; 

		for (int k = 0; k < this->lenta_size; k++) //������� � ����� ���������
		{
			this->lenta[k] = GRB::Rule::Chain::T((GRBALPHABET)lexTable.table[k].lexema);
		}

		this->lenta_position = 0;
		this->st.push_back(grebach.stbottomT); //��������� ��� �����
		this->st.push_back(grebach.startN); //��������� ��������� ������
		this->nrulechain = -1; //���������� ������� ����� -1

		this->synt = new std::ofstream;
		this->synt->open(syntfile);
		if (!this->synt->is_open())
		{
			throw ERROR_THROW(115);
		}
	}

	//��������� ���� ������ ��������, ��������� ��������������� ���������, ������������ ���������� ����� �� �������
	Mfst::RC_STEP Mfst::step() 
	{
		RC_STEP rc = SURPRISE; //����������� ��� �������� (������ ��������)

		if (lenta_position < lenta_size) //���� ������� ������� ������ ������� ����� 
		{
			//��������� ��������� ������� ����� � ��������� ���������� �� ��
			if (GRB::Rule::Chain::isN(st[st.size() - 1]))
			{
				GRB::Rule rule;
				if ((nrule = grebach.getRule(st[st.size() - 1], rule)) >= 0) //�������, ���� ����� ������� ���� ��� ������
				{
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0) //�������� ���� ������� � ������� � �����, ��� -1
					{
						MFST_TRACE1(*Mfst::synt) //�����
						savestate();			 //��������� ��������� ��������
						st.pop_back();			 //��������� �������� �����.
						push_chain(chain);		 //��������� ������� ������� � ����
						rc = NS_OK;				 //������� ������� � �������... ������� �������� � ����
						MFST_TRACE2(*Mfst::synt) //�����
					}
					else
					{
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE", *Mfst::synt) //�����
						savedDiagnosis(NS_NORULECHAIN);						  //��� ����������
						rc = resetstate() ? NS_NORULECHAIN : NS_NORULE;       //������������ ��������� ��������
					};
				}
				else
				{
					rc = NS_ERROR; //����������� �������������� ������ ����������
				}
			}
			else if (st[st.size() - 1] == lenta[lenta_position]) //���� ������� $
			{
				lenta_position++;	//���������� �����
				st.pop_back();		//������� �����
				nrulechain = -1;	//����� �������� ������� ����� -1
				rc = TS_OK;
				MFST_TRACE3(*Mfst::synt)
			}
			else
			{   //��� ������ ��� �������
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

	//�������� ������ ������� (������������ ��������) � ���� ��������, ������ ���������� true
	bool Mfst::push_chain(GRB::Rule::Chain chain) //��������� ������� ������� � ���� (������� �������)
	{
		for (int k = chain.size - 1; k >= 0; k--) //� = (������ �������-1) + ������� ������� � ����
		{
			st.push_back(chain.chainOfTerm_nTerm[k]);
		}
		return true;
	}

	//��������� ������� ��������� �������� � storestate, ������ ���������� true
	bool Mfst::savestate() //��������� ��������� ��������(��� ������)
	{
		storestate.push_back(MfstState(lenta_position, st, nrule, nrulechain)); //���� ��� ���������� ���������. ������� ����������� ���������
		MFST_TRACE6("SAVESTATE:", storestate.size(), *Mfst::synt)
		return true;
	}

	//��������������� ��������� ����������� ��������� �������� �� storestate, ���������� true, ���� �������������� ���������(���� ������ ��� ��������������)
	bool Mfst::resetstate() //������������ ��������� ��������
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

	//��������� � ������� diagnosis ������ �����������; � ������� diagnosis ����������� ��������������� 
	//��������� � ������� �������� ������� �����(��������� �����������) � ������ � ������ ����� �������(������ MFST_DIAGN_NUMBER)
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

	//��������� ������ ��������, � ����� ��������� ������� step, ������������ ����� ��������������� ���������
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
				* Mfst::synt << lenta_size << " ����� �����, �������������� ������ �������� ��� ������" << std::endl;
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

	//��������� ���� �������� � �����; ��������� ����� ���������� �����(� ������� ASCII������) ��� �����������, � ����� 0x00; ���������� � �����
	//������ ��������� �� �����
	char* Mfst::getCSt(char* buf) //����� �����
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

	//��������� �����(������ ��������) ���������� ����� � �������� �������(������ ��������) �������� ���������� ��������(������ ��������) � �������
	//ASCII - ������ ��� �����������, � ����� ������ 0x00; ���������� � ����� ������ ��������� �� ������ �����
	char* Mfst::getCLenta(char* buf, short pos, short n) //����� �����
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)
		{
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		}
		buf[i - pos] = '\0';
		return buf;
	}

	//��������� �����(������ ��������) ���������� ����� � �������� �������(������ ��������) �������� ���������� ��������(������ ��������) � ������� ASCII - 
	//������ ��� �����������, � ����� ������ 0x00, ���������� � ����� ������ ��������� �� ������ �����
	char* Mfst::getDiagnosis(short n, char* buf) //����� ������
	{
		char* rc = new char[200]{};
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::geterror(errid);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: ������ %d,	%s", err.id, lexTable.table[lpos].sn, err.message);
			rc = buf;
		}
		return rc;
	}

	void Mfst::printRules() //����� ������
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

	bool Mfst::savededucation() //����� ������ �������
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