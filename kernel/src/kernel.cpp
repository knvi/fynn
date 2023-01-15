#include "kernelUtil.h"

extern "C" void _start(BootInfo *bootInfo) {
    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;
   
    GlobalRenderer->Cls();

    GlobalRenderer->Println("Kernel Initialised Successfully!!", Colors.yellow);

    KeyboardPrintStart();

    while(true)
    {
        ProcessMousePacket();
    }
    
    while(true); 
}