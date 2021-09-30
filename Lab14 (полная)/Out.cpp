#include "stdafx.h"
#include "Parm.h"
#include "In.h"
using namespace std;

void Outout(Parm::PARM parm, In::IN in)
{
	if (parm.in != parm.out)
	{
		ofstream out(parm.out);
		for (int i = 0; i < in.size; i++)
		{
			if (in.text[i] != '|')
				out << in.text[i];
			else
			{
				out << in.text[i] << "\n";
			}
		}
		out.close();
	}
}