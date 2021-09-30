#include "stdafx.h"
#include "IT.h"
#include "Error.h"
#include <iomanip>

namespace IT
{
	IdTable Create(int size) // ������� ������� ���������������
	{
		IdTable* tabl = new IdTable; //�������� ������ ��� �������
		if (size > TI_MAXSIZE) { throw ERROR_THROW(160); }

		tabl->maxsize = size;
		tabl->size = 0;
		tabl->table = new Entry[size];
		return*tabl;
	}

	void Add(IdTable& idtable, Entry entry) // �������� ������ � ������� ���������������
	{
		if (idtable.size + 1 > idtable.maxsize) { throw ERROR_THROW(161); }

		idtable.table[idtable.size] = entry; // ������ ������� ���������������
		idtable.size += 1; 
	}

	Entry GetEntry(IdTable& idtable, int n) { return idtable.table[n]; } // �������� ������ ������� ���������������

	int IsId(IdTable& idtable, char id[ID_MAXSIZE]) // �������: ����� ������(���� ����), TI_NULLIDX(���� ���)
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == 0) { return i; }
			//������� �������� ������� � ���, ��� ��� ������ �����.
			//�������� ������ ���� ��������� �� ��, ��� ������ string1 ������ ������ string2, �������� ������ ���� ��������������� �� ��������.
		}
		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable) // ������� ������� ������ (���������� ������)
	{
		delete[] idtable.table;
	}

	void showTable(IdTable& idtable)
	{
		int i, number = 0;
		std::cout << " �" << "	| " << "�������������" << " | " << "��� ������" << " | " << "��� ��������������" << " | " << "������ (��)" << " | " << "�������� " << std::endl;
		for (i = 0; i < idtable.size; i++)
		{
			std::cout << std::setfill('0') << std::setw(4) << std::right << i << " | "; //��������� �����
			std::cout << std::setfill(' ') << std::setw(13) << std::left << idtable.table[i].id << " | "; //�������������
			switch (idtable.table[i].iddatatype)
			{
			case INT: std::cout << std::setw(10) << std::left; //���� INT
				if (idtable.table[i].idtype == OP) std::cout << "-" << " | ";
				else std::cout << "integer" << " | "; break;
			case STR: std::cout << std::setw(10) << std::left; //���� STRING
				if (idtable.table[i].idtype == OP) std::cout << "-" << " | ";
				else std::cout << "string" << " | "; break;
			default: std::cout << std::setw(10) << std::left << "?" << " | "; break; //���� �������� �� ��������
			}
			switch (idtable.table[i].idtype)
			{//�������� � ����������� �� ���� ���������������
			case V: std::cout << std::setw(18) << std::left << "����������" << " | "; break;
			case F: std::cout << std::setw(18) << std::left << "�������" << " | "; break;
			case P: std::cout << std::setw(18) << std::left << "��������" << " | "; break;
			case L: std::cout << std::setw(18) << std::left << "�������" << " | "; break;
			case OP: std::cout << std::setw(18) << std::left << "��������" << " | ";
				number++;
				break;
			default: std::cout << std::setw(18) << std::left << "?" << " | "; break; //���� �������� �� ��������
			}
			std::cout << std::setw(11) << std::left << idtable.table[i].idxfirstLE << " | "; //������� � ������� ������ 

			//���� �������� ����� int � �������� ��� ����������, ��� ���������, �� ������� 
			if (idtable.table[i].iddatatype == INT && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				std::cout << std::setw(18) << std::left << idtable.table[i].value.vint;

			//���� �������� ����� string � �������� ��� ����������, ��� ���������, �� ������� 
			else if (idtable.table[i].iddatatype == STR && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				std::cout << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else
				std::cout << "-";
			std::cout << std::endl;
		}
		std::cout << "���������� ���������������: " << i - number << std::endl;
	}
};