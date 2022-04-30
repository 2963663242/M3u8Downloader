// M3u8Downloader.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CM3u8Download.h"
#include <cassert>
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
        downloader->dsSavePath = (char *)malloc(strlen(savePath)+3);

        downloader->infoSavePath = (char*)malloc(strlen(savePath) + 5);

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
int main()
{
    string url = "https://blog.csdn.net/keith_bb/article/details/51333473";
    string savePath = "1.mp4";
    string strCookie = "";
    CM3u8Download* downloader = MovieDLCreate();
    MovieDLSetUrl(downloader, url.c_str());
    MovieDLSetSavePath(downloader, savePath.c_str());
    MovieDLSetCookie(downloader, strCookie.c_str());

    return 0;
}


