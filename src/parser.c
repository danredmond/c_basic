// NOTE: Supported grammer in Backus-Naur form
// ===========================================
// <program> -> <lineblock>
// <lineblock> -> <line> (<lineblock)
// <line> -> LNUM <stmtlist>
// <stmtlist> -> <stmt> (COLON <stmt>)
// <stmt> -> IDENT EQUAL <assign>
// <stmt> -> PRINT <printlist>
// <stmt> -> FOR <forindex> TO INTLIT <lineblock> NEXT IDENT
// <stmt> -> INPUT IDENT
// <stmt> -> DIM IDENT
// <stmt> -> IF IDENT EQUAL INTLIT THEN <stmt>
// <stmt> -> END
// <assign> -> O_PAREN <assign> C_PAREN ([PLUS|MINUS|MULT|DIV] <assign>)
// <assign> -> [INTLIT|IDENT] ([PLUS|MINUS|MULT|DIV] <assign>)
// <printlist> -> QUOTE STRING QUOTE|IDENT (,STRING|IDENT,...)
// <forindex> -> IDENT EQUAL <assignment>

#include "parser.h"

Parser::Parser(char *fname)
{
    char tline[255]; // temp line
    std::ifstream newstr;

    myout.open("parse.dat");

    myout << "Parser Initiated: " << std::endl;

    forexit = 0;
    lines = 0;
    newstr.open(fname);

    T.init(); // sets nextsymtab = nextloc = 0
    T.install(fname, Symbol::PROGNAME, 0);

    while (!(newstr.eof()))
    {
        newstr.getline(tline, (MAXLINELEN + 1), '\n'); // get line from file
        if (!(tline[0] == '~'))
        {
            if (strlen(tline) > MAXLINELEN) // not really needed ??
            {
                std::cout << "file error: long line\n";
                exit(0);
            }

            lines++; // increment number of lines

            if (!(T.putcode(tline)))
            {
                std::cout << "file error: too many lines\n";
                exit(0);
            }

            S.init(tline);

            strcpy(tokstring, S.scan(nextoken));

            if (nextoken == Scanner::LNUM)
                if (T.install(tokstring, Symbol::LINENUM, lines) == 0)
                {
                    std::cout << "symbol table error: line number\n";
                    exit(0);
                }
        }
    }
    Parser::level = 0;
}

int Parser::parse()
{
    errorln = 0;
    level = 0;
    char temp[255];
    Parser::end_flag = 0;
    if (matchnterm(PROGRAM))
    {
        myout << "\nParse: no grammer errors found\n";
        T.dump();   // outputs the symbol table
        return (1); // parse = true;
    }
    else
    {
        if (Parser::end_flag == 0)
        {
            myout << "\nParse: error on " << newinst;
        }
        return (0); // parse = false;
    }
}

int Parser::matchtoken(Scanner::Tokentype t)
{
    for (int x = 1; x <= level; x++)
        myout << "  ";
    myout << S.token2string(nextoken);
    //     for (int y = 1;
    //        y <= (40 - strlen(S.token2string(Parser::nextoken)) - (level * 2));
    //        y++)
    myout << " ";
    myout << tokstring << "\n";
    if (t == nextoken)
    {
        do
        {
            strcpy(tokstring, S.scan(nextoken));
            //  std::cout << nextoken << std::endl;
        } while (nextoken == Scanner::COMMENT);
        return (1);
    }
    else
        return (0);
}

const char *Parser::nterm2str(NTtype nt)
{
    switch (nt)
    {
    case PROGRAM:
        return ("<program>\n");
    case LINEBLOCK:
        return ("<lineblock>\n");
    case LINE:
        return ("<line>\n"); // could say <line ###>
    case STMTLIST:
        return ("<stmtlist>\n");
    case STMT:
        return ("<stmt>\n");
    case ASSIGN:
        return ("<assign>\n");
    case PRINTLIST:
        return ("<printlist>\n");
    case FORINDEX:
        return ("<forindex>\n");
    case FORBLOCK:
        return ("<forblock>\n");
    }
    return (" error in Parser::nterm2str(NTtype)");
}

int Parser::matchnterm(NTtype nt)
// output nterm name, inc level x and call nterm function
{
    int ret_val;
    if (Parser::level > 0)
        for (int x = 1; x <= Parser::level; x++)
            myout << "  ";

    myout << nterm2str(nt);
    Parser::level++;
    switch (nt)
    {
    case PROGRAM:
        ret_val = ntprogram();
        break;
    case LINEBLOCK:
        ret_val = ntlineblock();
        break;
    case LINE:
        ret_val = ntline();
        break;
    case STMTLIST:
        ret_val = ntstmtlist();
        break;
    case STMT:
        ret_val = ntstmt();
        break;
    case ASSIGN:
        ret_val = ntassign();
        break;
    case PRINTLIST:
        ret_val = ntprintlist();
        break;
    case FORINDEX:
        ret_val = ntforindex();
        break;
    case FORBLOCK:
        ret_val = ntforblock();
        break;
    }
    level--;
    return (ret_val);
}

int Parser::ntprogram() // controls the execution cycle
                        // <program> -> <lineblock>
{
    oldinst = 1;    // initialize variables to control execution
    newinst = 1;    // flow...these are private and within the
    go_flag = TRUE; // Parser Scope
    lbexit = FALSE; // used to exit a line block

    return (matchnterm(LINEBLOCK));
}

int Parser::ntlineblock() // error tracking here ******
// <lineblock> -> <line> (<lineblock)
// deviation from recursive method
{
    char dummy[255]; // used to make the user push enter;

    do
    { // std::cout << lines;
        if (newinst > lines)
        {
            go_flag = FALSE;
            myout << "Ran out of lines to process - Execution complete";
            // std::cout << "Execution complete";
        }
        else
        {
            myout << "Line " << newinst << std::endl;
            if (!(matchnterm(LINE)))
            {
                go_flag = FALSE;
                errorln = oldinst;
                return (0);
            }

            else
            {
                oldinst = newinst;
                newinst++;
            }
        }
    } while (go_flag && (lbexit == FALSE) && (!end_flag));

    return (1);
}

int Parser::ntline()
// <line> -> LNUM <stmtlist>
{
    S.init(T.getcode(newinst)); // transfers code from code space
                                // to sline (used by scanner::scan
    strcpy(tokstring, S.scan(nextoken));
    if (!(matchtoken(Scanner::LNUM)))
        return (0);
    if (!(matchnterm(STMTLIST)))
        return (0);
    return (1);
}

int Parser::ntstmtlist() // a bit of a deviation from pure
                         // recursive descent..ie i used
                         // iterations to track multiple
                         // statements on one line.

// <stmtlist> -> <stmt> (COLON <stmt>),...      <-  iteration
{
    if (!(matchnterm(STMT)))
        return (0);
    while (nextoken == Scanner::COLON) // maybe take this out
    {
        strcpy(tokstring, S.scan(nextoken));
        if (!(matchnterm(STMT)))
            return (0);
    }
    return (1);
}

int Parser::ntstmt()
// <stmt> -> IDENT EQUAL <assign>
// <stmt> -> PRINT <printlist>
// <stmt> -> FOR <forindex> TO INTLIT <lineblock> NEXT IDENT
// <stmt> -> INPUT IDENT
// <stmt> -> DIM IDENT
{
    Symbol::Attrib attr;
    Symbol::Attrib att2;
    char oldtokstr[255];
    float fl_temp;
    int x, d1, d2, len, dsize, i;
    char dconvert[255];
    char first[255];
    char name[255];
    char cvt[255];
    char cstring[255];
    int cflag;
    float cvalue;
    float tvalue;
    int goflag = 0;
    int tloc;
    int forstart, forlimit, forloop;
    int firstinst;
    char forvar[255];
    char dummy[255];

    switch (nextoken)
    {
    case Scanner::IDENT:
        T.lookup(tokstring, attr);
        strcpy(oldtokstr, tokstring);
        if (!(matchtoken(Scanner::IDENT)))
            return (0);
        if (!(matchtoken(Scanner::EQUAL)))
            return (0);
        if (!(matchnterm(ASSIGN)))
            return (0);
        T.lookup(oldtokstr, att2);
        T.pop(fl_temp);
        T.assdata(att2.loc, fl_temp);
        // now do the assignment to dspace!!!
        return (1);

    case Scanner::PRINT:
    {
        if (!(matchtoken(Scanner::PRINT)))
            return (0);
        if (!(matchnterm(PRINTLIST)))
            return (0);
        return (1);
    }

    case Scanner::PAUSE:
        if (!(matchtoken(Scanner::PAUSE)))
            return (0);
        std::cin >> dummy;
        return (1);

    case Scanner::FOR:
        if (!(matchtoken(Scanner::FOR)))
            return (0);

        strcpy(forvar, tokstring);
        if (!(matchnterm(FORINDEX)))
            return (0);

        T.lookup(forvar, att2);
        T.pop(fl_temp);
        T.assdata(att2.loc, fl_temp);
        // now do the assignment to dspace!!!

        if (!(matchtoken(Scanner::TO)))
            return (0);

        T.lookup(forvar, attr);
        forstart = T.getdata(attr.loc);

        strcpy(oldtokstr, tokstring);

        switch (nextoken)
        {
        case (Scanner::INTLIT):

            if (!(matchtoken(Scanner::INTLIT)))
                return (0);
            forlimit = atoi(oldtokstr);
            break;

        case (Scanner::IDENT):
            if (!(matchtoken(Scanner::IDENT)))
                return (0);
            T.lookup(oldtokstr, att2);
            forlimit = T.getdata(att2.loc);
            break;

        default:
            break;
        }

        newinst++;
        firstinst = newinst;
        for (forloop = forstart;
             forloop <= forlimit;
             forloop++)
        {
            newinst = firstinst;
            T.assdata(attr.loc, forloop);
            if (!(matchnterm(FORBLOCK))) // emsg & cur_lnum
                return (0);

            // matchtoken (lnum) ???? i think so!
        }
        return (1);
        break;

    case Scanner::NEXT:

        if (!(matchtoken(Scanner::NEXT)))
            return (0);
        /* could remove */
        if (!(matchtoken(Scanner::IDENT))) // errchking?
            return (0);

        forexit = 1; // resets the loop;

        return (1);

    case Scanner::INPUT:
        if (!(matchtoken(Scanner::INPUT)))
            return (0);
        strcpy(oldtokstr, tokstring);
        if (!(matchtoken(Scanner::IDENT)))
            return (0);
        if (!(T.lookup(oldtokstr, attr)))
            T.lookup(oldtokstr, attr);
        // could add flush(stdin);
        std::cin >> fl_temp;
        T.assdata(attr.loc, fl_temp);
        return (1);

    case Scanner::DIM:
        if (!(matchtoken(Scanner::DIM)))
            return (0);
        strcpy(oldtokstr, tokstring);
        if (!(matchtoken(Scanner::IDENT)))
            return (0);
        x = 0;
        d1 = 0;
        d2 = 0;
        len = strlen(oldtokstr);
        while ((oldtokstr[x] != '[') &&
               (x < len))
            x++;
        d1 = x;
        x++;
        while ((oldtokstr[x] != ']') &&
               (x < len))
            x++;
        d2 = x;
        if ((d1 + 1) < d2) // case of array[]
        {
            strcpy(dconvert, "");
            i = 0;
            for (x = (d1 + 1); x < d2; x++)
            {
                dconvert[i] = oldtokstr[x];
                i++;
            }
            dconvert[i] = '\0';
            dsize = atof(dconvert);

            for (x = 1; x <= dsize; x++)
            {
                strcpy(name, "");
                strcpy(first, "");
                strncpy(first, oldtokstr, d1);
                first[d1] = '\0';
                strcpy(name, first);
                strcat(name, "[");
                snprintf(cvt, 8, "%d", x);
                strcat(name, cvt);
                strcat(name, "]");
                T.install(name, Symbol::FLOAT, 0);
            }
            return (1);
        }
        else
            return (0);
        break;

    case Scanner::IF:
        if (!(matchtoken(Scanner::IF)))
            return (0);
        strcpy(cstring, tokstring);
        if (!(matchtoken(Scanner::IDENT)))
            return (0);
        tloc = T.lookup(cstring, att2);
        cvalue = T.getdata(att2.loc);
        if (!((nextoken == Scanner::EQUAL) ||
              (nextoken == Scanner::G_THAN) ||
              (nextoken == Scanner::L_THAN)))
            return (0);
        switch (nextoken)
        {
        case Scanner::EQUAL:
            cflag = 0;
            if (!(matchtoken(Scanner::EQUAL)))
                return (0);
            break;
        case Scanner::G_THAN:
            cflag = 1;
            if (!(matchtoken(Scanner::G_THAN)))
                return (0);
            break;
        case Scanner::L_THAN:
            cflag = -1;
            if (!(matchtoken(Scanner::L_THAN)))
                return (0);
            break;
        default:
            break;
        }

        if (nextoken == Scanner::IDENT)
        {
            tloc = T.lookup(tokstring, att2);
            tvalue = T.getdata(att2.loc);
            if (!(matchtoken(Scanner::IDENT)))
                return (0);
        }
        else if (nextoken == Scanner::INTLIT)
        {
            tvalue = atoi(tokstring);
            if (!(matchtoken(Scanner::INTLIT)))
                return (0);
        }
        if (!(matchtoken(Scanner::THEN)))
            return (0);

        switch (cflag)
        {
        case (0):
            if (tvalue == cvalue)
                goflag = 1;
            break;
        case (1):
            if (cvalue > tvalue)
                goflag = 1;
            break;
        case (-1):
            if (cvalue < tvalue)
                goflag = 1;
            break;
        default:
            break;
        }

        if (goflag)
            if (!(matchnterm(STMT)))
                return (0);
        return (1);

    case Scanner::GOTO:
        if (!(matchtoken(Scanner::GOTO)))
            return (0);
        strcpy(oldtokstr, tokstring);
        if (!(matchtoken(Scanner::INTLIT)))
            return (0);
        T.lookup(oldtokstr, attr);
        newinst = T.getdata(attr.loc);
        newinst--; // because it will be inc by lineblock
        return (1);

    case Scanner::END:
        if (!(matchtoken(Scanner::END)))
            return (0);
        Parser::end_flag = 1;
        return (1); // exit program

    default:
        return (0);
    }
}

int Parser::ntassign()
// <assign> -> O_PAREN <assign> C_PAREN ([PLUS|MINUS|MULT|DIV]) <assign>)
// <assign> -> [INTLIT|IDENT] ([PLUS|MINUS|MULT|DIV] <assign>)
{
    Symbol::Attrib attr;
    char oldtokstr[255]; // to stop sigsegv
    float fl1, fl2, fltemp;

    switch (nextoken)
    {
    case Scanner::O_PAREN:
        if (!(matchtoken(Scanner::O_PAREN)))
            return (0);
        if (!(matchnterm(ASSIGN)))
            return (0);
        if (!(matchtoken(Scanner::C_PAREN)))
            return (0);
        if ((nextoken == Scanner::PLUS) ||
            (nextoken == Scanner::MINUS) ||
            (nextoken == Scanner::MULT) ||
            (nextoken == Scanner::DIV))

            switch (nextoken)
            {
            case Scanner::PLUS:
                if (!(matchtoken(Scanner::PLUS)))
                    return (0);
                if (!(matchnterm(ASSIGN)))
                    return (0);

                T.pop(fl1);
                T.pop(fl2);
                T.push(fl2 + fl1);

                return (1);
            case Scanner::MINUS:
                if (!(matchtoken(Scanner::MINUS)))
                    return (0);
                if (!(matchnterm(ASSIGN)))
                    return (0);

                T.pop(fl1);
                T.pop(fl2);
                T.push(fl2 - fl1);

                return (1);
            case Scanner::MULT:
                if (!(matchtoken(Scanner::MULT)))
                    return (0);
                if (!(matchnterm(ASSIGN)))
                    return (0);

                T.pop(fl1);
                T.pop(fl2);
                T.push(fl2 * fl1);

                return (1);
            case Scanner::DIV:
                if (!(matchtoken(Scanner::DIV)))
                    return (0);
                if (!(matchnterm(ASSIGN)))
                    return (0);

                T.pop(fl1);
                T.pop(fl2);
                T.push(fl2 / fl1);

                return (1);

            default:
                break;
            }
        else
            return (1); // if there were no more plus, minus etc..

        break;

    case Scanner::INTLIT:
        strcpy(oldtokstr, tokstring);
        if (!(matchtoken(Scanner::INTLIT)))
            return (0);
        /* ADD TO STACK */
        if (!(T.push(atof(oldtokstr))))
        {
            std::cout << "\nSTACK FULL: ntassign\n";
            return (0);
        }

        if (!(nextoken == Scanner::EOSTR))
            if ((nextoken == Scanner::PLUS) ||
                (nextoken == Scanner::MINUS) ||
                (nextoken == Scanner::MULT) ||
                (nextoken == Scanner::DIV))

                switch (nextoken)
                {
                case Scanner::PLUS:
                    if (!(matchtoken(Scanner::PLUS)))
                        return (0);
                    if (!(matchnterm(ASSIGN)))
                        return (0);

                    T.pop(fl1);
                    T.pop(fl2);
                    T.push(fl2 + fl1);

                    return (1);
                case Scanner::MINUS:
                    if (!(matchtoken(Scanner::MINUS)))
                        return (0);
                    if (!(matchnterm(ASSIGN)))
                        return (0);

                    T.pop(fl1);
                    T.pop(fl2);
                    T.push(fl2 - fl1);

                    return (1);
                case Scanner::MULT:
                    if (!(matchtoken(Scanner::MULT)))
                        return (0);
                    if (!(matchnterm(ASSIGN)))
                        return (0);

                    T.pop(fl1);
                    T.pop(fl2);
                    T.push(fl2 * fl1);

                    return (1);
                case Scanner::DIV:
                    if (!(matchtoken(Scanner::DIV)))
                        return (0);
                    if (!(matchnterm(ASSIGN)))
                        return (0);

                    T.pop(fl1);
                    T.pop(fl2);
                    T.push(fl2 / fl1);

                    return (1);

                default:
                    break;
                }

        break;

    case Scanner::IDENT:
        strcpy(oldtokstr, tokstring);

        T.lookup(tokstring, attr);

        if (!(matchtoken(Scanner::IDENT)))
            return (0);

        /* ADD TO STACK */
        fltemp = T.getdata(attr.loc);
        if (!(T.push(fltemp)))
        {
            std::cout << "\nSTACK FULL: ntassign\n";
            return (0);
        }

        if ((nextoken == Scanner::PLUS) ||
            (nextoken == Scanner::MINUS) ||
            (nextoken == Scanner::MULT) ||
            (nextoken == Scanner::DIV))

            switch (nextoken)
            {
            case Scanner::PLUS:
                if (!(matchtoken(Scanner::PLUS)))
                    return (0);
                if (!(matchnterm(ASSIGN)))
                    return (0);
                T.pop(fl1);
                T.pop(fl2);
                T.push(fl2 + fl1);
                return (1);
            case Scanner::MINUS:
                if (!(matchtoken(Scanner::MINUS)))
                    return (0);
                if (!(matchnterm(ASSIGN)))
                    return (0);

                T.pop(fl1);
                T.pop(fl2);
                T.push(fl2 - fl1);
                return (1);
            case Scanner::MULT:
                if (!(matchtoken(Scanner::MULT)))
                    return (0);
                if (!(matchnterm(ASSIGN)))
                    return (0);

                T.pop(fl1);
                T.pop(fl2);
                T.push(fl2 * fl1);
                return (1);
            case Scanner::DIV:
                if (!(matchtoken(Scanner::DIV)))
                    return (0);
                if (!(matchnterm(ASSIGN)))
                    return (0);

                T.pop(fl1);
                T.pop(fl2);
                T.push(fl2 / fl1);
                return (1);

            default:
                break;
            }
        break;
    default:
        return (0); // error:set emsg _ clnum (invalid assignment)
    }
    return (1);
}

int Parser::ntprintlist()
// <printlist> -> QUOTE STRING QUOTE|IDENT (,"STRING"|IDENT,...)
// iteration again
{
    float fltemp;
    char oldtokstr[255];
    Symbol::Attrib attr;

    if (!((nextoken == Scanner::QUOTE) || (nextoken == Scanner::IDENT)))
        return (0);
    if (nextoken == Scanner::QUOTE)
    {
        if (!(matchtoken(Scanner::QUOTE))) // NOT REQUIRED HERE
            return (0);                    //         <-----
        strcpy(oldtokstr, tokstring);
        if (!(matchtoken(Scanner::STRING)))
            return (0);
        if (!(matchtoken(Scanner::QUOTE)))
            return (0);
        std::cout << oldtokstr << " ";
    }
    else
    {
        strcpy(oldtokstr, tokstring);
        if (!(matchtoken(Scanner::IDENT)))
            return (0);
        T.lookup(oldtokstr, attr);
        fltemp = T.getdata(attr.loc);
        std::cout << fltemp << " ";
    }

    while (nextoken == Scanner::COMMA)
    {
        if (!(matchtoken(Scanner::COMMA)))
            return (0);
        if (!((nextoken == Scanner::QUOTE) || (nextoken == Scanner::IDENT)))
            return (0);
        if (nextoken == Scanner::QUOTE)
        {
            if (!(matchtoken(Scanner::QUOTE))) // NOT REQUIRED HERE
                return (0);                    //         <-----
            strcpy(oldtokstr, tokstring);
            if (!(matchtoken(Scanner::STRING)))
                return (0);
            if (!(matchtoken(Scanner::QUOTE)))
                return (0);
            std::cout << oldtokstr << " ";
        }
        else
        {
            strcpy(oldtokstr, tokstring);
            if (!(matchtoken(Scanner::IDENT)))
                return (0);
            T.lookup(oldtokstr, attr);
            fltemp = T.getdata(attr.loc);
            std::cout << fltemp << " ";
        }
    }

    std::cout << std::endl; // end of line - ends for all of the printouts

    return (1);
}

int Parser::ntforindex()
// <forindex> -> IDENT EQUAL <assignment>
{
    Symbol::Attrib attr;

    T.lookup(tokstring, attr);
    if (!(matchtoken(Scanner::IDENT)))
        return (0);
    if (!(matchtoken(Scanner::EQUAL)))
        return (0);
    if (!(matchnterm(ASSIGN)))
        return (0);
    return (1);
}

int Parser::ntforblock() // error tracking here ******
// <lineblock> -> <line> (<lineblock)
// deviation from recursive method
{
    char dummy[255]; // used to make the user push enter;

    do
    {
        {
            myout << "Line " << newinst << std::endl;
            if (!(matchnterm(LINE)))
            {
                go_flag = FALSE;
                errorln = oldinst;
                return (0);
            }

            else if (!(forexit))
            {
                oldinst = newinst;
                newinst++;
            }
        }
    } while (go_flag && (lbexit == FALSE) && (!(forexit)));

    if (forexit)
        forexit = 0;

    return (1);
}
