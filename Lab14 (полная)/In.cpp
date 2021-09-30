#include "stdafx.h"
#include "In.h"
#include "Error.h"
#include "Parm.h"

using namespace std;
namespace In
{
	IN getin(Parm::PARM parm)
	{
		IN in;
		in.size = 0; 
		in.lines = 0;
		in.ignor = 0; 
		int col = 0, ksum = 0;

		char* text = new char[IN_MAX_LEN_TEXT];

		ifstream file(parm.in);
		if (file.fail()) {throw ERROR_THROW(110);}
		int lines = 0;

		while (in.size < IN_MAX_LEN_TEXT)
		{
			char x;
			file.get(x);
			unsigned char y = x;
			if (file.eof())
			{
				text[in.size] = IN_CODE_ENDS;
				in.lines++;
				break;
			}
			switch (in.code[y])
			{
			case in.K: {
				{
					ksum++;
					text[in.size] = y;
					in.size++;
					col++;
				}
			} break;
			case in.S: {
				
				if (in.code[text[in.size - 1]] != in.S)
				{
					text[in.size] = '|';
					in.size++;
					in.lines++;
					col = 0;
				}
			} break;
			
			case in.T: {
				text[in.size] = y;
-				in.size++;
				col++;
			} break;
			case in.L: {
				if (in.code[text[in.size - 1]] == in.P)
				{
					text[in.size - 1] = y;
				}
				else
				{
					text[in.size] = y;
					in.size++;
				}
				col++;
			} break;
			case in.P: {
				if (in.code[text[in.size - 1]] != in.L && in.code[text[in.size - 1]] != in.P)
				{
					text[in.size] = x;
					in.size++;
				}
				col++;
				if (ksum == 2 || ksum == 0)
				{
					if (text[in.size - 1] != ' ') { text[in.size++] = ' '; }
					if (x == ' ') { continue; }

					if (ksum == 2) { ksum = 0; }

					text[in.size] = y;
					in.size++;

					text[in.size] = IN_CODE_ENDL;
					in.size++;
					col++;
				}
				else
				{
					text[in.size] = y;
					in.size++;
					col++;
				}
			} break;
			case in.I: { in.ignor++; } break;
			case in.F: { throw ERROR_THROW_IN(111, in.lines, col); } break;
			default: {
				text[in.size] = in.code[y];
				in.size++;
				col++;}
			}
		}
		file.close();
		in.text = text;

		return in;
	}
}