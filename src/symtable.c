#include "symtable.h"

/* copies source[start] to dest[0] */
/* adjusts for \0  */
/* returns number of chars moved */
int sh(char *dest, char *src, int start)
{
    int d = 0;

    do
    {
        dest[d++] = src[start++];
    } while (src[start] != '\0');

    dest[d++] = '\0';
    return d - 1;
}
Symbol::Entry Table[MAXSYMTAB];

Symbol::Symbol() : MAXMEMORY(2047), DEFKIND(FLOAT), DEFSIZE(1)
{
    sout.open("symbol.dat");
}

int Symbol::install(Name nm, KindType k, int s)
{
    int i = 0;
    strcpy(Table[nextsymtab].n, nm);

    while (strcmp(nm, Table[i].n))
        i++;

    if ((i < nextsymtab) || (nextloc + s > MAXMEMORY + 1) || (nextsymtab >= MAXSYMTAB))
        return 0;

    Table[i].a.kind = k;
    switch (k)
    {
    case PROGNAME:
        Table[i].a.size = 0;
        break;
    case LINENUM:
        Table[i].a.size = 1;
        putdata(nextloc, s);
        break;
    case STRING:
        break;
    case FLOAT:
        Table[i].a.size = 1;
        putdata(nextloc, 0.0);
        break;
    }

    Table[i].a.loc = nextloc;
    nextloc += Table[i].a.size;
    nextsymtab++;
    return 1;
}

int Symbol::lookup(Name nm, Attrib &a)
{
    int x, aflag = 0, m1, m2, index;
    Name tnm, fnm, tindex, newindex;
    Attrib arrattr;
    float ivalue;

    for (x = 0; x < strlen(nm); x++)
    {
        if (nm[x] == '[')
        {
            aflag = 1;
            m1 = x;
        }
        else if (aflag == 1 && nm[x] == ']')
        {
            aflag = 2;
            m2 = x;
        }
    }

    if (aflag == 1)
        aflag = 0;

    if (aflag)
    {
        strcpy(tnm, nm);
        sh(tnm, tnm, m1 + 1);
        strncpy(tindex, tnm, m2 - m1 - 1);
        tindex[m2 - m1 - 1] = '\0';

        if (isalpha(tindex[0]))
        {
            lookup(tindex, arrattr);
            ivalue = getdata(arrattr.loc);
            index = static_cast<int>(ivalue);

            strncpy(fnm, nm, m1 + 1);
            fnm[m1 + 1] = '\0';
            snprintf(newindex, 8, "%d", index);
            strcat(fnm, newindex);
            sh(tnm, tnm, m2 - m1 - 1);
            strcat(fnm, tnm);

            nm = fnm;
        }
    }

    for (x = 0; x < nextsymtab; x++)
    {
        if (!strcmp(nm, Table[x].n))
        {
            a = Table[x].a;
            return 1;
        }
    }

    install(nm, DEFKIND, DEFSIZE);
    return 0;
}
int Symbol::dump()
{
    sout << '\n';
    sout << "symbol        kind    length  location\n\n";
    for (int i = 0; i < nextsymtab; i++)
    {
        sout << Table[i].n;
        sout << std::setw(15 - strlen(Table[i].n)) << " ";
        sout << Table[i].a.kind;
        sout << "       ";
        sout << Table[i].a.size;
        sout << "       ";
        sout << Table[i].a.loc << '\n';
    }
    return 0;
}

int Symbol::init()
{
    nextsymtab = 0;
    nextloc = 0;
    return 0;
}
