
#include "BasicRenderer.h"
#include "cstr.h"
#include <stdint.h>
#include "Colors.h"
#include "cmd/strTools.h"
#include "memory/heap.h"

BasicRenderer *GlobalRenderer;

void BasicRenderer::putChar(char chr, unsigned int xoff, unsigned int yoff)
{
    unsigned int *pixPtr = (unsigned int *)framebuffer->BaseAddress;
    char *fontPtr = ((char *)psf1_font->glyphBuffer) + (chr * psf1_font->psf1_Header->charsize);

    if (overwrite)
        BasicRenderer::delChar(xoff, yoff);

    for (unsigned long y = yoff; y < yoff + 16; y++)
    {
        for (unsigned long x = xoff; x < xoff + 8; x++)
        {
            if ((*fontPtr & (0b10000000 >> (x - xoff))) > 0)
            {
                *(unsigned int *)(pixPtr + x + (y * framebuffer->PixelsPerScanLine)) = color;
            }
        }
        fontPtr++;
    }
}

void BasicRenderer::delChar(unsigned int xoff, unsigned int yoff, uint32_t col)
{
    unsigned int *pixPtr = (unsigned int *)framebuffer->BaseAddress;

    for (unsigned long y = yoff; y < yoff + 16; y++)
        for (unsigned long x = xoff; x < xoff + 8; x++)
            *(unsigned int *)(pixPtr + x + (y * framebuffer->PixelsPerScanLine)) = col;
}

void BasicRenderer::delChar(unsigned int xoff, unsigned int yoff)
{
    BasicRenderer::delChar(xoff, yoff, 0x00000000);
}

void BasicRenderer::putStr(const char *chrs, unsigned int xoff, unsigned int yoff)
{
    for (unsigned int x = 0; chrs[x] != 0; x++)
        putChar(chrs[x], xoff + (x * 8), yoff);
}

void BasicRenderer::printStr(const char *chrs)
{
    printStr(chrs, NULL, true);
}

void BasicRenderer::printStr(const char *chrs, bool allowEscape)
{
    printStr(chrs, NULL, allowEscape);
}

void BasicRenderer::printStr(const char *chrs, const char *var)
{
    printStr(chrs, var, true);
}

void BasicRenderer::printStr(const char *chrs, const char *var, bool allowEscape)
{
    unsigned int index = 0;
    while (chrs[index] != 0)
    {
        if (CursorPosition.x >= framebuffer->Width)
        {
            CursorPosition.x = 0;
            CursorPosition.y += 16;
        }

        if (chrs[index] == '\n')
        {
            CursorPosition.y += 16;
        }
        else if (chrs[index] == '\r')
        {
            CursorPosition.x = 0;
        }
        else if (chrs[index] == '{' && allowEscape && var != NULL)
        {
            if (chrs[index + 1] == '}')
            {
                printStr(var);
                index++;
            }
        }
        else if (chrs[index] == '\\' && allowEscape)
        {
            if (chrs[index + 1] == '\\')
            {
                index++;
                putChar(chrs[index], CursorPosition.x, CursorPosition.y);
                CursorPosition.x += 8;
            }
            else if (chrs[index + 1] == '%')
            {
                index++;
                putChar('%', CursorPosition.x, CursorPosition.y);
                CursorPosition.x += 8;
            }
            else if (chrs[index + 1] == '{')
            {
                index++;
                putChar('{', CursorPosition.x, CursorPosition.y);
                CursorPosition.x += 8;
            }
            else if (chrs[index + 1] == '}')
            {
                index++;
                putChar('{', CursorPosition.x, CursorPosition.y);
                CursorPosition.x += 8;
            }
            else if (chrs[index + 1] == 'C')
            {
                index++;
                if (chrs[index + 1] == 0 || chrs[index + 2] == 0 || chrs[index + 3] == 0 || chrs[index + 4] == 0 || chrs[index + 5] == 0 || chrs[index + 6] == 0)
                {
                    putChar('?', CursorPosition.x, CursorPosition.y);
                }
                else
                {
                    index++;
                    color = ConvertStringToHex(&chrs[index]);
                    index += 5;
                }
            }
            else
            {
                putChar(chrs[index], CursorPosition.x, CursorPosition.y);
                CursorPosition.x += 8;
            }
        }
        else
        {
            putChar(chrs[index], CursorPosition.x, CursorPosition.y);
            CursorPosition.x += 8;
        }

        index++;
    }
}

void BasicRenderer::Println()
{
    BasicRenderer::printStr("\n\r");
}

void BasicRenderer::Print(char chr)
{
    char temp[] = {chr, 0};

    BasicRenderer::Print((const char *)temp, false);
}

void BasicRenderer::Print(const char *chrs)
{
    BasicRenderer::printStr(chrs);
}

void BasicRenderer::Print(const char *chrs, bool allowEscape)
{
    BasicRenderer::printStr(chrs, allowEscape);
}

void BasicRenderer::Println(const char *chrs)
{
    BasicRenderer::printStr(chrs);
    BasicRenderer::printStr("\n\r");
}

void BasicRenderer::Print(const char *chrs, const char *var)
{
    BasicRenderer::printStr(chrs, var);
}

void BasicRenderer::Println(const char *chrs, const char *var)
{
    BasicRenderer::printStr(chrs, var);

    BasicRenderer::printStr("\n\r");
}

void BasicRenderer::Print(const char *chrs, uint32_t col)
{
    uint64_t tempcol = color;
    color = col;

    BasicRenderer::printStr(chrs);

    color = tempcol;
}

void BasicRenderer::Println(const char *chrs, uint32_t col)
{
    uint64_t tempcol = color;
    color = col;

    BasicRenderer::printStr(chrs);
    BasicRenderer::printStr("\n\r");

    color = tempcol;
}

void BasicRenderer::Print(const char *chrs, const char *var, uint32_t col)
{
    uint64_t tempcol = color;
    color = col;

    BasicRenderer::printStr(chrs, var);

    color = tempcol;
}

void BasicRenderer::Println(const char *chrs, const char *var, uint32_t col)
{
    uint64_t tempcol = color;
    color = col;

    BasicRenderer::printStr(chrs, var);
    BasicRenderer::printStr("\n\r");

    color = tempcol;
}

void BasicRenderer::Clear(uint32_t col, bool resetCursor)
{
    uint64_t fbBase = (uint64_t)framebuffer->BaseAddress;
    uint64_t bytesPerScanline = framebuffer->PixelsPerScanLine * 4;
    uint64_t fbHeight = framebuffer->Height;
    uint64_t fbSize = framebuffer->BufferSize;

    for (uint64_t verticalScanline = 0; verticalScanline < fbHeight; verticalScanline++)
    {
        uint64_t pixPtrBase = fbBase + (bytesPerScanline * verticalScanline);
        for (uint32_t *pixPtr = (uint32_t *)pixPtrBase; pixPtr < (uint32_t *)(pixPtrBase + bytesPerScanline); pixPtr++)
            *pixPtr = col;
    }

    if (resetCursor)
        CursorPosition = {0, 0};
}

void BasicRenderer::Clear(uint32_t col)
{
    BasicRenderer::Clear(col, true);
}

void BasicRenderer::Cls()
{
    BasicRenderer::Clear(0);
    BasicRenderer::Println("fynn", Colors.green);
    BasicRenderer::Println("-------------", Colors.green);
    BasicRenderer::Println();
}



void BasicRenderer::Println(const char* chrs, dispVar vars[])
{
    Print(chrs, vars);
    Println();
}
 
 

void BasicRenderer::PrintArg(dispVar var)
{
    const char* toPrint = "<IDK>";
    if (var.type == varType::_bool)
        toPrint = to_string(var._bool);
    else if (var.type == varType::_int)
        toPrint = to_string(var._int);
    else if (var.type == varType::_int64)
        toPrint = to_string(var._int64);
    else if (var.type == varType::_uint64)
        toPrint = to_string(var._uint64);
    else if (var.type == varType::_double)
        toPrint = to_string(var._double);
    else if (var.type == varType::_charPointer)
        toPrint = var._charPointer;

    GlobalRenderer->Print(toPrint);
}


void BasicRenderer::Print(const char* chrs, dispVar vars[])
{
    bool allowEscape = true;
    unsigned int index = 0;
    while (chrs[index] != 0)
    {
        if (CursorPosition.x >= framebuffer->Width)
        {
            CursorPosition.x = 0;
            CursorPosition.y += 16;
        }

        if (chrs[index] == '\n')
        {
            CursorPosition.y += 16;
        }
        else if (chrs[index] == '\r')
        {
            CursorPosition.x = 0;
        }
        else if (chrs[index] == '{' && allowEscape && vars != NULL)
        {
            uint64_t start = index;
            while (chrs[index] != 0 && chrs[index] != '}')
                index++;
            if (chrs[index] == '}')
            {
                char* sub = StrSubstr(chrs, start+1, index-(start+1));
                //GlobalRenderer->Print("<");
                //GlobalRenderer->Print(sub);
                //GlobalRenderer->Print(">");
                uint64_t indx = to_int(sub);
                free(sub);
                if (chrs[index] == '}')
                {
                    PrintArg(vars[indx]);
                    index++;
                }
            }
            index--;
        }
        else if (chrs[index] == '\\' && allowEscape)
        {
            if (chrs[index + 1] == '\\')
            {
                index++;
                putChar(chrs[index], CursorPosition.x, CursorPosition.y);
                CursorPosition.x += 8;
            }
            else if (chrs[index + 1] == '%')
            {
                index++;
                putChar('%', CursorPosition.x, CursorPosition.y);
                CursorPosition.x += 8;
            }
            else if (chrs[index + 1] == '{')
            {
                index++;
                putChar('{', CursorPosition.x, CursorPosition.y);
                CursorPosition.x += 8;
            }
            else if (chrs[index + 1] == '}')
            {
                index++;
                putChar('{', CursorPosition.x, CursorPosition.y);
                CursorPosition.x += 8;
            }
            else if (chrs[index + 1] == 'C')
            {
                index++;
                if (chrs[index + 1] == 0 || chrs[index + 2] == 0 || chrs[index + 3] == 0 || chrs[index + 4] == 0 || chrs[index + 5] == 0 || chrs[index + 6] == 0)
                {
                    putChar('?', CursorPosition.x, CursorPosition.y);
                }
                else
                {
                    index++;
                    color = ConvertStringToHex(&chrs[index]);
                    index += 5;
                }
            }
            else
            {
                putChar(chrs[index], CursorPosition.x, CursorPosition.y);
                CursorPosition.x += 8;
            }
        }
        else
        {
            putChar(chrs[index], CursorPosition.x, CursorPosition.y);
            CursorPosition.x += 8;
        }

        index++;
    }
}