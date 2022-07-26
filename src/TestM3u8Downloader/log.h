#pragma once
#include <cstdio>

extern char* logPath;
extern FILE** Stream;

enum LogLevel {
	Error,
	Waring,
	Info,
	Debug
};

void LogD(int deLevel, const char* format, ...);