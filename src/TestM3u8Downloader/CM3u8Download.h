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
	void* vD0;
	int vD8;
	bool vDC;
	CURL* pCurl; 
	curl_slist * headerchain;
};

