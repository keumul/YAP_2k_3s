#include "stdafx.h"

namespace PN
{
	bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		std::stack <LT::Entry> operators;
		std::queue <LT::Entry> str;
		int parm_counter = 0;
		int curLine = lextable.table[lextable_pos].sn;
		bool parameters = false;

		for (int i = lextable_pos; lextable.table[i].lexema != LEX_SEMICOLON; i++)
		{
			if (lextable.table[i].lexema == LEX_ID || lextable.table[i].lexema == LEX_LITERAL)
			{
				if (idtable.table[lextable.table[i].idxTI].idtype == IT::F)
				{
					operators.push({ '@', curLine, lextable.table[i].idxTI });
					parameters = true;
				}
				else
				{
					parm_counter += (parameters) ? 1 : 0;
					str.push(lextable.table[i]);
				}

				continue;
			}

			if (lextable.table[i].lexema == LEX_RIGHTHESIS)
			{
				while (operators.top().lexema != LEX_LEFTHESIS)
				{
					str.push(operators.top());
					operators.pop();
				}
				operators.pop();

				if (operators.top().lexema == '*' || operators.top().lexema == '/')
				{
					str.push(operators.top());
					operators.pop();
				}
				else if (operators.top().lexema == '@')
				{
					str.push(operators.top());
					str.push({ (char)(parm_counter + 48), curLine, (int)LT_TI_NULLIDX });
					operators.pop();
					parm_counter = 0;
					parameters = false;
				}

				continue;
			}

			if (lextable.table[i].lexema == LEX_COMMA)
			{
				continue;
			}

			if (lextable.table[i].lexema == LEX_EQUAL)
			{
				str.push(lextable.table[i]);
				continue;
			}

			if (lextable.table[i].idxTI != TI_NULLIDX && (idtable.table[lextable.table[i].idxTI].id[0] == '*' ||
				idtable.table[lextable.table[i].idxTI].id[0] == '/'))
			{
				operators.push({ LEX_OPERATIONS, curLine, lextable.table[i].idxTI });
				continue;
			}

			if (lextable.table[i].idxTI != TI_NULLIDX && (idtable.table[lextable.table[i].idxTI].id[0] == '+' ||
				idtable.table[lextable.table[i].idxTI].id[0] == '-'))
			{
				if (!operators.empty())
				{
					while (!operators.empty() && (operators.top().lexema == '*' || operators.top().lexema == '/'))
					{
						str.push(operators.top());
						operators.pop();
					}
					operators.push({ LEX_OPERATIONS, curLine, lextable.table[i].idxTI });
				}
				else
				{
					operators.push({ LEX_OPERATIONS, curLine, lextable.table[i].idxTI });
				}
			}
			else
			{
				operators.push(lextable.table[i]);
			}
		}

		while (!operators.empty())
		{
			str.push(operators.top());
			operators.pop();
		}

		int size = str.size() + lextable_pos;

		for (lextable_pos; lextable_pos < size; lextable_pos++)
		{
			lextable.table[lextable_pos] = str.front();
			str.pop();
		}

		lextable.table[lextable_pos++] = { LEX_SEMICOLON, curLine, (int)LT_TI_NULLIDX };

		for (lextable_pos; lextable.table[lextable_pos].lexema != LEX_SEMICOLON; lextable_pos++)
		{
			lextable.table[lextable_pos] = { '$', curLine, (int)LT_TI_NULLIDX };
		}

		lextable.table[lextable_pos] = { '$', curLine, (int)LT_TI_NULLIDX };

		return true;
	}
}