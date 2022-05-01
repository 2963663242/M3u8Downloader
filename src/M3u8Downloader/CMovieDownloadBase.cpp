#include "CMovieDownloadBase.h"
#include <processenv.h>
#include <errhandlingapi.h>
#include <process.h>

CMovieDownloadBase::CMovieDownloadBase()
{
	this->guid = getGuid();
}

int CMovieDownloadBase::start(bool flag1, bool flag2)
{
	char curDir[1024];
	unsigned int var_428;
	memset(curDir, 0, 1024);
	GetCurrentDirectoryA(1024, curDir);
	memset(logPath, 0, 1024);
	sprintf_s(logPath, "%s%s.mdmp", curDir, "\\Log\\Download");
	SetUnhandledExceptionFilter(lpTopLevelExceptionFilter);
	if (this->threadResult != 0) {
		return 0;
	}
	else {
		this->flag1 = flag1;
		this->flag2 = flag2;
		this->v68 = 0;
		this->v78 = 0;
		var_428 = 0;
		this->threadResult = _beginthreadex(0, 0, (_beginthreadex_proc_type)StartAddress, this, 0, &var_428);
		return !this->threadResult;
	}
	
}
