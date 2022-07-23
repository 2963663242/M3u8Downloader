
#include <iostream>
#include <map>
#include <thread>
#include <Windows.h>
#include "utils.h"

using namespace std;

ULONGLONG (*MovieDLCreate)(void);
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
	MovieDLCreate = (ULONGLONG(*)(void))GetProcAddress(dllDownloader, "MovieDLCreate");
	MovieDLSetUrl = (void (*)(ULONGLONG, const char*))GetProcAddress(dllDownloader, "MovieDLSetUrl");
	MovieDLSetSavePath = (void (*)(ULONGLONG, const char*))GetProcAddress(dllDownloader, "MovieDLSetSavePath");
	MovieDLSetCookie = (void (*)(ULONGLONG, const char*))GetProcAddress(dllDownloader, "MovieDLSetCookie");
	MovieDLSetHeader = (void (*)(ULONGLONG, const char*, const char*))GetProcAddress(dllDownloader, "MovieDLSetHeader");
	MovieDLStart = (void (*)(ULONGLONG, bool, bool))GetProcAddress(dllDownloader, "MovieDLStart");
	MovieDLGetState = (const char* (*)(ULONGLONG))GetProcAddress(dllDownloader, "MovieDLGetState");

	ULONGLONG instance = 0;
	const char * strUrl = "http://devimages.apple.com/iphone/samples/bipbop/bipbopall.m3u8";
	const char* strSavePath = "1.mp4";
	const char* strCookie = "";
	map<string, string> http_headers = { 
		{"User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.116 Safari/537.36"}, 
		{"Accept-Charset" , "ISO-8859-1,utf-8;q=0.7,*;q=0.7"},
		{"Accept" ,"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"},
		{"Accept-Language" , "en-us,en;q=0.5"},
};
	const char* data=nullptr;


	instance = MovieDLCreate();
	MovieDLSetUrl(instance, strUrl);
	MovieDLSetSavePath(instance,strSavePath);
	MovieDLSetCookie(instance, strCookie);

	map<string, string>::iterator iter;
	for (iter = http_headers.begin(); iter != http_headers.end(); iter++) {

		MovieDLSetHeader(instance, iter->first.c_str(), iter->second.c_str());
	}

	MovieDLStart(instance, true, false);
	do {
		std::this_thread::sleep_for(chrono::milliseconds(2000));

		data = MovieDLGetState(instance);

	} while (!strstr(data, "download_complete"));


	return 0;
}

