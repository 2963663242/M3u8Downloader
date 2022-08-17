#ifndef __UTILS_LOCAL_DEFINED//避免结构体CMenuItemInfo重复定义
#define __UTILS_LOCAL_DEFINED

#include <iostream>
#include <windows.h>
#include <curl/curl.h>
#include <vector>

using namespace std;

class CM3u8Download;


extern unsigned int *g_count;
extern char logPath[1024];

typedef struct  _headerchain
{
	char* cell;
	_headerchain* next;
}headerchain;

struct http{
	std::string protocol;
	std::string hostname;
	std::string port;
	std::string path;
	std::string query;
	std::string fragment;
};


string getGuid(void* rcx);
bool initFlag();
int init(int flag);
curl_slist* headersAppend(curl_slist* headers, const char* cell);
long lpTopLevelExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo);
bool RegexExec(std::string const&, char const*, std::vector<std::vector<std::string>> &);
//unsigned int StartAddress(void* downloader);
bool  heapFreeHook();
string GetProtocol(const string& url);
void hex2Bin(const std::string hexStr ,unsigned char* pData);
http parseURL(std::string url);
int  __stdcall StartAddress (CM3u8Download * downloader);
#endif