#include "D:\TX\TXlib.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "text.h"
#include "Data.h"
#include "assembler.h"
#include "CPU.h"

#define END cpu_Dtor(&cpu); return 1;
const char READNAME[] = "factor.txt";

int main()
{
    please txSetConsoleAttr (0xf0);

    CPU cpu;
    cpu_Ctor(&cpu);

    if (assembler(READNAME, COMNAME)) {                 printf("~~~~~~~~~~~Assemble error~~~~~~~~~~~~~~~~~\n"); END; }
    if (disassembler(COMNAME, WRITENAME, LOG)) {        printf("~~~~~~~~~~~Disassemble error~~~~~~~~~~~~~~~~~\n"); END; }
    if (cpu_get_commands(COMNAME, STACK_DUMP, &cpu)) {  printf("~~~~~~~~~~~CPU error~~~~~~~~~~~~~~~~~\n"); END; }

    cpu_Dtor(&cpu);
    return 0;
}

#undef END
