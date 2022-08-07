#pragma once
#include"utils.h"
#include <stdio.h>      /* printf, fgets	*/
#include <stdlib.h>     /* atoll */
#include <iostream>
#include "hook.h"
#include "CMovieDownloadBase.h"
#include "log.h"
#include "CM3u8Download.h"

template<typename dst_type,typename src_type>
dst_type inline union_cast(src_type src)
{
    union{
        src_type s;
        dst_type d;
    }u;
    u.s = src;
    return u.d;
}


void inline hookinit() {
    hook hk;
	DWORD dwOldProtect, dwAddress;
	long long  * heapfree = (long long  *)0x000000018008D2B0;

    hk.hook_by_code((FARPROC)0x0000000180001050, (PROC)LogD);
	//
	VirtualProtect((LPVOID)heapfree,8, PAGE_EXECUTE_READWRITE, &dwOldProtect);//为了修改字节，先向内存添加“写”的属性
	*heapfree=(long long)heapFreeHook;
 //  hk.hook_by_code((FARPROC)0x0000000180009CC0, (PROC)sub_180009CC0);

 //   

   hk.hook_by_code((FARPROC)0x000000018000B3F0, union_cast<PROC>(&CMovieDownloadBase::setCallbackState));
   hk.hook_by_code((FARPROC)0x0000000180001F70, union_cast<PROC>(&CM3u8Download::download));
   hk.hook_by_code((FARPROC)0x0000000180005B50, union_cast<PROC>(&CM3u8Download::downloadSegment));
	hk.hook_by_code((FARPROC)0x000000018000B8C0, (PROC)RegexExec);
   
}