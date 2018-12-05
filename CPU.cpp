// CPU. CPP

#include "CPU.h"

//************************************
/// Constructs CPU object and initialize it.
///
/// \param [in] CPU* cpu - pointer to CPU object
///
/// \return void
//************************************


void cpu_Ctor(CPU* cpu)
{
    assert(cpu);

    free(cpu->Ram);
    cpu->Ram = (cpu_type*) calloc(RAM_SIZE, sizeof(*(cpu->Ram)));

    for(int reg = 1; reg <= 10; reg++)
        cpu->r[reg] = 0;

    cpu->data = (MyStack*) calloc(1, sizeof(*(cpu->data)));
    stack_Ctor(cpu->data);

    cpu->func_calls = (MyStack*) calloc(1, sizeof(*(cpu->func_calls)));
    stack_Ctor(cpu->func_calls);
}

//************************************
/// Deletes CPU object and its data.
///
/// \param [in] CPU* cpu - pointer to CPU object
///
///
/// \return void
///
//************************************


void cpu_Dtor(CPU* cpu)
{
    assert(cpu);

    memset(cpu->Ram, NAN, RAM_SIZE);
    free(cpu->Ram);

    stack_Dtor(cpu->data);
    free(cpu->data);

    stack_Dtor(cpu->func_calls);
    free(cpu->func_calls);

    cpu = NULL;
}

//***********************************
/// Get cpu code from array and perform the program
///
/// \param [in] const char READNAME[] - name of the file to read from
/// \param [in] const char DUMPNAME[] - name of the file for logs
/// \param [in/out]  CPU* cpu - pointer to CPU object
///
/// \return 1 if there were any errors, 0 if not
///
//************************************

#define COM_DEF(name, num, oper, body) { case num:\
                                            body\
                                            break;\
                                       }
#define CHECK_RAM_SIZE(typed, arg) (type == typed && arg >= RAM_SIZE)

int cpu_get_commands(const char READNAME[], const char DUMPNAME[], CPU* cpu)
{
    char* comFile = read_from_file(READNAME, READBYTES);

    int len = txt_file_length(READNAME);
    assert(!(len % com_size) && cpu);

    size_t big = len /  com_size;

    char* argum[sizeof(cpu_type)];
    for (char* ip = comFile; ip < (comFile + len); ip += com_size)
    {
        int com_num = (ip - comFile) / com_size;

        int type = *(ip + 1);
        int registr = *(ip + 2);
        memmove(&argum, ip + 3, sizeof(cpu_type));
        size_t index = (unsigned int) (*((cpu_type*) &argum));

        if (CHECK_RAM_SIZE(ARR_ARG, index) || CHECK_RAM_SIZE(ARR_REG, cpu->r[registr]))
        {
            printf("~Invalid index of RAM in %i line\n", com_num);
            return 1;
        }

        unsigned int com = (unsigned int) *ip;

        switch(com)
        {
            #include "Commands.h"
        }

        if (type == ARR_ARG || type == ARR_REG)
            Sleep(2);

        dump(DUMPNAME, cpu->data, com_num);
    }
    free(comFile);
    return 0;
}

#undef COM_DEF
#undef CHECK_RAM_SIZE

//***********************************
/// Writes logs to file
///
/// \param [in] const char DUMPNAME[] - name of the file for logs
/// \param [in] MyStack* stck - pointer to MyStack object for logging
/// \param [in] int command - number of current command
///
/// \return 1 if there were any errors,0 if not
///
//************************************

int dump(const char DUMPNAME[], MyStack* stck, int command)
{
    assert(stck);
    FILE* dumptxt = (command == 0) ? fopen(DUMPNAME, "w") : fopen(DUMPNAME, "a");
    if (!dumptxt) return 1;

    fprintf(dumptxt, "In File:  %s\n     In command:  %i\n", __FILE__, command);
    fprintf(dumptxt, "CPU->data:\n      Capacity = %u\n      Size = %u\n      data [0x%X]:\n", stck->StackCapacity, stck->StackSize, stck->data);
    int bufn = stck->StackSize;
    for (int i = 0; i < bufn; i++)
    {
        fprintf(dumptxt, "         data[%d] = %f\n", i, stck->data[i]);
    }
    fprintf(dumptxt,"\n\n");

    fclose(dumptxt);
    return 0;
}
