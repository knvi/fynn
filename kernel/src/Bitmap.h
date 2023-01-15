#pragma once

#include <stddef.h>
#include <stdint.h>

class Bitmap
{
public:
    bool Set(uint64_t index, bool value);
    bool operator[](uint64_t index);
    size_t Size;
    uint8_t *Buffer;
};