#include "stdafx.h"

namespace Log
{
	LOG Log::getlog(wchar_t logfile[])
	{
		LOG result;

		result.stream = new std::ofstream;
		result.stream->open(logfile);
		if (!result.stream->is_open())
		{
			throw ERROR_THROW(112);
		}
		wcscpy_s(result.logfile, logfile);
		return result;
	}

	void WriteLine(LOG log, const char* c, ...)
	{
		const char** ptr = &c;

		while (strlen(*ptr))
		{
			*log.stream << *(ptr++);
		}

		*log.stream << std::endl;
	}

	void WriteLine(LOG log, const wchar_t* c, ...)
	{
		const wchar_t** pstr = &c;
		char str[PARM_MAX_SIZE];
		size_t charConv = 0;

		while (wcslen(*pstr))
		{
			charConv = 0;
			wcstombs_s(&charConv, str, *(pstr++), PARM_MAX_SIZE);
			*log.stream << str;
		}

		*log.stream << std::endl;
	}

	void WriteLog(LOG log)
	{
		time_t t = time(nullptr);
		tm now;
		localtime_s(&now, &t);
		char tmChars[PARM_MAX_SIZE];
		strftime(tmChars, PARM_MAX_SIZE, "%d.%m.%Y %H:%M:%S", &now);
		*log.stream << "---- Протокол ---- Дата: " << tmChars << " ----" << std::endl;
	}

	void WriteParm(LOG log, Parm::PARM parm)
	{
		char str[PARM_MAX_SIZE];
		size_t charConv = 0;

		*log.stream << "---- Параметры ----" << std::endl;
		wcstombs_s(&charConv, str, parm.log, PARM_MAX_SIZE);
		*log.stream << "-log: " << str << std::endl;
		wcstombs_s(&charConv, str, parm.out, PARM_MAX_SIZE);
		*log.stream << "-out: " << str << std::endl;
		wcstombs_s(&charConv, str, parm.in, PARM_MAX_SIZE);
		*log.stream << "-in: " << str << std::endl;
		/*	wcstombs_s(&charConv, str, parm.IT, PARM_MAX_SIZE);
			*log.stream << "-IT: " << str << std::endl;
			wcstombs_s(&charConv, str, parm.LT, PARM_MAX_SIZE);
			*log.stream << "-LT: " << str << std::endl;*/
	}

	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "---- Исходные данные ----" << std::endl;
		*log.stream << "Количество символов: " << in.size << std::endl;
		*log.stream << "Проигнорировано: " << in.ignor << std::endl;
		*log.stream << "Количество строк: " << in.lines << std::endl;
	}

	void WriteError(LOG log, Error::ERROR error)
	{
		if (log.stream == nullptr || !log.stream->is_open())
		{
			std::cout << "Ошибка " << error.id << ": " << error.message << ", строка " << error.inext.line << ", позиция " << error.inext.col << std::endl;
		}
		else
		{
			log.stream->clear();
			*log.stream << "Ошибка " << error.id << ": " << error.message << ", строка " << error.inext.line << ", позиция " << error.inext.col << std::endl;
		}
	}

	void Close(LOG log)
	{
		if (log.stream != nullptr)
		{
			log.stream->close();
			delete log.stream;
		}
	}

	void AssembleLog(Log::LOG log, Parm::PARM parm, In::IN in)
	{
		log = Log::getlog(parm.log);
		Log::WriteLine(log, L"Тест", L" Без ошибок", L"");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);
		Log::Close(log);
	}
};
