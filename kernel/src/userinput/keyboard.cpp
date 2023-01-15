#include "keyboard.h"

#include <stdint.h>
#include "kbScancodeTranslation.h"
#include "../BasicRenderer.h"
#include "../cmd/cmdParse.h"
#include "../Colors.h"
#include "../osData/userdata.h"

bool lshift = false;
bool rshift = false;
char lastUserData[256];
char userData[256];
int userLen = 0;
bool allowArrows = false;

uint32_t userCol = Colors.bgreen;
const char* userName = "User";

void ClearInput()
{
    for (int i = 0; i < 256; i++)
    {
        lastUserData[i] = userData[i];
        userData[i] = 0;
    }
    userLen = 0;
}


void HandleEnter()
{
    if (userLen > 0)
    {
        ParseCommand(userData, lastUserData, &currentUser);
        //GlobalRenderer->Print("> ");
        //GlobalRenderer->Println(userData);
    }
    GlobalRenderer->Println();
    PrintUser(currentUser);
    ClearInput();
}

void InitKeyboard()
{
    userCol = Colors.bgreen;
    ClearInput();
}

void KeyboardPrintStart()
{
    GlobalRenderer->CursorPosition.y += 16;
    PrintUser(currentUser);
}

void PrintUser(OSUser* user)
{
    GlobalRenderer->Print(user->userName, user->colData.userColor);
    GlobalRenderer->Print("> ");
}

void HandleKeyboard(uint8_t scancode)
{
    if (scancode == LeftShift)
        lshift = true; 
    else if (scancode == LeftShift + 0x80)
        lshift = false;
    else if (scancode == RightShift)
        rshift = true; 
    else if (scancode == RightShift + 0x80)
        rshift = false;
    else if (scancode == Enter)
        {
            GlobalRenderer->Println();
            HandleEnter();
        }
    else if (scancode == Backspace)
    {
        if (userLen > 0)
        {
            userLen--;
            userData[userLen] = 0;
            GlobalRenderer->CursorPosition.x -= 8; 
            GlobalRenderer->delChar(GlobalRenderer->CursorPosition.x, GlobalRenderer->CursorPosition.y);
        }
    } 
    else if (scancode == ARR_UP)
    {
        if (allowArrows)
        {
            if (GlobalRenderer->CursorPosition.y >= 16)
                GlobalRenderer->CursorPosition.y -= 16;
        }
    }
    else if (scancode == ARR_DOWN) 
    {
        if (allowArrows)
        {
            GlobalRenderer->CursorPosition.y += 16;
        }
    }
    else if (scancode == ARR_LEFT)
    {  
        if (allowArrows)
        {
            if (GlobalRenderer->CursorPosition.x >= 8)
                GlobalRenderer->CursorPosition.x -= 8; 
            else if (GlobalRenderer->CursorPosition.y >= 16)
            {
                GlobalRenderer->CursorPosition.y -= 16;
                GlobalRenderer->CursorPosition.x = GlobalRenderer->framebuffer->Width - 8;
            }
        }
    }
    else if (scancode == ARR_RIGHT)
    {
        if (allowArrows)
        {
            GlobalRenderer->CursorPosition.x += 8; 
        }
    }
    else
    {
        char ascii = QWERTYKeyboard::Translate(scancode, lshift || rshift);

        if (ascii != 0)
        {
            
            if (currentUser->mode == commandMode::none)
                GlobalRenderer->Print(ascii);
            else if (currentUser->mode == commandMode::enterText)
                GlobalRenderer->Print(ascii);
            else if (currentUser->mode == commandMode::enterPassword)
                GlobalRenderer->Print("*");

            if (userLen < 255)
            {
                userData[userLen] = ascii;
                userLen++;
            }
        }
    }
} 



/*
    else if (scancode == Backspace)
    {
        if (GlobalRenderer->CursorPosition.x >= 8)
            GlobalRenderer->CursorPosition.x -= 8; 
        else if (GlobalRenderer->CursorPosition.y >= 16)
        {
            GlobalRenderer->CursorPosition.y -= 16;
            GlobalRenderer->CursorPosition.x = GlobalRenderer->framebuffer->Width - 8;
        }
        GlobalRenderer->delChar(GlobalRenderer->CursorPosition.x, GlobalRenderer->CursorPosition.y);
        if (userLen > 0)
        {
            userLen--;
            userData[userLen] = 0;
        }
    } 
*/