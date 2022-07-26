#include <Windows.h>
#include "log.h"
#include <sys/timeb.h>
#include <time.h>


 char* logPath = (char*)0x00000001800BB8A0;
 FILE** Stream = (FILE**)0x00000001800BBDC8;
 PDWORD64 off_1800B2500 = (PDWORD64)0x1800B2500;
 PDWORD dword_1800BBDC0 = (PDWORD)0x1800BBDC0;
 int* g_format = (int*)0x00000001800B4508;
 LPCRITICAL_SECTION CriticalSection = (LPCRITICAL_SECTION)0x00000001800B4538;
 char OutputString[0x3E7F];
 char level[4][8] = { "ERROR","WARING","INFO","DEBUG"};
 void LogD(int deLevel, const char* format, ...)
 {
	 FILE* stream;
	 __timeb64 Time;
	 char timeYMD[24];
	 char timeHMS[24];
	 tm* lotime;
	 va_list va;
	 

	 if (deLevel <= *g_format) {

		 stream = *Stream;
		 if (!stream) {
			 stream = (FILE *)off_1800B2500 + 12;
		 }

		 *dword_1800BBDC0 = *dword_1800BBDC0 == 0 ? -1 : *dword_1800BBDC0;

		 EnterCriticalSection(CriticalSection);

		 _ftime64(&Time);
		 lotime = localtime(&Time.time);

		 sprintf(timeYMD, "%04d-%02d-%02d", lotime->tm_year, lotime->tm_mon, lotime->tm_mday);
		 sprintf(timeHMS, "%02d:%02d:%02d", lotime->tm_hour, lotime->tm_min, lotime->tm_sec);
		 
		 va_start(va, format);
		 vsnprintf(OutputString, 0x3E7F, format,va);
		 va_end(va);

		 fprintf(stream, "(%s %s)%s: ", timeYMD, timeHMS, level[deLevel]);
		 fprintf(stream, "%s\n", OutputString);

		 OutputDebugStringA(OutputString);
		 OutputDebugStringA("\n");
		 
		 if (*Stream) {
			 fflush(*Stream);
		 }

		 LeaveCriticalSection(CriticalSection);

	 }
 }
