#include "utils.h"
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")


bool g_flag = 0;
unsigned int g_count = 0;
int g_initFlag = 0;
unsigned int  currentTime = 0;

std::string getGuid() {

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
    int len = strlen(buffer);

    return std::string(buffer, len);
}

bool initFlag() {
    if (g_flag == false) {
        g_flag = true;
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
    currentTime = currentTime * 0x41C64E6D + 0x3039;
    return __rol(currentTime, 16);
}
time_t getCurrentTime() {
    currentTime = time(0);
    transferTime();
    transferTime();
    return transferTime();
}

int init(int flag)
{
    int count = g_count;
    g_count += 1;
    if (count == 0) {
        if (flag & 1) {
            if (!initFlag())
                return 2;
        }
        if (flag & 2) {
            if (initWSA()!=0) return 2;
            g_initFlag = flag;
            getCurrentTime();
        }
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