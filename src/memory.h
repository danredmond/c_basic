#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>

const int MAXLINELEN = 80;
const int MAXLINES = 1000;
const int MAXDATA_INT = 400;
const int MAXSTACK = 64;

class Code
{
public:
    Code() : nextcode(1) {}
    char *getcode(int loc);
    int putcode(char *codestr);

private:
    int nextcode;
    char cspace[MAXLINES][MAXLINELEN];
};

class Data
{
public:
    Data() : nextdata(0) {}
    float getdata(int loc);
    int putdata(int loc, float dat);
    int assdata(int loc, float dat);

private:
    int nextdata;
    float dspace[MAXDATA_INT];
};

class Stack
{
public:
    Stack() : nextstack(0) {}
    int push(float data);
    void pop(float &dat);

private:
    int nextstack;
    float sspace[MAXSTACK];
};

class Memory : public Code, public Data, public Stack
{
public:
    Memory();
};