#pragma once
#include <curl/curl.h>
#include "CMovieDownloadBase.h"
#include "utils.h"


__int64* sub_180009CC0();

class CM3u8Download: public CMovieDownloadBase
{
public:
	CM3u8Download();
	void download();
	CURLcode  downloadSegment(string& url,string * pCookie,string *pReceiveData);
public:
	int time; //D0
	int dlnow; //D4
	int invalidDLCount; //D8
	bool dlBody; //DC
	CURL* pCurl; //E0
	curl_slist * headerchain; //E8
};

