#ifndef __UTILS_LOCAL_DEFINED//����ṹ��CMenuItemInfo�ظ�����
#define __UTILS_LOCAL_DEFINED
#include <iostream>

extern bool g_flag;
extern int g_initFlag;
extern unsigned int g_count;
extern unsigned int  currentTime;

typedef struct  _headerchain
{
	char* cell;
	_headerchain* next;
}headerchain;

std::string getGuid();
bool initFlag();
int init(int flag);
headerchain* headersAppend(headerchain* headers, const char* cell);

#endif