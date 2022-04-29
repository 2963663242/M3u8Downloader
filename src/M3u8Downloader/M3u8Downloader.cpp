// M3u8Downloader.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CM3u8Download.h"
void * MovieDLCreate() {
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
int main()
{
    MovieDLCreate();
}


