#include "cmdParse.h"
#include <stdint.h>
#include "../memory/heap.h"
#include "../BasicRenderer.h"
#include "../paging/PageFrameAllocator.h"
#include "../userinput/keyboard.h"
#include "../userinput/mouse.h"
#include "strTools.h"
#include "../osData/osdata.h"
#include "../pit/pit.h"

void LogError(const char *msg)
{
    GlobalRenderer->Println(msg, Colors.bred);
};

void LogError(const char *msg, const char *var)
{
    GlobalRenderer->Println(msg, var, Colors.bred);
};

void LogInvalidArgumentCount(int expected, int found)
{
    GlobalRenderer->Print("Invalid Argument count. Expected {} but got ", to_string((uint64_t)expected), Colors.bred);
    GlobalRenderer->Println("{} instead.", to_string((uint64_t)found), Colors.bred);
}

void ParseCommand(char *input, char *oldInput, OSUser **user)
{
    if (StrEquals(input, "cls"))
    {
        GlobalRenderer->Cls();
        return;
    }

    if (StrEquals(input, "malloc"))
    {
        malloc(0x5000);
        return;
    }

    if (StrEquals(input, "exit"))
    {
        GlobalRenderer->Println("Exiting...");
        osData.exit = true;
        return;
    }

    StringArrData *data = SplitLine(oldInput);

    if (StrEquals(data->data[0], "login") && (*user)->mode == commandMode::enterPassword)
    {
        (*user)->mode = commandMode::none;
        StringArrData *data2 = SplitLine(input);
        if (data->len == 2)
        {
            if (data2->len == 1)
                login(data->data[1], data2->data[0], user);
            else
                LogError("Password can only be one Argument long!");
        }

        free(data);
        free(data2);
        return;
    }

    if (StrEquals(data->data[0], "set") && (*user)->mode == commandMode::enterPassword)
    {
        (*user)->mode = commandMode::none;
        StringArrData *data2 = SplitLine(input);
        if (data->len == 2 || data->len == 3)
        {
            if (StrEquals(data->data[1], "password"))
            {
                if (data2->len == 1)
                {
                    (*user)->password = StrCopy(data2->data[0]);
                    // GlobalRenderer->Println("Password is now \"{}\".", (*user)->password, Colors.yellow);
                }
                else
                    LogError("Password can only be one Argument long!");
            }
        }

        free(data);
        free(data2);
        return;
    }

    free(data);
    data = SplitLine(input);

    if (data->len == 0)
    {
        free(data);
        return;
    }

    if (StrEquals(data->data[0], "echo"))
    {
        if (data->len == 2)
        {
            GlobalRenderer->Println(data->data[1]);
        }
        else
        {
            LogInvalidArgumentCount(1, data->len - 1);
        }
        free(data);
        return;
    }

    if (StrEquals(data->data[0], "sleep"))
    {
        if (data->len == 2)
        {
            int64_t time = to_int(data->data[1]);
            if (time > 0)
            {
                GlobalRenderer->Println("Sleeping for {} ms...", to_string(time), Colors.yellow);
                // PIT::Sleep((uint64_t)time);
                {

                    GlobalRenderer->Println("TIME: {} s", to_string(PIT::TimeSinceBoot), Colors.bred);
                    GlobalRenderer->Println("TIME: {} ms", to_string((int)(PIT::TimeSinceBoot * 1000)), Colors.bred);
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
                    GlobalRenderer->Println("TIME: {} ms", to_string((int)(PIT::TimeSinceBoot * 1000)), Colors.bred);
                    GlobalRenderer->Println("DIV:  {}", to_string(PIT::Divisor), Colors.bred);
                    GlobalRenderer->Println("FREQ: {} Hz", to_string(PIT::freq), Colors.bred);
                }
            }
            else
                LogError("You cannot sleep less than 0ms!");
        }
        else
            LogInvalidArgumentCount(1, data->len - 1);

        free(data);
        return;
    }

    if (StrEquals(data->data[0], "set"))
    {
        if (data->len == 3)
            SetCommand(data->data[1], data->data[2], user);
        else if (data->len == 2 && StrEquals(data->data[1], "password"))
            SetCommand(data->data[1], "", user);
        else
            LogInvalidArgumentCount(2, data->len - 1);

        free(data);
        return;
    }

    if (StrEquals(data->data[0], "get"))
    {
        if (data->len == 2)
            GetCmd(data->data[1], *user);
        else
            LogInvalidArgumentCount(1, data->len - 1);

        free(data);
        return;
    }

    if (StrEquals(data->data[0], "login"))
    {
        if (data->len == 2)
            login(data->data[1], user);
        else
            LogInvalidArgumentCount(1, data->len - 1);

        free(data);
        return;
    }
    LogError("Unknown command \"{}\"!", data->data[0]);
    free(data);
    return;
}

void login(const char *name, OSUser **user)
{
    OSUser *usr = getUser(name);
    if (usr == 0)
    {
        LogError("User \"{}\" was not found!", name);
        return;
    }

    if (StrEquals(usr->password, ""))
        *user = usr;
    else
    {
        GlobalRenderer->Println("Please enter the password down below:");
        (*user)->mode = commandMode::enterPassword;
    }
}

void login(const char *name, const char *pass, OSUser **user)
{
    (*user)->mode = commandMode::none;

    OSUser *usr = getUser(name);
    if (usr == 0)
    {
        LogError("User \"{}\" was not found!", name);
        return;
    }

    if (StrEquals(usr->password, pass))
        *user = usr;
    else
        LogError("Password is incorrect!", name);
}

void SetCommand(const char *name, const char *value, OSUser **user)
{
    if (StrEquals(name, "user color") || StrEquals(name, "usercol"))
    {
        ParsedColData data = ParseColor(value);
        if (data.parseSuccess)
        {
            (*user)->colData.userColor = data.col;
        }
        else
        {
            LogError("Color \"{}\" could not be parsed! ", value);
        }
    }
    else if (StrEquals(name, "username"))
    {
        (*user)->userName = StrCopy(value);
    }
    else if (StrEquals(name, "password"))
    {
        (*user)->mode = commandMode::enterPassword;
        GlobalRenderer->Println("Please enter the new password down below:");
    }
    else if (StrEquals(name, "mouse color front"))
    {
        ParsedColData data = ParseColor(value);
        if (data.parseSuccess)
            mouseColFront = data.col;
        else
            LogError("Color \"{}\" could not be Parsed!", value);
    }
    else if (StrEquals(name, "mouse color back"))
    {
        ParsedColData data = ParseColor(value);
        if (data.parseSuccess)
            mouseColBack = data.col;
        else
            LogError("Color \"{}\" could not be Parsed!", value);
    }
    else
    {
        LogError("Unknown command \"{}\"! ", name);
    }
}

void GetCmd(const char *name, OSUser *user)
{
    if (StrEquals(name, "free ram"))
    {
        GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    }
    else if (StrEquals(name, "free pages"))
    {
        GlobalRenderer->Println("Free Page Count: {} pages.", to_string(GlobalAllocator->GetFreePageCount()), Colors.bgreen);
    }
    else
    {
        LogError("Parameter \"{}\" does not exist.", name);
    }

    // GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    // GlobalRenderer->Println("");
}

ParsedColData ParseColor(const char *color)
{
    ParsedColData data = ParsedColData();
    data.parseSuccess = false;
    data.col = 0;

    if (color[0] == '#')
    {
        const char *subStr = StrSubstr(color, 1);
        // GlobalRenderer->Println("Col: \"{}\"", subStr, Colors.cyan);
        data.col = ConvertStringToHex(subStr);
        data.parseSuccess = true;
        return data;
    }

    for (int i = 0; i < colCount; i++)
        if (StrEquals(color, colNames[i]))
        {
            data.col = colValues[i];
            data.parseSuccess = true;
            break;
        }
    return data;
}

StringArrData *SplitLine(char *input)
{
    uint64_t index = 0;
    uint64_t parts[100];
    uint64_t partIndex = 0;

    for (int i = 0; i < 100; i++)
        parts[i] = 0;

    bool inString = false;

    for (; input[index] != 0; index++)
    {
        if (input[index] == '\"')
            inString = !inString;
        else if (!inString && input[index] == ' ')
            partIndex++;
        else
        {
            parts[partIndex]++;
            if (input[index] == '\\')
                if (input[index + 1] != 0)
                    index++;
        }
    }

    int partCount = partIndex + 1;

    int totalsize = sizeof(StringArrData);
    for (int i = 0; i < partCount; i++)
    {
        totalsize += parts[i] + 1 + sizeof(char *);
    }

    // uint64_t datAddr = (uint64_t) GlobalAllocator->RequestPage();
    int64_t datAddr = (uint64_t)malloc(totalsize);
    StringArrData *data = (StringArrData *)datAddr;
    data->addrOfData = (void *)(datAddr + sizeof(StringArrData));

    data->data = (char **)data->addrOfData;

    char **splitLine = data->data;
    data->len = partCount;
    uint64_t splitIndex = sizeof(char **) + sizeof(char *) * partCount;

    for (int i = 0; i < partCount; i++)
    {
        splitLine[i] = (char *)(data->addrOfData + splitIndex);
        for (int i2 = 0; i2 < parts[i] + 1; i2++)
            splitLine[i][i2] = 0;
        splitIndex += parts[i] + 1;
    }

    inString = false;
    partIndex = 0;
    index = 0;

    for (int i = 0; i < 100; i++)
        parts[i] = 0;

    for (; input[index] != 0; index++)
    {
        if (input[index] == '\"')
            inString = !inString;
        else if (!inString && input[index] == ' ')
        {
            splitLine[partIndex][parts[partIndex]] = 0;
            partIndex++;
        }
        else
        {
            if (input[index] == '\\')
                if (input[index + 1] != 0)
                    index++;

            splitLine[partIndex][parts[partIndex]] = input[index];

            parts[partIndex]++;
        }
    }

    return data;
}
