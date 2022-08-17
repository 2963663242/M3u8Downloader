#pragma once
#include "utils.h"
#include <process.h>
#include <iostream>

typedef  unsigned (__stdcall *_beginthreadex_proc_type) (void *);

using namespace std;



struct stateCallback;

class CMovieDownloadBase
{

public:
	 CMovieDownloadBase();
	virtual ~CMovieDownloadBase();
	virtual int start(bool flag1,bool flag2);
	virtual void stop();
	virtual void wait();
	void setCallbackState(stateCallback  stateInfo);
	
	string getGuid();

public:

	uintptr_t threadResult; //8
	void  (*callback)(char *);  //10
	long long int v18;   //18
	char * dsSavePath; //20
	char * savePath; // 28
	char * infoSavePath;//30
	char * url;//38
	char * strCookie;//40
	long long int v48; 
	char * useragent;//50
	char * v58; 
	int speedLimit; 
	bool flag1;  //64
	bool flag2;  //65
	long long int downloadedSize; //68
	long long int totalSize; //70
	 int stopFlag; //78
	 int stateType;
	 string guid; //80
	 string state; //A8
};



struct stateCallback
{
	int type;
	__int64 speed;
	__int64 downloadingSize;
	__int64 totalSize;
};
