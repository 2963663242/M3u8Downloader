//// M3u8Downloader.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
////
//
//#include <iostream>
//#include <cassert>
//#include "CM3u8Download.h"
//#include <map>
//CM3u8Download * MovieDLCreate() {
//    CM3u8Download* downloader = new CM3u8Download();
//    return downloader;
//}
//
//void MovieDLSetUrl(CM3u8Download* downloader, const char* url) {
//    if (downloader != nullptr) {
//        if (url != 0) {
//            if (downloader->url != 0) {
//                free(downloader->url);
//            }
//            downloader->url = 0;
//            downloader->url =  _strdup(url);
//        }
//    }
//}
//
//void MovieDLSetSavePath(CM3u8Download* downloader, const char* savePath) {
//    if (downloader != nullptr) {
//        assert(savePath != 0);
//        if (downloader->savePath != 0) {
//            free(downloader->savePath);
//            downloader->savePath = 0;
//        }
//        if (downloader->dsSavePath != 0) {
//            free(downloader->dsSavePath);
//            downloader->dsSavePath = 0;
//        }
//        
//        downloader->savePath = _strdup(savePath);
//        downloader->dsSavePath = (char *)malloc(strlen(savePath)+1+3);
//
//        downloader->infoSavePath = (char*)malloc(strlen(savePath) +1+ 5);
//
//        strcpy_s(downloader->dsSavePath,strlen(savePath)+1, savePath);
//        strcpy_s(downloader->dsSavePath + strlen(downloader->dsSavePath), strlen(".ds") + 1, ".ds");
//        
//        strcpy_s(downloader->infoSavePath, strlen(savePath) + 1, savePath);
//        strcpy_s(downloader->infoSavePath + strlen(downloader->infoSavePath), strlen(".info") + 1, ".info");
//    }
//
//}
//
//void MovieDLSetCookie(CM3u8Download* downloader, const char* strCookie) {
//
//    if (downloader != 0) {
//
//        if (downloader->strCookie != 0) {
//            free(downloader->strCookie);
//        }
//        downloader->strCookie = 0;
//        if (strCookie != 0) {
//            downloader->strCookie = _strdup(strCookie);
//        }
//    }
//}
//
//void MovieDLSetHeader(CM3u8Download* downloader, const char* key, const char* value) {
//  
//    if (downloader != 0) {
//        string cell(key);
//        cell += ":";
//        cell += value;
//        downloader->headerchain = headersAppend(downloader->headerchain, cell.c_str());
//    }
//}
//
//void MovieDLStart(CM3u8Download* downloader, bool startFlag1, bool startFlag2) {
//    if (downloader != 0) {
//        downloader->start(startFlag1, startFlag2);
//    }
//}
//
////int main()
////{
////    string url = "http://devimages.apple.com/iphone/samples/bipbop/bipbopall.m3u8";
////    string savePath = "1.mp4";
////    string strCookie = "";
////    map<string, string> http_headers = {
////        {"User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.116 Safari/537.36"},
////        {"Accept-Charset" , "ISO-8859-1,utf-8;q=0.7,*;q=0.7"},
////        {"Accept" ,"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"},
////        {"Accept-Language" , "en-us,en;q=0.5"},
////    };
////
////    CM3u8Download* downloader = MovieDLCreate();
////    MovieDLSetUrl(downloader, url.c_str());
////    MovieDLSetSavePath(downloader, savePath.c_str());
////    MovieDLSetCookie(downloader, strCookie.c_str());
////    map<string, string>::iterator iter;
////    for (iter = http_headers.begin(); iter != http_headers.end(); iter++) {
////
////        MovieDLSetHeader(downloader, iter->first.c_str(), iter->second.c_str());
////    }
////    MovieDLStart(downloader, true, false);
////    return 0;
////}
////
////
#include <iostream>
#include <map>
#include <Windows.h>
#include "utils.h"

#include<string>

using namespace std;

ULONGLONG(*MovieDLCreate)(void);
void (*MovieDLSetUrl)(ULONGLONG, const char*);
void (*MovieDLSetSavePath)(ULONGLONG, const char*);
void (*MovieDLSetCookie)(ULONGLONG, const char*);
void (*MovieDLSetHeader)(ULONGLONG, const char*, const char*);
void (*MovieDLStart)(ULONGLONG, bool, bool);
const char* (*MovieDLGetState)(ULONGLONG);
void (*MovieDLSetCallback)(ULONGLONG, void*, void*);
void callback(char* state) {
	cout << state << endl;
}
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
	MovieDLSetCallback = (void (*)(ULONGLONG, void*, void*))GetProcAddress(dllDownloader, "MovieDLSetCallback");

	ULONGLONG  instance = 0;
	//const char * strUrl = "http://devimages.apple.com/iphone/samples/bipbop/bipbopall.m3u8";
	//const char * strUrl = "https://encrypt-k-vod.xet.tech/2919df88vodtranscq1252524126/33373c79387702304636672569/drm/v.f421220.m3u8?sign=22eb6e78eda48b51368135446789b5f9&t=62fae4e8&us=QXjPYKANrr";
	const char* strUrl = "https://vkceyugu.cdn.bspapp.com/VKCEYUGU-uni4934e7b/c4d93960-5643-11eb-a16f-5b3e54966275.m3u8";
	//const char * strUrl = "https://manifest.googlevideo.com/api/manifest/hls_variant/expire/1660342265/ei/mXv2YpCQOJGGs8IP8qGLwAs/ip/2406%3Ada14%3A918%3A8901%3A3833%3A3a14%3A3b74%3Abcca/id/BkTDF-wKQvo.1~24228425/source/yt_live_broadcast/requiressl/yes/tx/24210080/txs/24174780%2C24174781%2C24174782%2C24174783%2C24174805%2C24210080/hfr/1/playlist_duration/30/manifest_duration/30/maudio/1/gcr/jp/spc/lT-Khofc9wZJy7aZ9y0B8Acnm7IsnpY/vprv/1/go/1/pacing/0/nvgoi/1/keepalive/yes/fexp/24001373%2C24007246/dover/11/itag/0/playlist_type/DVR/sparams/expire%2Cei%2Cip%2Cid%2Csource%2Crequiressl%2Ctx%2Ctxs%2Chfr%2Cplaylist_duration%2Cmanifest_duration%2Cmaudio%2Cgcr%2Cspc%2Cvprv%2Cgo%2Citag%2Cplaylist_type/sig/AOq0QJ8wRQIgHtJqT4AHkFNSONW86CjbJL-H_hTsCwFaV3_f3MDDDDcCIQDOxBeVyI51hrQhq1WiaSzozLNCnYEPKZ2S1SIE6kGNDw%3D%3D/file/index.m3u8";
	//const char * strUrl = "https://vkceu.cdn.bspapp.com/VKCEYUGU-uni4934e7b/c4d93960-5643-11eb-a16f-5b3e5466275.m3u8"; //error url
	const char* strSavePath = "1.mp4";
	const char* strCookie = "";
	map<string, string> http_headers;
	//http_headers[string("User-Agent")] = string("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.116 Safari/537.36");

	const char* data = 0;
	int a = sizeof(string);
	instance = MovieDLCreate();
	MovieDLSetUrl(instance, strUrl);
	MovieDLSetSavePath(instance, strSavePath);
	MovieDLSetCookie(instance, strCookie);
	MovieDLSetCallback(instance, callback, callback);
	map<string, string>::iterator iter;
	for (iter = http_headers.begin(); iter != http_headers.end(); iter++) {

		MovieDLSetHeader(instance, iter->first.c_str(), iter->second.c_str());

	}

	MovieDLStart(instance, true, false);
	do {
		Sleep(1000);

		data = MovieDLGetState(instance);

	} while (!strstr(data, "download_complete") && !strstr(data, "{event=download_error}"));

	return 0;
}


