#pragma once
#include "utils.h"
#include <process.h>
#include <processenv.h>
#include <errhandlingapi.h>
#include <iostream>

using namespace std;

struct stateCallback;

class CMovieDownloadBase
{

public:
	 CMovieDownloadBase();
	virtual ~CMovieDownloadBase();
	 CMovieDownloadBase *CMovieDownloadBaseEx();
	 int start(bool flag1,bool flag2);
	 void setCallbackState(stateCallback * stateInfo);
	
	string getGuid();

public:
	//void *vft;
	uintptr_t threadResult;
	void  (*v10)(char *);
	long long int v18;
	char * dsSavePath;
	char * savePath;
	char * infoSavePath;
	char * url;
	char * strCookie;
	long long int v48;
	char * useragent;
	long long int v58;
	int speedLimit;
	bool flag1;
	bool flag2;
	long long int v68;
	long long int v70;
	 int v78;
	 int v7C;
	 string guid;
	 string state;
};

extern _beginthreadex_proc_type  StartAddress;

struct stateCallback
{
	int type;
	int nkn_4;
	__int64 nkn_8;
	__int64 downloadingSize;
	__int64 totalSize;
};
