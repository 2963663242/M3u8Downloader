#pragma once
#include"utils.h"
#include <iostream>
#include "hook.hpp"
#include "CMovieDownloadBase.h"
#include "log.h"
#include "CM3u8Download.h"
void inline hookinit() {
    hook hk;

    hk.hook_by_code((FARPROC)0x0000000180021930, (PROC)init);
    char buf[64] = { 0 };

    sprintf(buf, "%I64u", &CMovieDownloadBase::start);

    long long a = atoll(buf);
    hk.hook_by_code((FARPROC)0x0000000180001050, (PROC)LogD);

    hk.hook_by_code((FARPROC)0x0000000180009CC0, (PROC)sub_180009CC0);
    /*void** start = (void**)0x00000001800A19A8;
    *start = (void*)a;*/
    // hk.hook_by_code((FARPROC)0x000000018000B180, (PROC)a);
    // sprintf(buf, "%I64u", &CMovieDownloadBase::CMovieDownloadBaseEx);

    //a = atoll(buf);

    // hk.hook_by_code((FARPROC)0x000000018000AC95, (PROC)a);
}