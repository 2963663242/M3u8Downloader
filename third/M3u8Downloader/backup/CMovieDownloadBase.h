#pragma once
#include <iostream>
#include "utils.h"
using namespace std;
class CMovieDownloadBase
{

public:

	CMovieDownloadBase();
	virtual int start(bool flag1,bool flag2);

public:

	uintptr_t threadResult;
	long long int v10;
	long long int v18;
	char * dsSavePath;
	char * savePath;
	char * infoSavePath;
	char * url;
	char * strCookie;
	long long int v48;
	long long int v50;
	long long int v58;
	int v60;
	bool flag1;
	bool flag2;
	long long int v68;
	long long int v70;
	 int v78;
	 int v7C;
	 string guid;
	 char v88;
	 long long int v98;
	 string vA8;
	char vB0;
	long long int vC0;
};

//extern  LONG (WINAPI* lpTopLevelExceptionFilter)(
//		_In_ struct _EXCEPTION_POINTERS* ExceptionInfo
//		);