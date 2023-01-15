#pragma once
#include "../kernelUtil.h"
#include "../types/list.h"
#include "../windows/Window/window.h"

struct OSData
{
    bool exit;
    KernelInfo* kernelInfo;
    List<Window*> windows;
};

extern OSData osData;