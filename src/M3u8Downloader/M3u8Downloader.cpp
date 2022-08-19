#include <cassert>
#include "CM3u8Download.h"
#include <map>
#include <string>



CM3u8Download* MovieDLCreate() {
    CM3u8Download* downloader = new CM3u8Download();
    return downloader;
}

void  MovieDLSetUrl(CM3u8Download* downloader, const char* url) {
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

 void  MovieDLSetSavePath(CM3u8Download* downloader, const char* savePath) {
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

void  MovieDLSetCookie(CM3u8Download* downloader, const char* strCookie) {

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

 void  MovieDLSetHeader(CM3u8Download* downloader, const char* key, const char* value) {

    if (downloader != 0) {
        string cell(key);
        cell += ":";
        cell += value;
        downloader->headerchain = headersAppend(downloader->headerchain, cell.c_str());
    }
}

  void  MovieDLStart(CM3u8Download* downloader, bool startFlag1, bool startFlag2) {
    if (downloader != 0) {
        downloader->start(startFlag1, startFlag2);
    }
}
 char* MovieDLGetState(CM3u8Download* downloader) {
    if (downloader)
        return _strdup(downloader->state.data());
    return 0;
}

static void MovieDLStop(CM3u8Download* downloader) {
    if (downloader)
        downloader->stop();
}
void MovieDLSetSpeedLimit(CM3u8Download* downloader, int speedLimit) {
    if (downloader)
        downloader->speedLimit = speedLimit;
}
 long long int MovieDLGetLength(CM3u8Download* downloader) {
    if (downloader)
        return downloader->totalSize;
    return 0;
}
void MovieDLRelease(CM3u8Download* downloader) {
    if (downloader)
        delete downloader;
}

void MovieDLSetCallback(CM3u8Download* downloader, void  (*callback)(char*), void* callback2) {
    if (downloader) {
        downloader->callback = callback;
        downloader->callback2 = callback2;
    }

}
 void MovieDLDeleteTempFiles(CM3u8Download* downloader) {
    if (downloader) {
        deleteFile(downloader->dsSavePath);
        deleteFile(downloader->infoSavePath);
    }

}

int main()
{
    string url = "https://vkceyugu.cdn.bspapp.com/VKCEYUGU-uni4934e7b/c4d93960-5643-11eb-a16f-5b3e54966275.m3u8";
    string savePath = "1.mp4";
    string strCookie = "";
    string data;
    map<string, string> http_headers = {
        {"User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.116 Safari/537.36"},
        {"Accept-Charset" , "ISO-8859-1,utf-8;q=0.7,*;q=0.7"},
        {"Accept" ,"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"},
        {"Accept-Language" , "en-us,en;q=0.5"},
    };

    CM3u8Download* downloader = MovieDLCreate();
    MovieDLSetUrl(downloader, url.c_str());
    MovieDLSetSavePath(downloader, savePath.c_str());
    MovieDLSetCookie(downloader, strCookie.c_str());
    map<string, string>::iterator iter;
    for (iter = http_headers.begin(); iter != http_headers.end(); iter++) {

        MovieDLSetHeader(downloader, iter->first.c_str(), iter->second.c_str());
    }
    MovieDLStart(downloader, true, false);

    do {
        Sleep(1000);

        data = MovieDLGetState(downloader);

    } while (!strstr(data.c_str(), "download_complete") && !strstr(data.c_str(), "{event=download_error}"));
    return 0;
}


