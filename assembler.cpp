#include "D:\TX\TXlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "stack.h"
#include "text.h"
#include "Data.h"

#define ERRORTYPE -1
#define NON-ARG 0
#define ARGUM 1
#define REGIST 2
#define ARR_ARG 3
#define ARR_REG 4

#define COM_DEF(name,num) if(command_cmp(#name, Program->text_table[str_i])) \
                          {\
                                com = COM_##name;\
                                char* asm_point = (assembl + str_i * com_size);\
                                *(asm_point) = (char) com;\
                                *(asm_point + 1) = type;\
                                *(asm_point + 2) = registr;\
                                *((cpu_type*)(asm_point + 3)) = argum;\
                                \
                          }\

const int com_size = sizeof(char) + sizeof(char) + sizeof(char) + sizeof(cpu_type);

int funct_type (char* str, char* registr, cpu_type* arg);
char* skip_letspaces_and_comment(char*);
int command_cmp (const char* command, char* str);

int assembler()
{
    MyText* Program = read_in_Text(READNAME, READ);

    char* assembl = (char*) calloc (Program->N_table * com_size + 1, sizeof (*assembl));

    enum AllCommand com;
    char* regbuf = (char*) calloc(4, sizeof(*regbuf));
    char registr = 0;
    char type = 0;
    cpu_type argum = 0;

    for (int str_i = 0; str_i < (Program->N_table); ++str_i)
    {
        type = funct_type(Program->text_table[str_i], regbuf, &argum);
        registr = atoi(regbuf);

        #include "Commands.h"

        memset(regbuf, '\0', 4);
        argum = 0;
    }

    assembl[Program->N_table * com_size] = '\0';

    if (write_to_file(COMNAME, assembl, Program->N_table * com_size, DELANDWRITEBYTES))
    {
        printf("~Error in writing to .com file\n");
        return 0;
    }

    free(regbuf);
    free(assembl);
    text_Dtor(Program);
    return 1;
}

#undef COM_DEF

//***********************************
/// Recognize type of command on the string and its arguments
///
/// \param [in] char* str - the string to read from
/// \param [out] char registr - number of register variable used in command
/// \param [out] cpu_type arg - argument of command
///
/// \return the type of command from the string
///
//************************************

#define FIND(ch) (strchr(str,(ch)) != NULL)
#define DIG(num) if (isdigit(*str))\
            {\
                type = num;\
                sscanf(str, "%lf", arg);\
            }
#define ALPH(num) if (isalpha(*str))\
            {\
                type = num;\
                sscanf(str + 1, "%[0-9]s", registr);\
            }

int funct_type (char* str, char* registr, cpu_type* arg)
{
    assert(str && registr && arg);

    int type = 0;

    str = skip_letspaces_and_comment(str);
    if (*str == '\0') return 0;

    if ((*str == '[') && FIND(']'))
    {
        while (isspace(*(++str)) && (*str != ']'));
        ALPH(ARR_REG)
        else DIG(ARR_ARG)
        else type = ERRORTYPE;
    }
    else
    {
        DIG(ARGUM)
        else ALPH(REGIST)
        else type = ERRORTYPE;
    }
    return type;
}

#undef FIND
#undef DIG
#undef ALPH

//***********************************
/// Skip letters and spaces in the beginning of the string and comments
///
/// \param [in] char* s - the string to read from
///
///
/// \return pointer to first non-letter character or '0' if there is none of them excluding ';' and the whole comments
///
//************************************

char* skip_letspaces_and_comment(char* s)
{
    assert(s);

    for (; *(s) != '0' ; ++s)
    {
        if(!isalpha(*s)) break;
    }

    while (isspace(*s) && (*s != '\0')) s++;
    if (*s == ';') s = '\0';
    return s;
}

//***********************************
/// Compares the beginning of a string with command
///
/// \param [in] char* command - the command to compare with
/// \param [in] char* str - the string to compare
///
///
/// \return pointer to first non-letter character or '0' if there is none of them excluding ';' and the whole comments
///
//************************************

int command_cmp (const char* command, char* str)
{
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] != str[i]) return 0;
    }
    return 1;
}
