#pragma once

#include <fstream>
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <locale>
#include <cwchar>
#include <string>
#include <ctime>
#include <chrono>
#include <string.h>
#include <stack>
#include <queue>

#include "Functions.h"			//	вспомогательные функции
#include "Error.h"				//	обработка ошибок
#include "Parm.h"				//	обработка параметров
#include "LT.h"					//  таблица лексем
#include "IT.h"					//  таблица идентификаторов
#include "In.h"					//	ввод исходного файла
#include "Log.h"				//	работа с протоколом
#include "FST.h"				//  КА
#include "Lex.h"		    	//  лексический анализ
#include "Polish.h"		        //  польская запись
#include "Greibach.h"
#include "MFST.h"