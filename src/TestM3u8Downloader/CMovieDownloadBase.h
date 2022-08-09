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
	 CMovieDownloadBase *CMovieDownloadBaseEx();
	 int start(bool flag1,bool flag2);
	 void setCallbackState(stateCallback * stateInfo);
	
	string getGuid();

public:
	//void *vft;
	uintptr_t threadResult; //8
	void  (*v10)(char *);  //10
	long long int v18;   //18
	char * dsSavePath;
	char * savePath;
	char * infoSavePath;
	char * url;
	char * strCookie;//40
	long long int v48; 
	char * useragent;
	long long int v58;
	int speedLimit; 
	bool flag1;  //64
	bool flag2;  //65
	long long int downloadedSize; //68
	long long int totalSize; //70
	 int v78; 
	 int stateType;
	 string guid; //80
	 string state; //A8
};

extern _beginthreadex_proc_type  StartAddress;

struct stateCallback
{
	int type;
	__int64 speed;
	__int64 downloadingSize;
	__int64 totalSize;
};
