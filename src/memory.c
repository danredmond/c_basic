#include "memory.h"

// member functions for Class Code
char *Code::getcode(int loc)
{
    if (loc < 1 || loc >= nextcode)
    {
        std::cerr << "Memory error in memory.c - getcode\n\n";
        return nullptr;
    }
    return cspace[loc];
}

int Code::putcode(char *codestr)
{
    if (nextcode > MAXLINES)
    {
        std::cerr << "Memory error in memory.c - putcode\n\n";
        return 0;
    }
    strcpy(cspace[nextcode], codestr);
    nextcode++;
    return nextcode == MAXLINES + 1 ? 0 : 1;
}

// member functions for Class Data
float Data::getdata(int loc)
{
    if (loc < 0 || loc > nextdata)
    {
        std::cerr << "Memory error in memory.c - getdata\n\n";
        return 0.0f;
    }
    return dspace[loc];
}

int Data::putdata(int loc, float dat)
{
    if (loc < 0 || loc > nextdata)
    {
        std::cerr << "Memory error in memory.c - putdata\n\n";
        return 0;
    }
    dspace[loc] = dat;
    nextdata++;
    return nextdata == MAXLINES + 1 ? 0 : 1;
}

int Data::assdata(int loc, float dat)
{
    if (loc < 0 || loc > nextdata)
    {
        std::cerr << "Memory error in memory.c - assdata\n\n";
        return 0;
    }
    dspace[loc] = dat;
    return 1;
}

// member functions for Class Stack
int Stack::push(float dat)
{
    if (nextstack > MAXSTACK)
    {
        std::cerr << "Memory error in memory.c - push\n\n";
        return 0;
    }
    sspace[nextstack++] = dat;
    return nextstack == MAXSTACK + 1 ? 0 : 1;
}

void Stack::pop(float &dat)
{
    if (nextstack < 1)
    {
        std::cerr << "Memory error in memory.c - pop\n\n";
        return;
    }
    dat = sspace[--nextstack];
}

Memory::Memory() {}
