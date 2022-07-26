#include "CM3u8Download.h"
#include "utils.h"
#include "log.h"
CM3u8Download::CM3u8Download()
{
		init(3);
}
void CM3u8Download::download()
{
	LogD(LogLevel::Info, "============================ %s ==> Start Download ============================", this->guid.data());
	string str1;
	string str2;
	string str3;

	long long var_790 = 0;
	long var_7D4 = 0;
	long var_7D0 = 0;

	long long* var_668;
	if (var_668 = new __int64) {
		*var_668 = (long long)&var_668;
	}
	else {
		var_668 = 0;
	}
	__int64 * var_638 = sub_180009CC0();
	*((char*)var_638 + 0x69) = 1;
	var_638[1] = (__int64)var_638;
	var_638[0] = (__int64)var_638;
	var_638[2] = (__int64)var_638;
	__int64* var_630 = 0;

	string str_4E8;
	string str_388;
	string str_338;
	string str_360;
	string str_3D8;
	string str_428;
	string str_498;

	long long* var_6E8;
	if (var_6E8 = new __int64) {
		*var_6E8 = (long long)&var_6E8;
	}
	else {
		var_6E8 = 0;
	}


}

__int64* sub_180009CC0() {
	__int64* arg_8;
	
	if (arg_8 = new __int64[0x14]) {
		arg_8[0] = 0;
	}
	if (&arg_8[1]) {
		arg_8[1] = 0;
	}
	if (&arg_8[2]) {
		arg_8[2] = 0;
	}
	*((char*)arg_8 + 0x68) = 1;
	*((char*)arg_8 + 0x69) = 0;
	return arg_8;
}