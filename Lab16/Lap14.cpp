#include "stdafx.h"

int wmain(int argc, wchar_t* argv[])
{
	setlocale(LC_ALL, "rus");

	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		LT::LexTable lexTable = LT::Create(300);
		IT::IdTable	idTable = IT::Create(100);
		In::IN in = In::getin(parm.in, lexTable, idTable);

		/*PN::PolishNotation(15, lexTable, idTable);
		PN::PolishNotation(60, lexTable, idTable);
		PN::PolishNotation(116, lexTable, idTable);*/

		LT::LexTablePrint(lexTable, parm.out);
		IT::PrintIdTable(idTable, parm.log);

		MFST::Mfst mfst(lexTable, GRB::getGreibach(), parm.synt);
		mfst.start();
		mfst.savededucation();
		mfst.printRules();

		LT::Delete(lexTable);
		IT::Delete(idTable);
		Log::AssembleLog(log, parm, in);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
	}

	return 0;
}