#include "stdafx.h"

using namespace In;
using namespace LT;
using namespace IT;
using namespace std;

namespace Analysis
{
	std::string integralID[SIZE_INTEGRAL_ID] = { "strlen", "substr" };	// ìàññèâ ñòàíäàðòíûõ èäåíòèôèêàòîðîâ
	TypeOfVariable tov;

	IT::Entry ÑonstructorITEntry(char* parentBlockName, char* id, IT::IDTYPE idtype, TypeOfVariable type)
	{
		IT::Entry entry;

		entry.idtype = idtype;
		switch (type.type_of_variable)
		{
		case TypeOfVariable::INT:
			entry.iddatatype = INT;
			break;

		case TypeOfVariable::STRING:
			entry.iddatatype = STR;
			break;
		}
		strcpy_s(entry.id, id);
		strcpy_s(entry.parentBlockName, parentBlockName);

		return entry;
	}

	bool TokenAnalysis(char* token, LexTable& lexTable, IdTable& idTable, int curCol, int curLine)
	{
		FST::FST* auto_value;
		IT::Entry entry;

		switch (token[0])
		{
		case LEX_SEMICOLON:
			LT::Add(lexTable, { LEX_SEMICOLON, curLine, (int)LT_TI_NULLIDX });
			break;

		case LEX_COMMA:
			LT::Add(lexTable, { LEX_COMMA, curLine, (int)LT_TI_NULLIDX });
			break;

		case LEX_LEFTBRACE:
			LT::Add(lexTable, { LEX_LEFTBRACE, curLine, (int)LT_TI_NULLIDX });
			break;

		case LEX_RIGHTBRACE:
			LT::Add(lexTable, { LEX_RIGHTBRACE, curLine, (int)LT_TI_NULLIDX });
			break;

		case LEX_LEFTSQBRACE:
			LT::Add(lexTable, { LEX_LEFTSQBRACE, curLine, (int)LT_TI_NULLIDX });
			break;

		case LEX_RIGHTSQBRACE:
			LT::Add(lexTable, { LEX_RIGHTSQBRACE, curLine, (int)LT_TI_NULLIDX });
			break;

		case LEX_LEFTHESIS:
			LT::Add(lexTable, { LEX_LEFTHESIS, curLine, (int)LT_TI_NULLIDX });
			break;

		case LEX_RIGHTHESIS:
			LT::Add(lexTable, { LEX_RIGHTHESIS, curLine, (int)LT_TI_NULLIDX });
			break;

		case LEX_EQUAL:
			LT::Add(lexTable, { LEX_EQUAL, curLine, (int)LT_TI_NULLIDX });
			break;

		case '+':
			strcpy_s(entry.id, token);
			entry.idtype = O;
			IT::Add(idTable, entry);
			LT::Add(lexTable, { LEX_PLUS, curLine, idTable.current_size - 1 });
			break;

		case '-':
			strcpy_s(entry.id, token);
			entry.idtype = O;
			IT::Add(idTable, entry);
			LT::Add(lexTable, { LEX_MINUS, curLine, idTable.current_size - 1 });
			break;

		case '*':
			strcpy_s(entry.id, token);
			entry.idtype = O;
			IT::Add(idTable, entry);
			LT::Add(lexTable, { LEX_STAR, curLine, idTable.current_size - 1 });
			break;

		case '/':
			strcpy_s(entry.id, token);
			entry.idtype = O;
			IT::Add(idTable, entry);
			LT::Add(lexTable, { LEX_DIRSLASH, curLine, idTable.current_size - 1 });
			break;

		case 'i':
			auto_value = new FST::FST(INTEGER(token));
			if (FST::execute(*auto_value))
			{
				LT::Add(lexTable, { LEX_INTEGER, curLine, (int)LT_TI_NULLIDX });
				delete auto_value;
				auto_value = NULL;
				tov.LT_position = lexTable.current_size - 1;
				tov.type_of_variable = TypeOfVariable::INT;
			}
			else
			{
				delete auto_value;
				auto_value = NULL;
				return IdAnalysis(token, lexTable, idTable, tov, curCol, curLine);
			}
			break;

		case 'f':
			auto_value = new FST::FST(FUNCTION(token));
			if (FST::execute(*auto_value))
			{
				LT::Add(lexTable, { LEX_FUNCTION, curLine, (int)LT_TI_NULLIDX });
				delete auto_value;
				auto_value = NULL;
			}
			else
			{
				delete auto_value;
				auto_value = NULL;
				return IdAnalysis(token, lexTable, idTable, tov, curCol, curLine);
			}
			break;

		case 'd':
			auto_value = new FST::FST(DECLARE(token));
			if (FST::execute(*auto_value))
			{
				LT::Add(lexTable, { LEX_DECLARE, curLine, (int)LT_TI_NULLIDX });
				delete auto_value;
				auto_value = NULL;
			}
			else
			{
				delete auto_value;
				auto_value = NULL;
				return IdAnalysis(token, lexTable, idTable, tov, curCol, curLine);
			}
			break;

		case 'r':
			auto_value = new FST::FST(RETURN(token));
			if (FST::execute(*auto_value))
			{
				LT::Add(lexTable, { LEX_RETURN, curLine, (int)LT_TI_NULLIDX });
				delete auto_value;
				auto_value = NULL;
			}
			else
			{
				delete auto_value;
				auto_value = NULL;
				return IdAnalysis(token, lexTable, idTable, tov, curCol, curLine);
			}
			break;

		case 's':
			auto_value = new FST::FST(STRING(token));
			if (FST::execute(*auto_value))
			{
				LT::Add(lexTable, { LEX_STRING, curLine, (int)LT_TI_NULLIDX });
				delete auto_value;
				auto_value = NULL;
				tov.LT_position = lexTable.current_size - 1;
				tov.type_of_variable = TypeOfVariable::STRING;
			}
			else
			{
				delete auto_value;
				auto_value = NULL;
				return IdAnalysis(token, lexTable, idTable, tov, curCol, curLine);
			}
			break;

		case 'p':
			auto_value = new FST::FST(PRINT(token));
			if (FST::execute(*auto_value))
			{
				LT::Add(lexTable, { LEX_PRINT, curLine, (int)LT_TI_NULLIDX });
				delete auto_value;
				auto_value = NULL;
			}
			else
			{
				delete auto_value;
				auto_value = NULL;
				return IdAnalysis(token, lexTable, idTable, tov, curCol, curLine);
			}
			break;

		case '\'':
			auto_value = new FST::FST(STRING_LITERAL(token));
			if (FST::execute(*auto_value))
			{
				int tempIndex = IT::IsLiteral(idTable, token);
				if (tempIndex == TI_NULLIDX)
				{
					IT::Entry entry;
					entry.idtype = L;
					entry.idxfirstLE = idTable.current_literals++;
					entry.iddatatype = STR;
					strcpy_s(entry.id, "");
					strcpy_s(entry.parentBlockName, "");
					IT::Add(idTable, entry);
					idTable.table[idTable.current_size - 1].value.vstr->len = 0;

					for (int i = 0; token[i] != '\0'; i++)
					{
						if (token[i] != '\'')
						{
							idTable.table[idTable.current_size - 1].value.vstr->str[idTable.table[idTable.current_size - 1].value.vstr->len] = token[i];
							idTable.table[idTable.current_size - 1].value.vstr->len++;
						}
					}
					idTable.table[idTable.current_size - 1].value.vstr->str[idTable.table[idTable.current_size - 1].value.vstr->len] = '\0';
					LT::Add(lexTable, { LEX_LITERAL, curLine, idTable.current_size - 1 });
				}
				else
				{
					LT::Add(lexTable, { LEX_LITERAL, curLine, tempIndex });
				}

				delete auto_value;
				auto_value = NULL;
			}
			else
			{
				delete auto_value;
				auto_value = NULL;
				return IdAnalysis(token, lexTable, idTable, tov, curCol, curLine);
			}
			break;

		default:
			auto_value = new FST::FST(INTEGER_LITERAL(token));
			if (FST::execute(*auto_value))
			{
				int tempIndex = IT::IsLiteral(idTable, atoi(token));
				if (tempIndex == TI_NULLIDX)
				{
					IT::Entry entry;
					entry.idtype = L;
					entry.idxfirstLE = idTable.current_literals++;
					entry.iddatatype = INT;
					strcpy_s(entry.id, "");
					strcpy_s(entry.parentBlockName, "");
					IT::Add(idTable, entry);
					idTable.table[idTable.current_size - 1].value.vint = atoi(token);
					LT::Add(lexTable, { LEX_LITERAL, curLine, idTable.current_size - 1 });
				}
				else
				{
					LT::Add(lexTable, { LEX_LITERAL, curLine, tempIndex });
				}

				delete auto_value;
				auto_value = NULL;
			}
			else
			{
				delete auto_value;
				auto_value = NULL;
				return IdAnalysis(token, lexTable, idTable, tov, curCol, curLine);
			}
			break;
		}

		return true;
	}

	bool IdAnalysis(char* token, LT::LexTable& lexTable, IT::IdTable& idTable, TypeOfVariable& type, int curCol, int curLine)
	{
		FST::FST* identificator = new FST::FST(IDENTIFICATOR(token));

		if (FST::execute(*identificator))
		{
			if (strcmp(token, IND_MAIN) == 0 || ((GetEntry(lexTable, lexTable.current_size - 1).lexema ==
				LEX_FUNCTION && GetEntry(lexTable, lexTable.current_size - 2).lexema == LEX_VARIABLE) &&
				type.LT_position == lexTable.current_size - 2))
			{
				if (IsId(idTable, token) == TI_NULLIDX)
				{
					IT::Add(idTable, ÑonstructorITEntry((char*)"", token, F, type));

					if (strcmp(token, IND_MAIN) == 0)
					{
						LT::Add(lexTable, { LEX_MAIN, curLine, idTable.current_size - 1 });
					}
					else
					{
						LT::Add(lexTable, { LEX_ID, curLine, idTable.current_size - 1 });
					}

					type.LT_position = -1;
					type.type_of_variable = TypeOfVariable::UNDEF;
				}
				else
				{
					throw ERROR_THROW_IN(123, curLine, curCol);
				}
			}
			else if (lexTable.table[lexTable.current_size - 1].lexema == LEX_VARIABLE && lexTable.table[lexTable.current_size - 2].lexema == LEX_DECLARE && type.LT_position == lexTable.current_size - 1)
			{
				bool flag_for_brace = false;
				for (int i = lexTable.current_size - 1; i > 0; i--)
				{
					if (lexTable.table[i].lexema == LEX_LEFTBRACE)
					{
						flag_for_brace = true;
					}

					if (lexTable.table[i].idxTI != TI_NULLIDX && flag_for_brace && GetEntry(idTable, lexTable.table[i].idxTI).idtype == IT::IDTYPE::F)
					{
						if (IsId(idTable, token) == TI_NULLIDX || idTable.table[IsId(idTable, token)].idtype != V || idTable.table[IsId(idTable, token)].parentBlockName != idTable.table[lexTable.table[i].idxTI].id)
						{
							IT::Add(idTable, ÑonstructorITEntry(idTable.table[lexTable.table[i].idxTI].id, token, V, type));
							LT::Add(lexTable, { LEX_ID, curLine, idTable.current_size - 1 });
							type.LT_position = -1;
							type.type_of_variable = TypeOfVariable::UNDEF;
							break;
						}
						else
						{
							throw ERROR_THROW_IN(126, curLine, curCol);
						}
					}
				}
			}
			else if (GetEntry(lexTable, lexTable.current_size - 1).lexema == LEX_VARIABLE && type.LT_position == lexTable.current_size - 1)
			{
				for (int i = lexTable.current_size - 1; i > 0; i--)
				{
					if (lexTable.table[i].lexema == LEX_ID && idTable.table[lexTable.table[i].idxTI].idtype == IT::IDTYPE::F)
					{
						if (GetEntry(lexTable, i - 1).lexema == LEX_FUNCTION && GetEntry(lexTable, i - 2).lexema == LEX_VARIABLE)
						{
							IT::Add(idTable, ÑonstructorITEntry(idTable.table[lexTable.table[i].idxTI].id, token, P, type));
							LT::Add(lexTable, { LEX_ID, curLine, idTable.current_size - 1 });
							type.LT_position = -1;
							type.type_of_variable = TypeOfVariable::UNDEF;
							break;
						}
					}
				}
			}
			else
			{
				bool isLeftBraceWas = false;

				for (int i = lexTable.current_size - 1; i > 0; i--)
				{
					if (GetEntry(lexTable, i).lexema == LEX_LEFTBRACE)
					{
						isLeftBraceWas = true;
					}

					if (isLeftBraceWas && lexTable.table[i].lexema == LEX_ID && idTable.table[lexTable.table[i].idxTI].idtype == IT::IDTYPE::F)
					{
						int tempIndex = IsId(idTable, token);

						if (tempIndex != TI_NULLIDX)
						{
							LT::Add(lexTable, { LEX_ID, curLine, tempIndex });
							break;
						}
						else
						{
							throw ERROR_THROW_IN(129, curLine, curCol);
						}

					}
				}
			}

			delete identificator;
			identificator = NULL;
			return true;
		}
		else
		{
			delete identificator;
			identificator = NULL;
			return false;
		}
	}
}