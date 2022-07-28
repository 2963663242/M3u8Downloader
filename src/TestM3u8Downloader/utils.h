#ifndef __UTILS_LOCAL_DEFINED//避免结构体CMenuItemInfo重复定义
#define __UTILS_LOCAL_DEFINED

#include <iostream>
#include <windows.h>

using namespace std;

extern unsigned int *g_count;
extern char logPath[1024];

typedef struct  _headerchain
{
	char* cell;
	_headerchain* next;
}headerchain;

string getGuid(void* rcx);
bool initFlag();
int init(int flag);
headerchain* headersAppend(headerchain* headers, const char* cell);
long lpTopLevelExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo);
//unsigned int StartAddress(void* downloader);
#endif