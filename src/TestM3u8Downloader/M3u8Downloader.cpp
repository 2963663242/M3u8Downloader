
#include <iostream>
#include <map>
#include <Windows.h>
#include "utils.h"
#include "M3u8DownloaderHook.h"
#include "hookmiddle.h"
#include<string>

using namespace std;

CM3u8Download * (*MovieDLCreate)(void);
void (*MovieDLSetUrl)(ULONGLONG, const char*);
void (*MovieDLSetSavePath)(ULONGLONG, const char*);
void (*MovieDLSetCookie)(ULONGLONG, const char*);
void (*MovieDLSetHeader)(ULONGLONG, const char*, const char*);
void (*MovieDLStart)(ULONGLONG, bool, bool);
const char* (*MovieDLGetState)(ULONGLONG);
int main()
{
	HMODULE dllDownloader = LoadLibrary(TEXT("M3u8Downloader.dll"));
	hookinit();

	if (dllDownloader == NULL)
		return -1;
	MovieDLCreate = (CM3u8Download *(*)(void))GetProcAddress(dllDownloader, "MovieDLCreate");
	MovieDLSetUrl = (void (*)(ULONGLONG, const char*))GetProcAddress(dllDownloader, "MovieDLSetUrl");
	MovieDLSetSavePath = (void (*)(ULONGLONG, const char*))GetProcAddress(dllDownloader, "MovieDLSetSavePath");
	MovieDLSetCookie = (void (*)(ULONGLONG, const char*))GetProcAddress(dllDownloader, "MovieDLSetCookie");
	MovieDLSetHeader = (void (*)(ULONGLONG, const char*, const char*))GetProcAddress(dllDownloader, "MovieDLSetHeader");
	MovieDLStart = (void (*)(ULONGLONG, bool, bool))GetProcAddress(dllDownloader, "MovieDLStart");
	MovieDLGetState = (const char* (*)(ULONGLONG))GetProcAddress(dllDownloader, "MovieDLGetState");

	CM3u8Download * instance = 0;
	const char * strUrl = "http://devimages.apple.com/iphone/samples/bipbop/bipbopall.m3u8";
	//const char * strUrl = "https://vkceyugu.cdn.bspapp.com/VKCEYUGU-uni4934e7b/c4d93960-5643-11eb-a16f-5b3e54966275.m3u8";
	//const char * strUrl = "https://vkceu.cdn.bspapp.com/VKCEYUGU-uni4934e7b/c4d93960-5643-11eb-a16f-5b3e5466275.m3u8"; //error url
	const char* strSavePath = "1.mp4";
	const char* strCookie = "";
	map<string, string> http_headers;
	//http_headers[string("User-Agent")] = string("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.116 Safari/537.36");

	const char* data=0;
	int a = sizeof(string);
	ALogInit((char *)"./log.txt");
	instance = MovieDLCreate();
	M3u8DownloaderHook::MovieDLSetUrl((CM3u8Download *)instance, strUrl);
	M3u8DownloaderHook::MovieDLSetSavePath((CM3u8Download*)instance,strSavePath);
	M3u8DownloaderHook::MovieDLSetCookie((CM3u8Download*)instance, strCookie);

	map<string, string>::iterator iter;
	for (iter = http_headers.begin(); iter != http_headers.end(); iter++) {

		M3u8DownloaderHook::MovieDLSetHeader((CM3u8Download*)instance, iter->first.c_str(), iter->second.c_str());
	
	}

	M3u8DownloaderHook::MovieDLStart((CM3u8Download*)instance, true, false);
	//MovieDLStart(instance, true, false);
	do {
		Sleep(1000);

		data = M3u8DownloaderHook::MovieDLGetState((CM3u8Download*)instance);

	} while (!strstr(data, "download_complete") && !strstr(data, "{event=download_error}"));

	ALogFree();
	return 0;
}

