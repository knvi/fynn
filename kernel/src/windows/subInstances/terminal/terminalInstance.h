#pragma once
#include "../../../osData/userdata.h"
#include "../default/defaultInstance.h"

class TerminalInstance : public DefaultInstance
{
    public:
    OSUser* currentUser;
    char lastTerminalInput[256];
    char terminalInput[256];
    uint64_t userlen;
    TerminalInstance(OSUser* user);

};