#pragma once
#include "CMovieDownloadBase.h"
#include "utils.h"



class CM3u8Download: public CMovieDownloadBase
{
public:
	CM3u8Download();

public:
	void* vD0;
	int vD8;
	char vDC = 1;
	void* vE0;
	headerchain* headerchain;
};

