#include "stdafx.h"

namespace PN
{
	bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		std::stack <LT::Entry> stack;
		std::queue <LT::Entry> queue;
		int parm_counter = 0;
		int curLine = lextable.table[lextable_pos].sn;
		bool param = false;

		for (int i = lextable_pos; lextable.table[i].lexema != LEX_SEMICOLON; i++)
		{
			if (lextable.table[i].lexema == LEX_ID || lextable.table[i].lexema == LEX_LITERAL)
			{
				if (idtable.table[lextable.table[i].idxTI].idtype == IT::F)
				{
					stack.push({ '@', curLine, lextable.table[i].idxTI });
					param = true;
				}
				else
				{
					parm_counter += (param) ? 1 : 0;
					queue.push(lextable.table[i]);
				}

				continue;
			}

			if (lextable.table[i].lexema == LEX_RIGHTHESIS)
			{
				while (stack.top().lexema != LEX_LEFTHESIS)
				{
					queue.push(stack.top());
					stack.pop();
				}
				stack.pop();

				if (stack.top().lexema == '*' || stack.top().lexema == '/')
				{
					queue.push(stack.top());
					stack.pop();
				}
				else if (stack.top().lexema == '@')
				{
					queue.push(stack.top());
					queue.push({ (char)(parm_counter + 48), curLine, (int)LT_TI_NULLIDX });
					stack.pop();
					parm_counter = 0;
					param = false;
				}

				continue;
			}

			if (lextable.table[i].lexema == LEX_COMMA)
			{
				continue;
			}

			if (lextable.table[i].lexema == LEX_EQUAL)
			{
				queue.push(lextable.table[i]);
				continue;
			}

			if (lextable.table[i].idxTI != TI_NULLIDX && (idtable.table[lextable.table[i].idxTI].id[0] == '*' ||
				idtable.table[lextable.table[i].idxTI].id[0] == '/'))
			{
				stack.push({ LEX_OPERATIONS, curLine, lextable.table[i].idxTI });
				continue;
			}

			if (lextable.table[i].idxTI != TI_NULLIDX && (idtable.table[lextable.table[i].idxTI].id[0] == '+' ||
				idtable.table[lextable.table[i].idxTI].id[0] == '-'))
			{
				if (!stack.empty())
				{
					while (!stack.empty() && (stack.top().lexema == '*' || stack.top().lexema == '/'))
					{
						queue.push(stack.top());
						stack.pop();
					}
					stack.push({ LEX_OPERATIONS, curLine, lextable.table[i].idxTI });
				}
				else
				{
					stack.push({ LEX_OPERATIONS, curLine, lextable.table[i].idxTI });
				}
			}
			else
			{
				stack.push(lextable.table[i]);
			}
		}

		while (!stack.empty())
		{
			queue.push(stack.top());
			stack.pop();
		}

		int size = queue.size() + lextable_pos;

		for (int i = lextable_pos; i < size; i++)
		{
			lextable.table[i] = queue.front();
			queue.pop();
		}

		for (int i = size; lextable.table[i].lexema != LEX_SEMICOLON; i++)
		{
			lextable.table[i] = { '#', curLine, (int)LT_TI_NULLIDX };
		}

		return true;
	}
}