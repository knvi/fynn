#pragma once
#include "stdint.h"

template <typename T> class Array
{
    private:
    T* arr;
    T def = T();
    bool freed;
    uint64_t size;

    public:
    //bool freed;
    uint64_t getSize();
    Array<T>(uint64_t size);
    T& operator[](uint64_t index);
    void free();
    Array<T> clone();
    void copy(Array<T> target);
    void reInit(uint64_t size);
};

template class Array<int>;