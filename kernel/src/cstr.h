#pragma once

#include <stdint.h>

const char* to_string(uint64_t value);
const char* to_string(int64_t value);
const char* to_string(int value);
const char* to_hstring(uint64_t value);
const char* to_hstring(uint32_t value);
const char* to_hstring(uint16_t value);
const char* to_hstring(uint8_t value);
const char* to_string(double value, uint8_t decimalPlaces);
const char* to_string(double value);
const char* to_string(bool value);

int64_t to_int(const char* string);

unsigned int ConvertStringToHex(const char* data);
const char* ConvertHexToString(uint64_t hex, uint8_t size);
const char* ConvertHexToString(uint64_t hex);
const char* ConvertHexToString(uint32_t hex);
const char* ConvertHexToString(uint16_t hex);
const char* ConvertHexToString(uint8_t hex);