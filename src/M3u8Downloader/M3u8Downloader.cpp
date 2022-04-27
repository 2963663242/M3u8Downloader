// M3u8Downloader.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CM3u8Download.h"
void * MovieDLCreate() {
    CM3u8Download* downloader = new CM3u8Download();
    return downloader;
}

int main()
{
    std::cout << "Hello World!\n";
}


