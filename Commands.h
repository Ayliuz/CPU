// COMMANDS. H

// COM_DEF(NAME OF FUNCTION, NUMBER OF FUNCTION, TYPE OF ARGUMENTS)
// TYPES OF ARGUMENTS: 0 - NO ARGUMENTS
//                     1 - ARGUMENT TO WORK WITH STACK(register variable, cpu_type value, array index)
//                     3 - ARGUMENTS AS IN 1 WITHOUT cpu_type value
//                     100 - END OF PROGRAM
//                     MARK - STRING ARGUMENT FROM ARRAY OF MARKS

#define REG(k) cpu->r[k]
#define PUSH(arg)   {\
                    cpu_type tempor = arg;\
                    if (tempor > sqrt(DBL_MAX))\
                    {\
                        printf("~Invalid value in %i line\n", com_num); \
                        return 1;\
                    }\
                    stack_push(cpu->data, tempor);\
                    }
#define POP stack_pop(cpu->data)
#define CASEPUSH(WhatType,argument)   case (WhatType): {PUSH(argument); break;}
#define CASEPOP(WhatType,argument)    case (WhatType): {argument = stack_pop(cpu->data); break;}
#define CASEIN(WhatType,argument)     case (WhatType): {printf("? "); scanf("%lf", argument); break;}
#define DO_MARK { ip = comFile + (*((long long*) argum) - 1) * com_size;}
#define DEFAULT  default:\
                    printf("Invalid syntax in %i line\n", (ip - comFile) / com_size);\
                    return 1;
#define INITPOP cpu_type a = POP; cpu_type b = POP; PUSH(b); PUSH(a);

COM_DEF(push, 1, 1,
        switch (type)
        {
            CASEPUSH(ARGUM, *((cpu_type*) &argum));
            CASEPUSH(REGIST, REG(registr));
            CASEPUSH(ARR_ARG, cpu->Ram[index]);
            CASEPUSH(ARR_REG, cpu->Ram[(int) REG(registr)]);
            DEFAULT
        })
COM_DEF(pop, 2, 3,
        switch (type)
        {
            case (NON_ARG): POP; break;
            case (ARGUM): POP; break;
            CASEPOP(REGIST, REG(registr));
            CASEPOP(ARR_ARG, cpu->Ram[index]);
            CASEPOP(ARR_REG, cpu->Ram[(int) REG(registr)]);
            DEFAULT
        })
COM_DEF(add, 3, 0, PUSH(POP + POP);)
COM_DEF(mul, 4, 0, PUSH(POP * POP);)
COM_DEF(jump, 5, MARK, DO_MARK)
COM_DEF(ifg, 6, MARK,  INITPOP; if(a < b) DO_MARK;)
COM_DEF(ifl, 7, MARK,  INITPOP; if(a > b) DO_MARK;)
COM_DEF(ifeq, 8, MARK, INITPOP; if(a == b)DO_MARK;)
COM_DEF(call, 9, MARK, stack_push(cpu->func_calls, com_num); DO_MARK;)
COM_DEF(ret, 10, 0,  if (cpu->func_calls->StackSize > 0) *((long long*)(argum)) = stack_pop(cpu->func_calls) + 1; DO_MARK;)
COM_DEF(out, 11, 0,  if (cpu->data->StackSize > 0) printf("%.7lg\n", POP);)
COM_DEF(in, 12, 3,
        switch (type)
        {
            case(NON_ARG):
            {
                cpu_type buf = 0;
                printf("? ");
                scanf("%lf", &buf);
                PUSH(buf);
                break;
            }
            CASEIN(REGIST,    &(REG(registr)));
            CASEIN(ARR_ARG,   &(cpu->Ram[index]));
            CASEIN(ARR_REG,   &(cpu->Ram[(int) REG(registr)]));
            DEFAULT
        })
COM_DEF(end, 100, 0, free(comFile); return 0;)
COM_DEF(subtr, 13, 0, INITPOP; POP; POP; PUSH(b - a);)
COM_DEF(div, 14, 0,
            INITPOP;
            POP;
            POP;
            if (!iszero(a))
            {
                PUSH(b/a);
            }
            else
            {
                printf("~Division by zero in %i line\n", com_num);
                return 1;
            })
COM_DEF(sqrt, 15, 0, PUSH(sqrt(POP));)
COM_DEF(meow, 16, 0, printf("               )\\._.,--....,'``.       \n .b--.        /;   _.. \\   _\\  (`._ ,. \n`=,-,-'~~~   `----(,_..'--(,_..'`-.;.'\n");)
#undef INITPOP
#undef DEFAULT
#undef DO_MARK
#undef CASEIN
#undef CASEPOP
#undef CASEPUSH
#undef POP
#undef PUSH
#undef REG
