#include "utils.h"
#include <Dbghelp.h>
#include <string>
#include <assert.h>
#include "log.h"
#include "CMovieDownloadBase.h"
#include "CM3u8Download.h"
#include <regex>


#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Dbghelp.lib")
char logPath[1024] = { 0, };

bool  heapFreeHook() {
	return true;
}
string GetProtocol(const string& url)//协议名
{
	size_t pos = url.find("://");
	if (pos != string::npos)
	{
		return url.substr(0, pos - 0);
	}
	else
	{
		return string();//匿名对象
	}

}

string getGuid(void* rcx) {
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

	return buffer;
}




curl_slist* getLastHeader(curl_slist* headers) {

	curl_slist* now;
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

curl_slist* headersAppend(curl_slist* headers, const char* cell) {
	curl_slist* lastHeader;
	curl_slist* newHeader = (curl_slist*)malloc(16);

	if (newHeader == 0) return 0;
	char* vCell = _strdup(cell);
	if (vCell == 0) {
		free(newHeader);
	}
	newHeader->next = 0;
	newHeader->data = vCell;
	if (headers == 0)
		return newHeader;
	lastHeader = getLastHeader(headers);
	lastHeader->next = newHeader;
	return headers;
}

long lpTopLevelExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
	char Text[1024] = { 0, };
	if (logPath)
	{
		HANDLE file = CreateFileA(logPath, 0x40000000, 0, 0, 2, 128, 0);
		if (file != INVALID_HANDLE_VALUE) {

			MINIDUMP_EXCEPTION_INFORMATION ExceptionParam;
			ExceptionParam.ClientPointers = 0;
			ExceptionParam.ExceptionPointers = ExceptionInfo;
			ExceptionParam.ThreadId = GetCurrentThreadId();

			bool ret = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, MiniDumpNormal, ExceptionInfo ? &ExceptionParam : 0, 0, 0);
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

std::vector<std::vector<std::string>> findAll(string pattern, string  anyStr)
{
	std::vector<std::vector<std::string>> result;
	const std::string inputStr = anyStr;
	std::smatch output;
	std::regex patternstr(pattern);
	std::string tmpStr = "";
	auto start = inputStr.begin();
	auto end = inputStr.end();
	while (regex_search(start, end, output, patternstr)) {
		std::vector<string> matcher = {};
	
		for (auto o : output) {
			matcher.push_back(o);
		}
	
		result.push_back(matcher);

		tmpStr = output.suffix();
		start = tmpStr.begin();
		end = tmpStr.end();
		
	}
	return result;
}

bool RegexExec(std::string const& url, char const* rex, std::vector<std::vector<std::string>>& results) {

	results = findAll(rex, url);
	if (results.size() == 0) {
		LogD(Info, "regex error : %s", rex);
		return false;
	}
	return true;
}


void hex2Bin(const std::string hexStr, unsigned char* pData)
{

	int wSize = hexStr.size() / 2;

	for (int i = 0; i < hexStr.size(); i += 2)
	{
		int ch = 0;

		char temp[10] = "";
		temp[0] = hexStr[i];
		temp[1] = hexStr[i + 1];

		sscanf(temp, "%x", &ch);  // HEX to int
		pData[i / 2] = ch;
	}
}

http parseURL(std::string url) {
	const char* parseptr1;
	const char* parseptr2;
	int len;
	int i;
	http hUrl;
	parseptr2 = url.c_str();
	parseptr1 = strchr(parseptr2, ':');
	if (NULL == parseptr1) {
		return hUrl;
	}

	len = parseptr1 - parseptr2;
	for (i = 0; i < len; i++) {
		if (!isalpha(parseptr2[i])) {

			return hUrl;
		}
	}
	hUrl.protocol = std::string(parseptr2, 0, len); //get protocol
	parseptr1++;
	parseptr2 = parseptr1;
	for (i = 0; i < 2; i++) {
		if ('/' != *parseptr2) {

			return hUrl;
		}
		parseptr2++;
	}

	parseptr1 = strchr(parseptr2, ':');
	if (NULL == parseptr1)//判断有无端口号
	{
		parseptr1 = strchr(parseptr2, '/');
		if (NULL == parseptr1) {

			return hUrl;
		}
		len = parseptr1 - parseptr2;

		hUrl.hostname = std::string(parseptr2, 0, len); //get hostname
	}
	else {
		len = parseptr1 - parseptr2;
		hUrl.hostname = std::string(parseptr2, 0, len); //get hostname
		parseptr1++;
		parseptr2 = parseptr1;
		parseptr1 = strchr(parseptr2, '/');
		if (NULL == parseptr1) {

			return hUrl;
		}
		len = parseptr1 - parseptr2;

		hUrl.port = std::string(parseptr2, 0, len); //get port
	}

	parseptr1++;
	parseptr2 = parseptr1;
	while ('\0' != *parseptr1 && '?' != *parseptr1 && '#' != *parseptr1) {
		parseptr1++;
	}
	len = parseptr1 - parseptr2;

	hUrl.path = std::string(parseptr2, 0, len); //get path

	parseptr2 = parseptr1;
	if ('?' == *parseptr1) {
		parseptr2++;
		parseptr1 = parseptr2;
		while ('\0' != *parseptr1 && '#' != *parseptr1) {
			parseptr1++;
		}
		len = parseptr1 - parseptr2;

		hUrl.query = std::string(parseptr2, 0, len); //get query
	}

	parseptr2 = parseptr1;
	if ('#' == *parseptr1) {
		parseptr2++;
		parseptr1 = parseptr2;
		while ('\0' != *parseptr1) {
			parseptr1++;
		}
		len = parseptr1 - parseptr2;
		hUrl.fragment = std::string(parseptr2, 0, len); //get fragment
	}
	return hUrl;
}

int __stdcall StartAddress(CM3u8Download* downloader) {
	assert(downloader);
	if (downloader->flag1 != 0) {
		downloader->startDownload();
	}
	else {
		downloader->startDownloadExternal();
	}
	return 0;
}

int deleteFile(char* filename) {

	if (!DeleteFile(filename)) {
		int err = GetLastError();
		if (err) {

			return -1;
		}
	}
	return 0;
}