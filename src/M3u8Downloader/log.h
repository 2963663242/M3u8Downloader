#pragma once

enum LogLevel;

void LogD(int deLevel, const char* format, ...);
void ALogInit(char* FileName);
char* AGetLogFileName();
void ALogFree();
void ALogSetLevel(LogLevel level);

class guard {
public:
	guard() {
		InitializeCriticalSection(&this->cs);
	}
	~guard() {
		DeleteCriticalSection(&this->cs);
	}
public:
	CRITICAL_SECTION cs;
};
enum LogLevel {
	Error,
	Waring,
	Info,
	Debug
};