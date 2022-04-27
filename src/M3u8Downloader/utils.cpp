#include <stdio.h>
#include <objbase.h>



void* getGuidString(void* pointer) {

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

    return nullptr;
}