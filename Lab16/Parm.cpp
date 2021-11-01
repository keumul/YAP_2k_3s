#include "stdafx.h"

namespace Parm
{
	PARM getparm(int argc, wchar_t* argv[])
	{
		PARM result;
		*result.in = *L"\0";
		*result.out = *L"\0";
		*result.log = *L"\0";
		*result.synt = *L"\0";
		int in_len = wcslen(PARM_IN);
		int out_len = wcslen(PARM_OUT);
		int log_len = wcslen(PARM_LOG);
		int synt_len = wcslen(PARM_SYNT);

		for (int i = 1; i < argc; ++i)
		{
			if (wcslen(argv[i]) > PARM_MAX_SIZE)
			{
				throw ERROR_THROW(104)
			}

			if (!wcsncmp(argv[i], PARM_IN, in_len))
			{
				wcscpy_s(result.in, argv[i] + in_len);
			}
			else if (!wcsncmp(argv[i], PARM_OUT, out_len))
			{
				wcscpy_s(result.out, argv[i] + out_len);
			}
			else if (!wcsncmp(argv[i], PARM_LOG, log_len))
			{
				wcscpy_s(result.log, argv[i] + log_len);
			}
			else if (!wcsncmp(argv[i], PARM_SYNT, synt_len))
			{
				wcscpy_s(result.synt, argv[i] + synt_len);
			}
		}

		if (!wcslen(result.in))
		{
			throw ERROR_THROW(100)
		}

		if (!wcslen(result.out))
		{
			wcscpy_s(result.out, result.in);
			wcscat_s(result.out, PARM_OUT_DEFAULT_EXT);
		}

		if (!wcslen(result.log))
		{
			wcscpy_s(result.log, result.in);
			wcscat_s(result.log, PARM_LOG_DEFAULT_EXT);
		}


		if (!wcslen(result.synt))
		{
			wcscpy_s(result.synt, result.in);
			wcscat_s(result.synt, PARM_PARS_DEFAULT_EXT);
		}

		return result;
	}
}