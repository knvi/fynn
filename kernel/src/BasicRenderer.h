#pragma once


#include "math.h"
#include "Framebuffer.h"
#include "simpleFonts.h"
#include <stdint.h> 


class BasicRenderer
{
    public:
        
        Point CursorPosition;
        Framebuffer* framebuffer;
        PSF1_FONT* psf1_font;
        unsigned int color;
        bool overwrite = false;
        void putChar(char chr, unsigned int xoff, unsigned int yoff);
        void delChar(unsigned int xoff, unsigned int yoff, uint32_t col);
        void delChar(unsigned int xoff, unsigned int yoff);
        void putStr(const char* chrs, unsigned int xoff, unsigned int yoff);

        void printStr(const char* chrs);
        void printStr(const char* chrs, const char* var);
 
        void Println();

        void Print(char chr);
        void Print(const char* chrs);
        void Println(const char* chrs);
        void Print(const char* chrs, const char* var);
        void Println(const char* chrs, const char* var);
        

        void Print(const char* chrs, uint32_t col);
        void Println(const char* chrs, uint32_t col);
        void Print(const char* chrs, const char* var, uint32_t col);
        void Println(const char* chrs, const char* var, uint32_t col);

        void Clear(uint32_t col);
        void Clear(uint32_t col, bool resetCursor);

        void Cls();


        BasicRenderer(Framebuffer* framebuffer, PSF1_FONT* psf1_font)
        {
            color = 0xffffffff;
            CursorPosition = {0, 0};
            this->framebuffer = framebuffer;
            this->psf1_font = psf1_font;
        }

        
};

extern BasicRenderer* GlobalRenderer;