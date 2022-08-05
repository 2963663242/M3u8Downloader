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
/*
    sprintf(buf, "%I64u", &CMovieDownloadBase::start);

	 long long a =0;
	 sscanf(buf,"%I64u",&a);*/
    hk.hook_by_code((FARPROC)0x0000000180001050, (PROC)LogD);

   // hk.hook_by_code((FARPROC)0x0000000180009CC0, (PROC)sub_180009CC0);

    

   // hk.hook_by_code((FARPROC)0x000000018000B3F0, union_cast<PROC>(&CMovieDownloadBase::setCallbackState));
	//hk.hook_by_code((FARPROC)0x000000018000B8C0, (PROC)RegexExec);
    /*void** start = (void**)0x00000001800A19A8;
    *start = (void*)a;*/
    // hk.hook_by_code((FARPROC)0x000000018000B180, (PROC)a);
    // sprintf(buf, "%I64u", &CMovieDownloadBase::CMovieDownloadBaseEx);

    //a = atoll(buf);

    // hk.hook_by_code((FARPROC)0x000000018000AC95, (PROC)a);
}