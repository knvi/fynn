#include "cstr.h"

char uintTo_StringOutput[128];
const char *to_string(uint64_t value)
{
    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0)
    {
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while (value / 10 > 0)
    {
        uint8_t remainder = value % 10;
        value /= 10;
        uintTo_StringOutput[size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    uintTo_StringOutput[size - index] = remainder + '0';
    uintTo_StringOutput[size + 1] = 0;
    return uintTo_StringOutput;
}

char hexTo_StringOutput[128];
const char *to_hstring(uint64_t value)
{
    uint64_t *valPtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;
    for (uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t *)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StringOutput[size + 1] = 0;
    return hexTo_StringOutput;
}

char hexTo_StringOutput32[128];
const char *to_hstring(uint32_t value)
{
    uint32_t *valPtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 4 * 2 - 1;
    for (uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t *)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput32[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput32[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StringOutput32[size + 1] = 0;
    return hexTo_StringOutput32;
}

char hexTo_StringOutput16[128];
const char *to_hstring(uint16_t value)
{
    uint16_t *valPtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 2 * 2 - 1;
    for (uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t *)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput16[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput16[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StringOutput16[size + 1] = 0;
    return hexTo_StringOutput16;
}

char hexTo_StringOutput8[128];
const char *to_hstring(uint8_t value)
{
    uint8_t *valPtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 1 * 2 - 1;
    for (uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t *)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput8[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput8[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StringOutput8[size + 1] = 0;
    return hexTo_StringOutput8;
}

char intTo_StringOutput[128];
const char *to_string(int64_t value)
{
    uint8_t isNegative = 0;

    if (value < 0)
    {
        isNegative = 1;
        value *= -1;
        intTo_StringOutput[0] = '-';
    }

    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0)
    {
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while (value / 10 > 0)
    {
        uint8_t remainder = value % 10;
        value /= 10;
        intTo_StringOutput[isNegative + size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    intTo_StringOutput[isNegative + size - index] = remainder + '0';
    intTo_StringOutput[isNegative + size + 1] = 0;
    return intTo_StringOutput;
}

char doubleTo_StringOutput[128];
const char *to_string(double value, uint8_t decimalPlaces)
{
    if (decimalPlaces > 20)
        decimalPlaces = 20;

    char *intPtr = (char *)to_string((int64_t)value);
    char *doublePtr = doubleTo_StringOutput;

    if (value < 0)
    {
        value *= -1;
    }

    while (*intPtr != 0)
    {
        *doublePtr = *intPtr;
        intPtr++;
        doublePtr++;
    }

    *doublePtr = '.';
    doublePtr++;

    double newValue = value - (int)value;

    for (uint8_t i = 0; i < decimalPlaces; i++)
    {
        newValue *= 10;
        *doublePtr = (int)newValue + '0';
        newValue -= (int)newValue;
        doublePtr++;
    }

    *doublePtr = 0;
    return doubleTo_StringOutput;
}

const char *to_string(double value)
{
    return to_string(value, 2);
}

unsigned int ConvertStringToHex(const char* data)
{
    unsigned int hex = 0;

    for (unsigned int i = 0; i < 6;)
    {
        unsigned char temp = 0;
        for (unsigned int i2 = 16; i2 != 0; i2 /= 16)
        {
            if (data[i] >= '0' && data[i] <= '9')
                temp += (data[i] - '0') * i2;
            else if (data[i] >= 'A' && data[i] <= 'F')
                temp += (data[i] + 10 - 'A') * i2;
            else if (data[i] >= 'a' && data[i] <= 'f')
                temp += (data[i] + 10 - 'a') * i2;
            i++;
        }
        hex = hex << 8;
        hex += temp;
    }
    
    return hex; 
}


char hexTo_stringOutput[128];
const char* hexABC = "0123456789ABCDEF";
const char* ConvertHexToString(uint64_t hex, uint8_t size)
{
    hex = (hex << (16 - size) * 4);
    for (uint8_t i = 0; i < size; i++)
        hexTo_stringOutput[i] = hexABC[((hex << i * 4) >> ((15) * 4))];
    
    hexTo_stringOutput[size] = 0;

    return hexTo_stringOutput; 
}
const char* ConvertHexToString(uint64_t hex)
{
    return ConvertHexToString(hex, (64/4));
}
const char* ConvertHexToString(uint32_t hex)
{
    return ConvertHexToString(hex, (32/4));
}
const char* ConvertHexToString(uint16_t hex)
{
    return ConvertHexToString(hex, (16/4));
}
const char* ConvertHexToString(uint8_t hex)
{
    return ConvertHexToString(hex, (8/4));
}