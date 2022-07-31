#include "CMovieDownloadBase.h"
#include "log.h"

_beginthreadex_proc_type StartAddress =(_beginthreadex_proc_type)0x000000018000B290;

CMovieDownloadBase::CMovieDownloadBase()
{
	this->guid = this->getGuid();
}
CMovieDownloadBase::~CMovieDownloadBase()
{
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
	//this->vft = (void*)0x00000001800A19A0;
	this->threadResult = 0;
	this->v10 = 0;
	this->v18 = 0;
	this->dsSavePath = 0;
	this->savePath = 0;
	this->infoSavePath = 0;
	this->url = 0;
	this->strCookie = 0;
	this->v48 = 0;
	this->useragent = 0;
	this->v58 = 0;
	this->speedLimit = 0;
	this->flag1 = 0;
	this->flag2 = 0;
	this->v68 = 0;
	this->v70 = 0;
	this->v78 = 0;
	this->v7C = 0;
	this->guid = "";
	this->state = "";
	
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
	sprintf(logPath, "%s%s.mdmp", curDir, "\\Log\\Download");
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

void CMovieDownloadBase::setCallbackState(stateCallback* stateInfo)
{
	string state = "";
	char Buffer[MAX_PATH];
	if (stateInfo->type == 0) {
		state = "{event=MDEVENT_INIT";
	}
	else if (stateInfo->type == 1) {
		state = "{event=download_error}";
	}
	else if (stateInfo->type == 2) {
		state = "{event=download_connecting";
	}
	else if (stateInfo->type == 3) {
		sprintf(Buffer, "{event=downloading, totoalSize=%lld, downloadingSize=%lld, }", stateInfo->totalSize, stateInfo->downloadingSize);
		state = Buffer;
	}
	else if (stateInfo->type == 4) {
		state = "{event=download_complete}";
	}
	long long * p = (__int64*)&this->state;
	
	*p = 0;
	this->state = state;
	LogD(LogLevel::Info, "callback %s", state.c_str());
	if (this->v10 != 0 && this->v78 == 0) {
		this->v10((char *)state.c_str());
	}
}


