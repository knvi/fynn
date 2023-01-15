#include "cmdParse.h"
#include <stdlib.h>
#include <stdint.h>
#include "../BasicRenderer.h"
#include "../paging/PageFrameAllocator.h"
#include "../userinput/keyboard.h"
#include "strTools.h"

void LogError(const char* msg) {
    GlobalRenderer->Println(msg, Colors.bred);
};

void LogError(const char* msg, const char* var) {
    GlobalRenderer->Println(msg, var, Colors.bred);
};

void ParseCommand(char* input) {
    if(StrEquals(input, "help")) {
        GlobalRenderer->Print("help - Displays this message\n\r");
    } else {
        GlobalRenderer->Print("Unknown command");
    }
}
