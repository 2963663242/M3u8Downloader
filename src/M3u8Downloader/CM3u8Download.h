#pragma once
#include "CMovieDownloadBase.h"
class CM3u8Download: public CMovieDownloadBase
{
public:
	CM3u8Download();

public:
	int vD8;
	char vDC = 1;
	long long int E8;
};

