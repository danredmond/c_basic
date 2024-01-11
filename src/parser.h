#include <cstring>
#include <iostream>
#include "scanner.h"
#include "symtable.h"

const bool TRUE = true;
const bool FALSE = false;

static char tokstring[255];

class Parser
{

public:
    enum NTtype
    { // List of all Non-Terminals
        PROGRAM,
        LINEBLOCK,
        LINE,
        STMTLIST,
        STMT,
        ASSIGN,
        PRINTLIST,
        FORINDEX,
        FORBLOCK
    };

    Parser(char *);
    int parse();

private:
    Symbol T;
    Scanner S;
    Scanner::Tokentype nextoken;
    std::ofstream myout;

    int level;   // level of depth on parse tree
    int errorln; // line number of error (if any)
    int lines;   // number of lines in file

    int oldinst;  // global oldinst  -  gloabal withing scope
    int newinst;  // global newinst
    int go_flag;  // changes when end of code is reached
    int end_flag; // is set when end is encountered to stop parse error
    int lbexit;   // forces the lineblock to exit
                  // above allows (for next) loops
    int forexit;  // for loop exit flag;

    // internal functions;

    int matchtoken(Scanner::Tokentype);

    const char *nterm2str(NTtype);

    int matchnterm(NTtype);
    // output nterm name, inc level x and call nterm function

    int ntprogram();
    // <program> -> <lineblock>

    int ntlineblock(); // error tracking here ******
    // <lineblock> -> <line> (<lineblock)
    // deviation from recursive method
    int ntline();
    // <line> -> LNUM <stmtlist>

    int ntstmtlist(); // a bit of a deviation from pure
                      // recursive descent..ie i used
                      // iterations to track multiple
                      // statements on one line.

    // <stmtlist> -> <stmt> (COLON <stmt>),...      <-  iteration

    int ntstmt();
    // <stmt> -> IDENT EQUAL <assign>
    // <stmt> -> PRINT <printlist>
    // <stmt> -> FOR <forindex> TO INTLIT <lineblock> NEXT INTLIT

    int ntassign();
    // <assign> -> O_PAREN <assign> C_PAREN ([PLUS|MINUS|MULT|DIV]) <assign>)
    // <assign> -> [INTLIT|IDENT] ([PLUS|MINUS|MULT|DIV] <assign>)

    int ntprintlist();
    // <printlist> -> STRING|IDENT (,STRING|IDENT,...)  // iteration again

    int ntforindex();
    // <forindex> -> IDENT EQUAL <assignment>

    int ntforblock();
};
