#include "kernelUtil.h"

extern "C" void _start(BootInfo *bootInfo) {
    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager *pageTableManager = kernelInfo.pageTableManager;

    osData.kernelInfo = &kernelInfo;
    osData.exit = false;

    currentUser = &adminUser;

    GlobalRenderer->Cls();

    // {
    //     dispVar vars[] = {dispVar("test"), dispVar(2), dispVar(0)};
    //     GlobalRenderer->Println("This is a {0}! Also {2}+2={1}.", vars);
    // }

    // {
    //     dispVar vars[] = {dispVar("cool thing"), dispVar(4), dispVar(2)};
    //     GlobalRenderer->Println("This is a {0}! Also {2}+2={1}.", vars);
    // }

    GlobalRenderer->Println("TIME: {} s", to_string(PIT::TimeSinceBoot), Colors.bred);
    GlobalRenderer->Println("TIME: {} ms", to_string((int)(PIT::TimeSinceBoot*1000)), Colors.bred);
    GlobalRenderer->Println("DIV:  {}", to_string(PIT::Divisor), Colors.bred);
    GlobalRenderer->Println("FREQ: {} Hz", to_string(PIT::freq), Colors.bred);

    GlobalRenderer->Println();

    for (int i = 0; i < 20; i++)
    {
        GlobalRenderer->Print("hoi! ");
        PIT::Sleep(100);
    }

    GlobalRenderer->Println();
    GlobalRenderer->Println();

    GlobalRenderer->Println("TIME: {} s", to_string(PIT::TimeSinceBoot), Colors.bred);
    GlobalRenderer->Println("TIME: {} ms", to_string((int)(PIT::TimeSinceBoot*1000)), Colors.bred);
    GlobalRenderer->Println("DIV:  {}", to_string(PIT::Divisor), Colors.bred);
    GlobalRenderer->Println("FREQ: {} Hz", to_string(PIT::freq), Colors.bred);

    GlobalRenderer->Println("Kernel Initialised Successfully!!", Colors.yellow);

    KeyboardPrintStart();

    while(!osData.exit)
    {
        ProcessMousePacket();
    }

    GlobalRenderer->Clear(Colors.black);
    GlobalRenderer->Println("Goodbye.");
    while(!osData.exit || true);
}