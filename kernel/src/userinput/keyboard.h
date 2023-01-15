#pragma once
#include <stdint.h>
#include "../osData/userdata.h"
#include "../windows/Window/window.h"

void ClearInput(TerminalInstance* instance);

void HandleEnter();

void InitKeyboard();

void KeyboardPrintStart(Window* window);

void PrintUser(Window* window, OSUser* user);

void HandleKeyboard(uint8_t scancode);