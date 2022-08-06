#include <Windows.h>
#include <iostream>
#include "log.h"
#include <sys/timeb.h>
#include <time.h>

 FILE* Stream;
 PDWORD64 off_1800B2500 = (PDWORD64)0x1800B2500;
 DWORD dword_1800BBDC0 = 0;
  LogLevel g_format =Info;
 guard CriticalSection;
 char OutputString[0x3E7F];
 char level[4][8] = { "ERROR","WARING","INFO","DEBUG"};
 char* Block;

 void ALogInit(char* FileName) {
	 if (!Stream) {
		 Block = new char[strlen(FileName)+1];
		 strcpy(Block, FileName);
		 Stream = fopen(FileName, "a+");
	 }
 }
 char* AGetLogFileName(){
	 return Block;
 }
 void ALogFree() {
	 if (Stream) {
		 fclose(Stream);
	 }
	 delete []Block;
	 Stream = 0;
}
 
 void ALogSetLevel(LogLevel level) {
	 g_format = level;
 }

 void LogD(int deLevel, const char* format, ...)
 {
	 FILE* stream;
	 __timeb64 Time;
	 char timeYMD[24];
	 char timeHMS[24];
	 tm* lotime;
	 va_list va;
	 

	 if (deLevel <= g_format) {

		 stream = Stream;
		 if (!stream) {
			 stream = (FILE *)off_1800B2500 + 12;
		 }

		 dword_1800BBDC0 = dword_1800BBDC0 == 0 ? -1 : dword_1800BBDC0;

		 EnterCriticalSection(&CriticalSection.cs);

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
		 
		 if (Stream) {
			 fflush(Stream);
		 }

		 LeaveCriticalSection(&CriticalSection.cs);

	 }
 }
