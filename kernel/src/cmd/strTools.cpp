#include "strTools.h"

#include "../paging/PageFrameAllocator.h"
#include "../memory/heap.h"

bool StrEquals(const char* a, const char* b) {
    int index = 0;
    while(!(a[index] == 0 && b[index] == 0)) {
        if(a[index] != b[index]) {
            return false;
        }
        index++;
    }

    return true;
}

char* StrCopy(const char* og) {
    int size = 0;
    for (int i = 0; og[i] != 0; i++)
        size++;
    char* newStr = (char*)malloc(size);

    for (int i = 0; og[i] != 0; i++)
        newStr[i] = og[i];
    return newStr;
}

char* StrSubstr(const char* og, int index, int len) {
    char* newStr = (char*)malloc(len);
    for (int i = 0; i < len; i++)
        newStr[i] = og[index + i];
    return newStr;
}

char* StrSubstr(const char* og, int index) {
    int len;
    for (len = 0; og[len] != 0; len++);
    len++;
    if (index >= len)
        index = len - 1;
    len -= index;
    return StrSubstr(og, index, len);
}