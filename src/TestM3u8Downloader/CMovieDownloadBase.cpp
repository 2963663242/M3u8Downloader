#include "CMovieDownloadBase.h"
#include "utils.h"
#include <processenv.h>
#include <errhandlingapi.h>
#include <process.h>

CMovieDownloadBase::CMovieDownloadBase()
{
	this->guid = this->getGuid();
}
string CMovieDownloadBase::getGuid() {
	char buffer[64] = { 0, };
	GUID pguid;

	CoCreateGuid(&pguid);

	sprintf_s(buffer, 64, "{%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X}", pguid.Data1,
		pguid.Data2,
		pguid.Data3,
		pguid.Data4[0],
		pguid.Data4[1],
		pguid.Data4[2],
		pguid.Data4[3],
		pguid.Data4[4],
		pguid.Data4[5],
		pguid.Data4[6],
		pguid.Data4[7]);

	return buffer;
}
CMovieDownloadBase* CMovieDownloadBase::CMovieDownloadBaseEx()
{
 this->guid = this->getGuid();
	return this;
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
	//测试lpTopLevelExceptionFilter函数
	// int * a = 0;
	// *a = 1;
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
