#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo)
{  
    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    osData.kernelInfo = &kernelInfo;
    osData.exit = false;
    osData.windows = List<Window*>();

    Window* mainWindow;
    {
        mainWindow = (Window*)malloc(sizeof(Window));
        TerminalInstance* terminal = (TerminalInstance*)malloc(sizeof(TerminalInstance));
        *terminal = TerminalInstance(&adminUser);
        *(mainWindow) = Window((DefaultInstance*)terminal, Size(600, 500), Position(10, 10), GlobalRenderer->framebuffer);
        osData.windows.add(mainWindow);

        activeWindow = mainWindow;
    }

    {
        Window* window = (Window*)malloc(sizeof(Window));
        TerminalInstance* terminal = (TerminalInstance*)malloc(sizeof(TerminalInstance));
        *terminal = TerminalInstance(&guestUser);
        *(window) = Window((DefaultInstance*)terminal, Size(480, 360), Position(400, 60), GlobalRenderer->framebuffer);
        osData.windows.add(window);
    }


    // osData.windows[1]->renderer->Clear(Colors.black);
    // osData.windows[1]->renderer->color = Colors.white;

    osData.windows[1]->renderer->Cls();    
    osData.windows[1]->renderer->Println("Hello, world!");
    KeyboardPrintStart(osData.windows[1]);
    

    //activeWindow = osData.windows[1];


    activeWindow->renderer->Cls();
    activeWindow->renderer->Println("Kernel Initialised Successfully!", Colors.yellow);
    KeyboardPrintStart(mainWindow);
    mainWindow->Render();

    while(!osData.exit)
    {
        //GlobalRenderer->Clear(Colors.black);

        for (int i = 0; i < osData.windows.getCount(); i++)
            osData.windows[i]->Render();

        PIT::Sleep(200);
        //asm("hlt");
    }

    GlobalRenderer->Clear(Colors.black);
    GlobalRenderer->color = Colors.white;
    GlobalRenderer->Println("Goodbye.");
    PIT::Sleep(1000);

}