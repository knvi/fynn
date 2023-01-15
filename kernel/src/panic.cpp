#include "panic.h"
#include "BasicRenderer.h"
#include "Colors.h"

void Panic(const char* panicMessage, const char* var, bool lock) {
    GlobalRenderer->Clear(Colors.red);
    GlobalRenderer->Print("PANIC: ", Colors.white);
    GlobalRenderer->Println();
    GlobalRenderer->Println(panicMessage, var, Colors.white);

    if(lock) {
        while(true) {
          asm("hlt");
        }
    }
};

// overloads
void Panic(const char* panicMessage, bool lock) {
    Panic(panicMessage, NULL, lock);
}

void Panic(const char* panicMessage, const char* var) {
    Panic(panicMessage, var, true);
}

void Panic(const char* panicMessage) {
    Panic(panicMessage, NULL, true);
}