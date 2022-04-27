
#include <iostream>
#include <map>
#include <thread>
#include <Windows.h>
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
	const char * strUrl = "https://manifest.googlevideo.com/api/manifest/hls_playlist/expire/1649714360/ei/WFBUYsryAcrS2roP5LaYiAU/ip/103.149.248.4/id/5nNSbadX-Xc.1/itag/301/source/yt_live_broadcast/requiressl/yes/ratebypass/yes/live/1/sgoap/gir%3Dyes%3Bitag%3D140/sgovp/gir%3Dyes%3Bitag%3D299/hls_chunk_host/rr3---sn-i3b7knsd.googlevideo.com/playlist_duration/30/manifest_duration/30/vprv/1/playlist_type/DVR/initcwndbps/16930/mh/2k/mm/44/mn/sn-i3b7knsd/ms/lva/mv/m/mvi/3/pl/24/dover/11/pacing/0/keepalive/yes/fexp/24001373,24007246/mt/1649692602/sparams/expire,ei,ip,id,itag,source,requiressl,ratebypass,live,sgoap,sgovp,playlist_duration,manifest_duration,vprv,playlist_type/sig/AOq0QJ8wRQIhAIr9QrKIrHEEpMCLuhlIT0fXQ7r-v165v5XA8GnncCUNAiBTGiLI8R1rmGkC3gh0LNEnoZbLT2PudOHd8vNKdKqrxQ%3D%3D/lsparams/hls_chunk_host,initcwndbps,mh,mm,mn,ms,mv,mvi,pl/lsig/AG3C_xAwRQIgKNrDQqNKeGe7O4AryP6xaA7pPIy33FBPWaJY_HaH3VYCIQD0FO72mvPbn9BDnqBOJqAes61bLLanorR3TMJu-ZUVsw%3D%3D/playlist/index.m3u8";
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

