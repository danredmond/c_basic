#include "scanner.h"

int shrink(char *dest, char *src, int start)
/* copies source[start] to dest[0] */
/* adjusts for \0  */
/* returns number of chars moved */
{
    int d = 0;

    do
        dest[d++] = src[start++];
    while (src[start] != '\0');
    dest[d++] = '\0';
    return (d - 1);
}

int equal(char *str1, char *str2)
{
    int x = 0;
    int eq = 1;
    do
    {
        if (str1[x] != str2[x])
            eq = 0;
        x++;
    } while ((x < 255) && (str1[x] != '\0') && (str2[x] != '\0') && (eq == 1));
    if (x >= 255)
        eq = 0;
    if (str1[x] != str2[x])
        eq = 0;
    return (eq);
}

Scanner::Scanner(char *p)
{

    Scanner::NEW_FLAG = 1;
    Scanner::QUOTE_FLAG = 0;
    // assigns istr to arg and opens for reading.
    istr.open(p);
    if (!istr)
    {
        std::cout << "Can't open source file.";
        exit(1);
    }
}

int Scanner::init(char *p)
{

    Scanner::NEW_FLAG = 1;
    Scanner::QUOTE_FLAG = 0;

    strcpy(sline, p); // to stop the warnings (temporary fix)

    // assigns istr to arg and opens for reading.
    //  istr.open(p);
    //  if (!istr) {std::cout << "Can't open source file."; return(0); }
    return (1);
}

const char *Scanner::token2string(Tokentype t)
{
    // converts token to an appropriate string;
    switch (t)
    {
    case PAUSE:
        return "PAUSE";
    case LNUM:
        return "LINE # ";
    case IDENT:
        return "IDENT: ";
    case INTLIT:
        return "INTLIT: ";
    case STRING:
        return "STRING: ";
    case EQUAL:
        return "EQUAL: ";
    case PLUS:
        return "PLUS: ";
    case MINUS:
        return "MINUS: ";
    case MULT:
        return "MULT: ";
    case DIV:
        return "DIV: ";
    case QUOTE:
        return "QUOTE: ";
    case O_PAREN:
        return "OPEN PAREN: ";
    case C_PAREN:
        return "CLOSE PAREN: ";
    case G_THAN:
        return "GREATER: ";
    case L_THAN:
        return "LESS: ";
    case SEMI:
        return "SEMI: ";
    case COLON:
        return "COLON: ";
    case COMMA:
        return "COMMA ";
    case PRINT:
        return "PRINT stmt: ";
    case INPUT:
        return "INPUT stmt: ";
    case END:
        return "END stmt: ";
    case DIM:
        return "DIM stmt: ";
    case GOTO:
        return "GOTO stmt: ";
    case GOSUB:
        return "GOSUB stmt: ";
    case RETURN:
        return "RETURN stmt: ";
    case FOR:
        return "FOR stmt: ";
    case TO:
        return "TO stmt: ";
    case NEXT:
        return "NEXT stmt: ";
    case IF:
        return "IF stmt: ";
    case THEN:
        return "THEN stmt: ";
    case UNKNOWN:
        return "unknown: ";
    case EOFILE:
        return "end of file: ";
    case EOSTR:
        return "end of line/string";
    default:
        return "?: ";
    }
}

char *Scanner::scan(Tokentype &t)
{ // var decs
    char newstr[255];
    char *n;
    char tchar; // temp char
    int LN_FLAG = 1;
    int i = 0; // line index;
    int templen;

    t = UNKNOWN;

    // eat up extra white spaces

    while (sline[i] == ' ')
        shrink(sline, sline, 1);
    // i++; // skip white spaces

    if (sline[i] == '\0') // new line means LNUM is coming
    {
        strcpy(newstr, "");
        t = EOSTR;
        LN_FLAG = 1;
        goto Finish;
    }
    else
        LN_FLAG = 0;

    if (sline[i] == EOF) // eof condition
    {
        *n = '\0';
        t = EOFILE;
        goto Finish;
    }

    // if (!istr.good())        // other bad file condition
    // {  *n = '\0';
    //    t = BADFILE;
    //      goto Finish;
    // }

    // check for 1 length operators

    strcpy(newstr, "");   // set up string and index
    n = newstr;           // index
    tchar = sline[i + 1]; // not required

    if (sline[i] == '+')
    {
        t = PLUS;
        tchar = sline[i];
        i++;
        *n = tchar;
        *(n + 1) = '\0';
        goto Finish;
    }

    if (sline[i] == '-')
    {
        t = MINUS;
        tchar = sline[i];
        i++;
        *n = tchar;
        *(n + 1) = '\0';
        goto Finish;
    }

    if (sline[i] == '*')
    {
        t = MULT;
        tchar = sline[i];
        i++;
        *n = tchar;
        *(n + 1) = '\0';
        goto Finish;
    }

    if (sline[i] == '/')
    {
        t = DIV;
        tchar = sline[i];
        i++;
        *n = tchar;
        *(n + 1) = '\0';
        goto Finish;
    }

    if (sline[i] == '(')
    {
        t = O_PAREN;
        tchar = sline[i];
        i++;
        *n = tchar;
        *(n + 1) = '\0';
        goto Finish;
    }

    if (sline[i] == ')')
    {
        t = C_PAREN;
        tchar = sline[i];
        i++;
        *n = tchar;
        *(n + 1) = '\0';
        goto Finish;
    }

    if (sline[i] == '<')
    {
        t = L_THAN;
        tchar = sline[i];
        i++;
        *n = tchar;
        *(n + 1) = '\0';
        goto Finish;
    }

    if (sline[i] == '>')
    {
        t = G_THAN;
        tchar = sline[i];
        i++;
        *n = tchar;
        *(n + 1) = '\0';
        goto Finish;
    }

    if (sline[i] == ';')
    {
        t = SEMI;
        tchar = sline[i];
        i++;
        *n = tchar;
        *(n + 1) = '\0';
        goto Finish;
    }

    if (sline[i] == ':')
    {
        t = COLON;
        tchar = sline[i];
        i++;
        *n = tchar;
        *(n + 1) = '\0';
        goto Finish;
    }

    if (sline[i] == ',')
    {
        t = COMMA;
        tchar = sline[i];
        i++;
        *n = tchar;
        *(n + 1) = '\0';
        goto Finish;
    }

    if (sline[i] == '"')
    {
        t = QUOTE;
        tchar = sline[i];
        i++;
        *n = tchar;
        *(n + 1) = '\0';

        if (Scanner::QUOTE_FLAG)
            Scanner::QUOTE_FLAG = 0;
        else
            Scanner::QUOTE_FLAG = 1;

        goto Finish;
    }

    if (Scanner::QUOTE_FLAG)
    {
        int c = 0;
        if (sline[i] != '\"')
        {
            t = STRING;

            while (sline[i] != '\"')
            {
                tchar = sline[i];
                i++;
                n[c++] = tchar;
                if (c >= MAX_STRING_SIZE)
                {
                    std::cout << "string too long\n";
                    exit(2);
                }
            }
            n[c] = '\0';
        }
        goto Finish;
    }

    tchar = sline[i];
    i++;
    *n = tchar;

    if (isalpha(*n))
    {
        n++; // identifier or reserved word.
        while ((isalpha(sline[i])) ||
               (sline[i] == '[') ||
               (sline[i] == ']') ||
               (isdigit(sline[i])))
        {
            tchar = sline[i];
            i++;
            *(n++) = tchar;
            if ((n - newstr) > MAX_TOKEN_SIZE)
            {
                std::cout << "token too long\n";
                exit(1);
            } // buffer overflow
        }
        *n = '\0';

        if (strcasecmp(newstr, "print") == 0)
            t = PRINT;
        else if (strcasecmp(newstr, "input") == 0)
            t = INPUT;
        else if (strcasecmp(newstr, "end") == 0)
            t = END;
        else if (strcasecmp(newstr, "dim") == 0)
            t = DIM;
        else if (strcasecmp(newstr, "pause") == 0)
            t = PAUSE;
        else if (strcasecmp(newstr, "goto") == 0)
            t = GOTO;
        else
            //    if (strcasecmp(newstr,"gosub") == 0)
            //      t = GOSUB;
            //    else
            //    if (strcasecmp(newstr,"return") == 0)
            //      t = RETURN;
            //    else
            if (strcasecmp(newstr, "if") == 0)
                t = IF;
            else if (strcasecmp(newstr, "then") == 0)
                t = THEN;
            else if (strcasecmp(newstr, "for") == 0)
                t = FOR;
            else if (strcasecmp(newstr, "to") == 0)
                t = TO;
            else if (strcasecmp(newstr, "next") == 0)
                t = NEXT;
            else

                t = IDENT; // default
        goto Finish;
    }

    else if ((isdigit(*n)) || (*n == '.'))
    {
        n++; // integer literal
        while ((isdigit(sline[i])) || (sline[i] == '.'))
        {
            tchar = sline[i];
            i++;
            *(n++) = tchar;
            if (n - newstr > MAX_TOKEN_SIZE)
            {
                std::cout << "token too long\n";
                exit(1);
            } // buffer overflow
        }

        *n = '\0';

        if (LN_FLAG || NEW_FLAG)
        {
            if (NEW_FLAG)
                NEW_FLAG = 0;
            t = LNUM;
        }
        else
            t = INTLIT;
        goto Finish;
    }
    else if (*n == '=')
    {
        n++;
        t = EQUAL;
        *n = '\0';
        goto Finish;
    } // equal sign

    /* t = UNKNOWN *q = '\0';*/ // moved to top
    if (t == UNKNOWN)
        *n = '\0';

    goto Finish;

Finish:
    templen = strlen(newstr);
    if (sline[templen] == ' ')
        templen++;
    shrink(sline, sline, templen);
    return (newstr);
}
