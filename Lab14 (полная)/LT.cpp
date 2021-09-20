#include "LT.h"
#include "stdafx.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
using namespace Log;
using namespace std;

namespace LT
{
	LexTable Create(int size) //������� ������� ������
	{
		//����������� �������� ������ ��� �������
		LexTable* Table = new LexTable;
		if (size > LT_MAXSIZE)
		{ throw ERROR_THROW(120); }
		Table->maxsize = size;
		Table->size = 0;
		Table->table = new Entry[size];

		return *Table;
	}
	//��������� � ������� ������
	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.size + 1 > lextable.maxsize) { throw ERROR_THROW(121); }
		lextable.table[lextable.size] = entry;
		lextable.size += 1;
	}
	// �������� ������ ������� ���������������
	Entry GetEntry(LexTable& lextable, int n)
	{
		return lextable.table[n];
	}
	//�������
	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
	}
	//��������� �������
	Entry writeEntry(Entry& entry, char lexema, int indx, int line)
	{
		entry.lexema = lexema;  //���������� �������
		entry.idxTI = indx;		//���������� id (���� ���-����� null)
		entry.sn = line;		//���������� ����� ������
		return entry;			
	}

	void showTable(LexTable lextable, Parm::PARM parm) // ����� ������� ������
	{
		std::fstream out;
		out.open(parm.out, std::ios::out); //ios::app - ���������� � ����� �����
		if (!out.is_open()){ throw ERROR_THROW(110); }
		out << "01 ";

		int number = 1;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].sn != number && lextable.table[i].sn != -1) //��������� �����
			{
				if (number < 9)
				{
					out << std::endl << '0' << lextable.table[i].sn << ' ';
				}
				else
				{
					out << std::endl << lextable.table[i].sn << ' ';
				}
				number++;
			}
			//���������� ��������
			out << lextable.table[i].lexema;
			if (lextable.table[i].lexema == LEX_ID || lextable.table[i].lexema == LEX_OPERATOR || lextable.table[i].lexema == LEX_LITERAL)
			{
				out << "<" << lextable.table[i].idxTI << ">";
			}
		}
	}
}