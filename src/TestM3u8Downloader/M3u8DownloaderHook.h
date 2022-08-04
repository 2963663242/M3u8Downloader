#pragma once
#include <iostream>
#include <cassert>
#include "CM3u8Download.h"
#include <map>
class M3u8DownloaderHook
{
public:
     
        static  CM3u8Download* MovieDLCreate() {
        CM3u8Download* downloader = new CM3u8Download();
        return downloader;
    }

        static void  MovieDLSetUrl(CM3u8Download* downloader, const char* url) {
        if (downloader != 0) {
            if (url != 0) {
                if (downloader->url != 0) {
                    free(downloader->url);
                }
                downloader->url = 0;
                downloader->url = _strdup(url);
            }
        }
    }

    static void  MovieDLSetSavePath(CM3u8Download* downloader, const char* savePath) {
        if (downloader != 0) {
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
            downloader->dsSavePath = (char*)malloc(strlen(savePath) + 1 + 3);

            downloader->infoSavePath = (char*)malloc(strlen(savePath) + 1 + 5);

            strcpy_s(downloader->dsSavePath, strlen(savePath) + 1, savePath);
            strcpy_s(downloader->dsSavePath + strlen(downloader->dsSavePath), strlen(".ds") + 1, ".ds");

            strcpy_s(downloader->infoSavePath, strlen(savePath) + 1, savePath);
            strcpy_s(downloader->infoSavePath + strlen(downloader->infoSavePath), strlen(".info") + 1, ".info");
        }

    }

    static void  MovieDLSetCookie(CM3u8Download* downloader, const char* strCookie) {

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

    static void  MovieDLSetHeader(CM3u8Download* downloader, const char* key, const char* value) {

        if (downloader != 0) {
            string cell(key);
            cell += ":";
            cell += value;
            downloader->headerchain = headersAppend(downloader->headerchain, cell.c_str());
        }
    }

    static  void  MovieDLStart(CM3u8Download* downloader, bool startFlag1, bool startFlag2) {
        if (downloader != 0) {
            downloader->start(startFlag1, startFlag2);
        }
    }
    static  char *  MovieDLGetState(CM3u8Download* downloader) {
        if(downloader)
            return _strdup(downloader->state.data());
        return 0;
    }
};

