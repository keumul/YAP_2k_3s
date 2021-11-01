#pragma once

#define	PARM_IN	L"-in:"		//	ключ	для	файла	исходного кода
#define	PARM_OUT L"-out:"	//	ключ	для	файла	объектного кода
#define	PARM_LOG L"-log:"	//	ключ	для	файла	журнала
#define	PARM_SYNT L"-synt:"	//	ключ	для	файла	дерева разбора
#define PARM_MAX_SIZE 300	// максимальная длинна строки параметра
#define PARM_OUT_DEFAULT_EXT L".out"	// расширение файла объектного кода по умолчанию
#define PARM_LOG_DEFAULT_EXT L".log"	// расширение файла протокола по умолчанию
#define PARM_PARS_DEFAULT_EXT L".synt"	// расширение файла дерева разбора по умолчанию

namespace Parm	// обработка входных параметров
{
	struct PARM	// входные параметры
	{
		wchar_t in[PARM_MAX_SIZE];	//	-in:	имя	файла	исходного кода
		wchar_t out[PARM_MAX_SIZE];	//	-out:	имя	файла	объектного кода
		wchar_t log[PARM_MAX_SIZE];	//	-log:	имя	файла	протокола
		wchar_t synt[PARM_MAX_SIZE];//	-pars:	имя	файла	лексем
	};
	//int _tmain(int arg, _TCHAR* argv[]);
	PARM getparm(int argc, wchar_t* argv[]); // сфрмировать struct PARM на основе параметров функции main
}