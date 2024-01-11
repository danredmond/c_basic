#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <string>

class Scanner
{
    std::ifstream istr;
    int QUOTE_FLAG = 0;
    int NEW_FLAG = 0;

public:
    static const int MAX_TOKEN_SIZE = 256;
    static const int MAX_STRING_SIZE = 64;

    enum Tokentype
    {
        COMMENT,
        PAUSE,
        LNUM,
        IDENT,
        INTLIT,
        STRING,
        EQUAL,
        PLUS,
        MINUS,
        MULT,
        DIV,
        QUOTE,
        O_PAREN,
        C_PAREN,
        G_THAN,
        L_THAN,
        SEMI,
        COLON,
        COMMA,
        PRINT,
        INPUT,
        END,
        DIM,
        GOTO,
        GOSUB,
        RETURN,
        FOR,
        TO,
        NEXT,
        IF,
        THEN,
        UNKNOWN,
        BADFILE,
        EOFILE,
        EOSTR
    };

    char sline[255]; // scan line

    Scanner() = default;
    explicit Scanner(char *);

    int init(char *);
    static const char *token2string(Tokentype);
    // converts token to an appropriate string;
    char *scan(Tokentype &);
    // finds next token on istr which should
    // have been opened by constructor.
    // 3rd arg should be pointer to an array of MAX_TOKEN_SIZE chars.
    // the character string which makes up the token goes in 3rd arg;
    // always find the longest posible token.
    // skip over white spaces.
    // continue to return EOFILE, "" after file is exhausted.
};