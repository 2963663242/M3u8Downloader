#include "utils.h"
#include <Dbghelp.h>
#include "hook.hpp"
#include "CMovieDownloadBase.h"
#include <string>
#include "log.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Dbghelp.lib")

bool *g_flag = (bool *)0x00000001800B468C;
unsigned int *g_count =(unsigned int*)0x00000001800B4680;
int *g_initFlag = (int*)0x00000001800B4684;
time_t * currentTime = (time_t*)0x00000001800B4688;



string getGuid(void * rcx){
    char buffer[64] = { 0, };
    GUID pguid;

    CoCreateGuid(&pguid);

    sprintf_s(buffer, 64, "{%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X}", pguid.Data1,
        pguid.Data2,
        pguid.Data3,
        pguid.Data4[0],
        pguid.Data4[1],
        pguid.Data4[2],
        pguid.Data4[3],
        pguid.Data4[4],
        pguid.Data4[5],
        pguid.Data4[6],
        pguid.Data4[7]);
   // int len = strlen(buffer);
   //memcpy_s(guidstring, sizeof(string), &str, sizeof(string));
   
   
    //guidstring->copy(buffer, len);
   
    return buffer;
}

bool initFlag() {
    if (*g_flag == false) {
        *g_flag = true;
    }
    return true;
}
int initWSA() {
    WSAData wsdata;
    int ret = WSAStartup(514,&wsdata);
    if (ret == 0) {
       unsigned int version = wsdata.wVersion;
        if ((version & 0xFF) == 2 && ((version >> 8) & 0xFF) == 2) {
            return 0;
        }
        WSACleanup();
    }
    return 2;
}

template<typename T>
T __rol(T val, size_t count)
{
    size_t bitcount = sizeof(T) * 8;

    count %= bitcount;
    return (val << count) | (val >> (bitcount - count));
}

template<typename T>
T __ror(T val, size_t count)
{
    size_t bitcount = sizeof(T) * 8;

    count %= bitcount;
    return (val >> count) | (val << (bitcount - count));
}
time_t transferTime() {
    *currentTime = *currentTime * 0x41C64E6D + 0x3039;
    return __rol(*currentTime, 16);
}
time_t getCurrentTime() {
    *currentTime = time(0);
    transferTime();
    transferTime();
    return transferTime();
}

int init(int flag)
{
    if (!*g_count++) {
        if ((flag & 0xFF & 0x1 != 0 && initFlag() == 0)
            || (flag & 0xFF & 0x2 != 0 && initWSA() != 0)
            || 0!=0 ) {
            return 2;
        }
            *g_initFlag = flag;
            getCurrentTime();
    //         struct tm* timeinfo; 
    //timeinfo = localtime(currentTime);
    //printf("The current date/time is: %s", asctime(timeinfo));

    }
    return 0;
}
headerchain* getLastHeader(headerchain* headers) {
    
    headerchain* now;
    if (headers == 0)
        return 0;
    if (headers->next == 0)
        return headers;
    headers = headers->next;
    do {
        now = headers;
        headers = headers->next;
    } while (headers != 0);
    return now;
}

headerchain* headersAppend(headerchain* headers, const char* cell) {
    headerchain* lastHeader;
    headerchain* newHeader = (headerchain *)malloc(16);
   
    if (newHeader == 0) return 0;
    char* vCell = _strdup(cell);
    if (vCell == 0) {
        free(newHeader);
    }
    newHeader->next = 0;
    newHeader->cell = vCell;
    if (headers == 0)
        return newHeader;
    lastHeader = getLastHeader(headers);
    lastHeader->next = newHeader;
    return headers;
}

long lpTopLevelExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
    char Text[1024] = {0,};
    if (logPath)
    {
        HANDLE file = CreateFileA(logPath, 0x40000000, 0, 0, 2, 128, 0);
        if (file != INVALID_HANDLE_VALUE) {
            
            MINIDUMP_EXCEPTION_INFORMATION ExceptionParam;
            ExceptionParam.ClientPointers = 0;
            ExceptionParam.ExceptionPointers = ExceptionInfo;
            ExceptionParam.ThreadId = GetCurrentThreadId();
      
            bool ret = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, MiniDumpNormal, ExceptionInfo?&ExceptionParam:0, 0, 0);
            CloseHandle(file);
        }
        sprintf_s(
            Text,
            "Application Fault! \r\nPlease send the following the file to our support center:\r\n%s",
            logPath);
        
        MessageBoxA(0, Text, "Application Fault", 0x11010);
    }
    return 0;
}

//unsigned int StartAddress(void* downloader) {
//
//    return 0;
//}

