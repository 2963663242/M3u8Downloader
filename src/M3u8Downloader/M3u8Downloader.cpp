// M3u8Downloader.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cassert>
#include "CM3u8Download.h"
#include <map>
CM3u8Download * MovieDLCreate() {
    CM3u8Download* downloader = new CM3u8Download();
    return downloader;
}

void MovieDLSetUrl(CM3u8Download* downloader, const char* url) {
    if (downloader != nullptr) {
        if (url != 0) {
            if (downloader->url != 0) {
                free(downloader->url);
            }
            downloader->url = 0;
            downloader->url =  _strdup(url);
        }
    }
}

void MovieDLSetSavePath(CM3u8Download* downloader, const char* savePath) {
    if (downloader != nullptr) {
        assert(savePath != 0);
        if (downloader->savePath != 0) {
            free(downloader->savePath);
            downloader->savePath = 0;
        }
        if (downloader->dsSavePath != 0) {
            free(downloader->dsSavePath);
            downloader->dsSavePath = 0;
        }
        
        downloader->savePath = _strdup(savePath);
        downloader->dsSavePath = (char *)malloc(strlen(savePath)+1+3);

        downloader->infoSavePath = (char*)malloc(strlen(savePath) +1+ 5);

        strcpy_s(downloader->dsSavePath,strlen(savePath)+1, savePath);
        strcpy_s(downloader->dsSavePath + strlen(downloader->dsSavePath), strlen(".ds") + 1, ".ds");
        
        strcpy_s(downloader->infoSavePath, strlen(savePath) + 1, savePath);
        strcpy_s(downloader->infoSavePath + strlen(downloader->infoSavePath), strlen(".info") + 1, ".info");
    }

}

void MovieDLSetCookie(CM3u8Download* downloader, const char* strCookie) {

    if (downloader != 0) {

        if (downloader->strCookie != 0) {
            free(downloader->strCookie);
        }
        downloader->strCookie = 0;
        if (strCookie != 0) {
            downloader->strCookie = _strdup(strCookie);
        }
    }
}

void MovieDLSetHeader(CM3u8Download* downloader, const char* key, const char* value) {
  
    if (downloader != 0) {
        string cell(key);
        cell += ":";
        cell += value;
        downloader->headerchain = headersAppend(downloader->headerchain, cell.c_str());
    }
}

void MovieDLStart(CM3u8Download* downloader, bool startFlag1, bool startFlag2) {
    if (downloader != 0) {
        downloader->start(startFlag1, startFlag2);
    }
}

//int main()
//{
//    string url = "http://devimages.apple.com/iphone/samples/bipbop/bipbopall.m3u8";
//    string savePath = "1.mp4";
//    string strCookie = "";
//    map<string, string> http_headers = {
//        {"User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.116 Safari/537.36"},
//        {"Accept-Charset" , "ISO-8859-1,utf-8;q=0.7,*;q=0.7"},
//        {"Accept" ,"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"},
//        {"Accept-Language" , "en-us,en;q=0.5"},
//    };
//
//    CM3u8Download* downloader = MovieDLCreate();
//    MovieDLSetUrl(downloader, url.c_str());
//    MovieDLSetSavePath(downloader, savePath.c_str());
//    MovieDLSetCookie(downloader, strCookie.c_str());
//    map<string, string>::iterator iter;
//    for (iter = http_headers.begin(); iter != http_headers.end(); iter++) {
//
//        MovieDLSetHeader(downloader, iter->first.c_str(), iter->second.c_str());
//    }
//    MovieDLStart(downloader, true, false);
//    return 0;
//}
//
//
