#pragma once

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include "memory.h"

const int MAXSYMBOLLEN = 12;
const int MAXSYMTAB = 256; // max number of entries in the table

class Symbol : public Memory
{
public:
    enum KindType
    {
        PROGNAME,
        LINENUM,
        STRING,
        FLOAT
    };

    using Name = char[MAXSYMBOLLEN];

    struct Attrib
    {
        KindType kind;
        int size, loc;
    };

    struct Entry
    {
        Name n;
        Attrib a;
    };

    Symbol();
    int install(Name, KindType, int);
    int lookup(Name, Attrib &);
    int dump();
    int init();

private:
    std::ofstream sout;
    int MAXMEMORY;
    int DEFSIZE;
    int nextsymtab, nextloc;
    KindType DEFKIND;
};
