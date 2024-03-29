# c_basic: A BASIC Interpreter in C

An implementation of an interpreter for a subset of Darthmouth-BASIC language

* BASIC = Beginners All-Purpose Symbolic Instruction Code
* Written in C with some C++ I/O and classes

## Installation

### Taskfile

On MacOS M1 for _Optimized_ build:

* Have __xcode__ and __clang__ updated ahead of time

`task build`

### Manual compile and link

On MacOS M1 for _non-optimized_ build:

```bash
cd src
g++ -g -c *.c
g++ -g -o c_basic c_basic.o parser.o scanner.o symtable.o memory.o
```

Set the binary to be executable:

```bash
chmod u+x ./c_basic
```

## Usage

Run program

```bash
./c_basic <program.bas>
```

Example: `./c_basic BASIC/test_1.bas`

Expected output:

```bash

c_basic:
HELLO WORLD

```

## Language support

### Grammar

Backus-Naur form

| Symbol        | Expression |
| ------------- | ---------- |
| `<program>`   | `<lineblock>` |
| `<lineblock>` | `<line> (<lineblock>)` |
| `<line>`      | `LNUM <stmtlist>` |
| `<stmtlist>`  | `<stmt> (COLON <stmt>)` |
| `<stmt>`      | `IDENT EQUAL <assign>` |
| `<stmt>`      | `PRINT <printlist>` |
| `<stmt>`      | `FOR <forindex> TO INTLIT <lineblock> NEXT IDENT` |
| `<stmt>`      | `INPUT IDENT` |
| `<stmt>`      | `DIM IDENT` |
| `<stmt>`      | `IF IDENT EQUAL INTLIT THEN <stmt>` |
| `<stmt>`      | `END` |
| `<assign>`    | `O_PAREN <assign> C_PAREN ([PLUS`&#124;`MINUS`&#124;`MULT`&#124;`DIV]`  `<assign>)` |
| `<assign>`    | `[INTLIT`&#124;`IDENT] ([PLUS`&#124;`MINUS`&#124;`MULT`&#124;`DIV] <assign>)` |
| `<printlist>` | `QUOTE STRING QUOTE`&#124;`IDENT (,STRING`&#124;`IDENT,...)` |
| `<forindex>`  | `IDENT EQUAL <assignment>` |


## Development

### To Do Items

1. ✅ Compile with optimizations, LTO, LLVM linking
2. Let `PRINT` statements take multiple values separated by `,` or `;`
3. Implement `RND` and other numerics
4. Make simple Bubble Sort
5. Making timing / testing setup vs multiple files and expected outputs
6. Test vs other interpreters
7. Add more tests and Sort algorithms
8. Explore and expand functional limits, ranges, max lengths of arrays etc.
9. Enhance memory management with `malloc`, `calloc` and similar functions
10. Run some of the sample programs other teammates are trying
