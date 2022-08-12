
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
	//const char * strUrl = "https://manifest.googlevideo.com/api/manifest/hls_variant/expire/1660342265/ei/mXv2YpCQOJGGs8IP8qGLwAs/ip/2406%3Ada14%3A918%3A8901%3A3833%3A3a14%3A3b74%3Abcca/id/BkTDF-wKQvo.1~24228425/source/yt_live_broadcast/requiressl/yes/tx/24210080/txs/24174780%2C24174781%2C24174782%2C24174783%2C24174805%2C24210080/hfr/1/playlist_duration/30/manifest_duration/30/maudio/1/gcr/jp/spc/lT-Khofc9wZJy7aZ9y0B8Acnm7IsnpY/vprv/1/go/1/pacing/0/nvgoi/1/keepalive/yes/fexp/24001373%2C24007246/dover/11/itag/0/playlist_type/DVR/sparams/expire%2Cei%2Cip%2Cid%2Csource%2Crequiressl%2Ctx%2Ctxs%2Chfr%2Cplaylist_duration%2Cmanifest_duration%2Cmaudio%2Cgcr%2Cspc%2Cvprv%2Cgo%2Citag%2Cplaylist_type/sig/AOq0QJ8wRQIgHtJqT4AHkFNSONW86CjbJL-H_hTsCwFaV3_f3MDDDDcCIQDOxBeVyI51hrQhq1WiaSzozLNCnYEPKZ2S1SIE6kGNDw%3D%3D/file/index.m3u8";
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

