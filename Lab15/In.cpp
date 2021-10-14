#include "stdafx.h"

namespace In
{
	IN getin(wchar_t infile[], LT::LexTable& lexTable, IT::IdTable& idTable)
	{
		std::ifstream inputF(infile);

		if (!inputF.is_open())
		{
			throw ERROR_THROW(110)
		}
		IN result;
		short curLine = 0;
		short curCol = 0;
		std::string fullText;
		std::string curStr;
		std::string tmp;

		while (true)
		{
			std::getline(inputF, curStr);
			if (!inputF) break;
			result.lines++;
			fullText += curStr + IN_SEPARATOR;
		}

		int fulltext_size = fullText.size();
		result.size = fulltext_size;

		if (fulltext_size > IN_MAX_LEN_TEXT)
		{
			throw ERROR_THROW(1)
		}

		unsigned char symbol;
		size_t i = (fullText[0] == ' ') ? 1 : 0;
		fulltext_size -= (fullText[fulltext_size - 1] == ' ') ? 1 : 0;

		for (; i < fulltext_size; ++i)
		{
			symbol = fullText[i];

			switch (result.code[symbol])
			{
			case IN::T:
				tmp += symbol;
				curCol++;
				continue;
				break;

			case IN::F:
				throw ERROR_THROW_IN(111, curLine, curCol);
				break;

			case IN::I:
				result.ignor++;
				break;

			default:
				if (tmp.size() != 0)
				{
					if (Analysis::TokenAnalysis(Functions::stringToChar(tmp), lexTable, idTable, curCol - 1, curLine))
					{
						tmp.clear();
						curCol++;
						i--;
					}
					else
					{
						throw ERROR_THROW_IN(120, curLine, curCol - 1);
					}
				}
				else
				{
					switch (symbol)
					{
					case '\'':
						tmp += fullText[i++];
						for (int j = 0; fullText[i] != '\''; j++, i++)
						{
							switch (result.code[fullText[i]])
							{
							case IN::F:
								throw ERROR_THROW_IN(111, curLine, curCol);
								break;

							case IN::I:
								result.ignor++;
								continue;
								break;

							default:
								break;
							}

							if (j <= TI_STR_MAXSIZE)
							{
								tmp += fullText[i];
								curCol++;
							}
							else
							{
								throw ERROR_THROW_IN(121, curLine, curCol - 1);
							}
						}
						tmp += fullText[i];

						if (Analysis::TokenAnalysis(Functions::stringToChar(tmp), lexTable, idTable, curCol - 1, curLine))
						{
							tmp.clear();
						}
						else
						{
							throw ERROR_THROW_IN(120, curLine, curCol - 1);
						}
						break;

					case ' ':
						if (result.code[fullText[i + 1]] == IN::S || result.code[fullText[i + 1]] == IN::L ||
							result.code[fullText[i - 1]] == IN::L)
						{
							result.ignor++;
						}
						else
						{
							curCol++;
						}
						break;

					case IN_SEPARATOR:
						curLine++;
						curCol = 0;
						break;

					default:
						tmp += symbol;
						if (Analysis::TokenAnalysis(Functions::stringToChar(tmp), lexTable, idTable, curCol - 1, curLine))
						{
							tmp.clear();
						}
						else
						{
							throw ERROR_THROW_IN(120, curLine, curCol - 1);
						}
						break;
					}
				}
				break;
			}
		}

		return result;
	}
}