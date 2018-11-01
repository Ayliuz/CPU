#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

const char READNAME[] = "program.txt";
const char COMNAME[]  = "done.asm";
typedef double cpu_type;

#define COM_DEF(name,num) COM_##name = (num),
///Соответствие имён команд их номерам
enum AllCommand
{
    #include "Commands.h"
    COM_MAX
};
#undef COM_DEF

///Регистровые переменные
enum Regist
{
    r1 = 1, r2, r3, r4, r5, r6, r7, r8, r9, r10
};

#endif // DATA_H_INCLUDED
