#pragma once
#define SIZE_INTEGRAL_ID	2

namespace Analysis
{
	struct TypeOfVariable
	{
		int LT_position = -1;
		enum
		{
			UNDEF = -1,
			INT = 0,
			STRING = 1
		} type_of_variable;
	};

	IT::Entry ÑonstructorITEntry(char* parentBlockName, char* id, IT::IDTYPE idtype, TypeOfVariable type);
	bool TokenAnalysis(char* token, LT::LexTable& lexTable, IT::IdTable& idTable, int curCol, int curLine);
	bool IdAnalysis(char* token, LT::LexTable& lexTable, IT::IdTable& idTable, TypeOfVariable& type, int curCol, int curLine);
}