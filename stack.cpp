// STACK. CPP

#include "stack.h"

//************************************
/// Compares double parameter with 0.
///
/// \param [in] double x
///
/// \return 1 if variable equals to 0 (less than 0.000001),
///         0 if variable is bigger than 0.000001
//************************************

int iszero( double x )
{
    return ( fabs( x )<0.000001 ) ? 1 : 0;
}

//************************************
/// Constructs MyStack structure and initialize it.
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
///
/// \return void
///
//************************************

void stack_Ctor(MyStack* s)
{
    assert(s);

    s->stack_guard_begin = GUARD;
    s->stack_guard_end = GUARD;

    s->data = (stack_type*) calloc(StartCapacity + 2, sizeof(stack_type));
    s->StackSize = 0;
    s->StackCapacity = StartCapacity;

    set_guards(s);

    for (unsigned int i = 0; i < (s->StackCapacity); i++)
    {
        s->data[i] = NAN;
    }

    make_hash(s);
}

//************************************
/// Sets guards of the stack's array
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
///
/// \return void
///
//************************************

void set_guards(MyStack* s)
{
    assert(s);

    s->data_guard_begin = s->data;
    *(s->data_guard_begin) = GUARD;

    s->data = s->data + 1;
    s->data_guard_end = s->data + s->StackCapacity;
    *(s->data_guard_end) = GUARD;
}

//************************************
/// Deletes MyStack structure and its data.
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
///
/// \return void
///
//************************************

void stack_Dtor(MyStack* s)
{
    assert(s);

    for (;s->StackSize >= 1; s->StackSize --)
    {
        s->data[s->StackSize - 1] = NAN;
    }

    free(s->data);
    s->data = NULL;

    s->StackSize = SIZEPOIS;
    s->StackCapacity = SIZEPOIS;

    s->StackHash_data = HASHPOIS;
    s->StackHash_stack = HASHPOIS;

    *(s->data_guard_begin) = NAN;
    *(s->data_guard_end) = NAN;
    s->data_guard_begin = NULL;
    s->data_guard_end = NULL;

    s->stack_guard_begin = GUARDPOIS;
    s->stack_guard_end = GUARDPOIS;

    s = NULL;
}

//************************************
/// Clears MyStack structure's data.
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
///
/// \return void
///
//************************************


void stack_Clear(MyStack* s)
{
    assert(s);

    for (;s->StackSize >= 1; s->StackSize --)
    {
        s->data[s->StackSize - 1] = NAN;
    }

    s->data = (stack_type*) realloc(s->data, (StartCapacity + 2) * sizeof(stack_type));
    s->StackCapacity = StartCapacity;

    set_guards(s);

    make_hash(s);
}

//************************************
/// Inserts an element on the top of stack
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
///
/// \return gives info about stack in console if there are any troubles
///
//************************************

void stack_push(MyStack* s, stack_type val)
{
    assert(s);
    if ( !stack_is_OK(s)) {DUMP(StackErNum,s); abort();}

    if (s->StackSize + 1 < s->StackCapacity)
    {
        s->data[s->StackSize] = val;
        s->StackSize ++;
    }
    else
    {
        stack_extend(s);
        s->data[s->StackSize] = val;
        s->StackSize ++;
    }

    make_hash(s);

    if ( !stack_is_OK(s)) {DUMP(StackErNum,s); abort();}
}

//************************************
/// Takes one element from the top of stack
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
///
/// \return stack_type element from the top of the stack, gives info about stack in console if there are any troubles
///
//************************************

stack_type stack_pop(MyStack* s)
{
    assert(s);
    if(s->StackSize == 0)
    {
        StackErNum = ERRORPOP;
        DUMP(StackErNum, s)
        return NAN;
    }

    if ( !stack_is_OK(s)) {DUMP(StackErNum,s); abort();}

    stack_type buf = s->data[--(s->StackSize)];
    s->data[(s->StackSize)] = NAN;

    if (((s->StackCapacity) >= 4 * (s->StackSize)) && (s->StackCapacity > StartCapacity))
    {
        stack_contract(s);
    }

    make_hash(s);

    if ( !stack_is_OK(s)) {DUMP(StackErNum,s); abort();}
    return buf;

}

//************************************
/// Extends the array in the stack.
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
/// \return changes the size of arrow in stack.
///
//************************************

void stack_extend (MyStack* s)
{
    assert(s);

    s->data = (stack_type*) realloc(s->data_guard_begin, ((s->StackCapacity) * 2 + 2) * sizeof(stack_type));
    s->StackCapacity *= 2;

    set_guards(s);

    for (unsigned int i = (s->StackSize); i < (s->StackCapacity); i++)
    {
        s->data[i] = NAN;
    }
}

//************************************
/// Contracts the array in the stack.
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
/// \return changes the size of arrow in stack.
///
//************************************

void stack_contract (MyStack* s)
{
    assert(s);

    s->data = (stack_type*) realloc(s->data_guard_begin, (s->StackCapacity / 2 + 2) * sizeof(stack_type) );
    s->StackCapacity /= 2;

    set_guards(s);
}

//************************************
/// Hashes the  stack.
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
/// \return changes StackHash_stack and StackHash_data in stack.
///
//************************************

void make_hash (MyStack* s)
{
    assert(s);

    s->StackHash_data = hash (s->data, s->StackCapacity * sizeof (stack_type));
    s->StackHash_stack = HASHDEFAULT;
    s->StackHash_stack = hash (s, sizeof (*s));
}

//************************************
/// Hashes memory pointed by given pointer.
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
/// \return changes StackHash_data in stack.
///
//************************************

int hash (void* s, unsigned int size)
{
    assert(s);

    int hashSum = 0;
    for (char* p = (char*) s; p < (char*)(s + size); p++)
    {
        hashSum = 22222 * hashSum  + abs(int(*p));
        hashSum = hashSum % 20002;
    }

    return hashSum;
}


//************************************
/// Checks if stack is fine or not, and writes error code in global variable
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
/// \return 1 if stack is fine 0 if not, changes the global variable StackErNum (0 - all is right, ERRORDATA - data in stack is damaged, ERRORCAPACITY - not suitable stack capacity, \
ERRORSIZE - not suitable stack size, ERRORMEMORY - memory of stack data was damaged, ERRORSTRUCT - memory of stack struct was damaged)
///
//************************************

int stack_is_OK(MyStack* s)
{
    assert(s);
    int not_error = 1;

    if ( !(s->data)){ StackErNum = ERRORDATA; return 0;}

    for (unsigned int i = s->StackSize + 1; i < s->StackCapacity; i++ )
    {
        if ( isfinite(s->data[i]))
        {
            StackErNum = ERRORMEMORY;
            not_error = 0;
            break;
        }
    }

    if ( !iszero(hash(s->data, sizeof(stack_type) * (s->StackCapacity)) - (s->StackHash_data))){                                                                        StackErNum = ERRORDATA;      not_error = 0; }

    if ( !iszero(*(s->data_guard_begin) - GUARD) || !iszero(*(s->data_guard_end) - GUARD)){                                                                             StackErNum = ERRORMEMORY;    not_error = 0;}

    int StackHashBuf = s->StackHash_stack;
    s->StackHash_stack = HASHDEFAULT;
    if ( !iszero(hash(s, sizeof(*s)) - StackHashBuf) || ((s->stack_guard_begin) != GUARD) || ((s->stack_guard_end != GUARD))){                                          StackErNum = ERRORSTRUCT;    not_error = 0;}
    s->StackHash_stack = StackHashBuf;

    if ( !isfinite(s->StackCapacity) || ((s->StackCapacity) < StartCapacity) || (((s->StackCapacity) % StartCapacity) != 0) || ((s->StackSize) > (s->StackCapacity))){  StackErNum = ERRORCAPACITY;  not_error = 0;}

    if ( !isfinite(s->StackSize) || (((s->StackSize) > (s->StackCapacity)) && !isfinite(s->data[s->StackSize-1])) || isfinite(s->data[s->StackSize])) {                 StackErNum = ERRORSIZE;      not_error = 0;}

    return not_error;
}

//************************************
/// Tests stack_is_OK in case of StackSize
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
/// \return 1 if test was passed, 0 if not and calls for DUMP
///
//************************************

int test_error_size(const MyStack* stak)
{
    assert(stak);
    MyStack s = *stak;
    stack_Ctor(&s);

    for(double i = 2.01; i < 44.01; i = i + 1.0)
    {
        stack_push((&s), 5.023 + i);
    }
    //
    s.StackSize = 20;
        UNERTEST(ERRORSIZE, &s)
    StackErNum = 0;
    //
    s.StackSize = 150;
        UNERTEST(ERRORSIZE, &s)
    StackErNum = 0;
    //
    s.StackSize = NAN;
        UNERTEST(ERRORSIZE, &s)
    StackErNum = 0;

    stack_Dtor(&s);

    return 1;
}

//************************************
/// Tests stack_is_OK in case of StackCapacity
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
/// \return 1 if test was passed, 0 if not and calls for DUMP
///
//************************************

int test_error_capacity(const MyStack* stak)
{
    assert(stak);
    MyStack s = *stak;
    stack_Ctor(&s);

    for(double i = 2.01; i < 44.01; i = i + 1.0)
    {
        stack_push((&s), 5.023 + i);
    }

    s.StackCapacity = 10;
        UNERTEST(ERRORCAPACITY, &s)
    StackErNum = 0;
    //
    s.StackCapacity = 77;
        UNERTEST(ERRORCAPACITY, &s)
    StackErNum = 0;
    //
    s.StackCapacity = NAN;
        UNERTEST(ERRORCAPACITY, &s)
    StackErNum = 0;

    stack_Dtor(&s);

    return 1;
}

//************************************
/// Tests stack_is_OK in case of data
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
/// \return 1 if test was passed, 0 if not and calls for DUMP
///
//************************************

int test_error_data(const MyStack* stak)
{
    assert(stak);
    MyStack s = *stak;
    stack_Ctor(&s);


    for(double i = 2.01; i < 44.01; i = i + 1.0)
    {
        stack_push((&s), 5.023 + i);
    }

    stack_type buf = s.data[41];
    s.data[41] = 90;
        UNERTEST(ERRORDATA, &s)
    StackErNum = 0;
    s.data[41] = buf;

    buf = s.data[1];
    s.data[1] = 92020;
        UNERTEST(ERRORDATA, &s)
    StackErNum = 0;
    s.data[1] = buf;

    buf = s.data[20];
    s.data[20] = 12340;
        UNERTEST(ERRORDATA, &s)
    StackErNum = 0;
    s.data[20] = buf;

    void* buf_point = s.data;
    s.data = NULL;
        UNERTEST(ERRORDATA, &s)
    StackErNum = 0;
    s.data = (stack_type*) buf_point;

    stack_Dtor(&s);

    return 1;
}

//************************************
/// Tests stack_is_OK in case of data memory
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
/// \return 1 if test was passed, 0 if not and calls for DUMP
///
//************************************

int test_error_memory(const MyStack* stak)
{
    assert(stak);
    MyStack s = *stak;
    stack_Ctor(&s);


    for(double i = 2.01; i < 44.01; i = i + 1.0)
    {
        stack_push((&s), 5.023 + i);
    }

    *(s.data_guard_begin) = 500;
        UNERTEST(ERRORMEMORY, &s)
    StackErNum = 0;
    *(s.data_guard_begin) = GUARD;

    *(s.data_guard_end) = -2200;
        UNERTEST(ERRORMEMORY, &s)
    StackErNum = 0;
    *(s.data_guard_end) = GUARD;

    stack_Dtor(&s);

    return 1;
}

//************************************
/// Tests stack_is_OK in case of struct memory
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
/// \return 1 if test was passed, 0 if not and calls for DUMP
///
//************************************

int test_error_stackbuf(const MyStack* stak)
{
    assert(stak);
    MyStack s = *stak;
    stack_Ctor(&s);

    for(double i = 2.01; i < 44.01; i = i + 1.0)
    {
        stack_push((&s), 5.023 + i);
    }

    void* buf_point = s.data;
    s.data += 3;
        UNERTEST(ERRORSTRUCT, &s)
    StackErNum = 0;
    s.data = (stack_type*) buf_point;

    buf_point = s.data_guard_begin;
    s.data_guard_begin += 5;
        UNERTEST(ERRORSTRUCT, &s)
    StackErNum = 0;
    s.data_guard_begin = (stack_type*) buf_point;

    buf_point = s.data_guard_end;
    s.data_guard_end -= 50;
        UNERTEST(ERRORSTRUCT, &s)
    StackErNum = 0;
    s.data_guard_end = (stack_type*) buf_point;

    s.stack_guard_begin = 432200;
        UNERTEST(ERRORSTRUCT, &s)
    StackErNum = 0;
    s.stack_guard_begin = GUARD;

    s.stack_guard_end = -4352328;
        UNERTEST(ERRORSTRUCT, &s)
    StackErNum = 0;
    s.stack_guard_end = GUARD;

    stack_Dtor(&s);

    return 1;
}

//************************************
/// Tests stack
///
/// \param [in] struct MyStack* s - pointer to MyStack structure
///
/// \return 1 if test was passed, 0 if not and calls for DUMP
///
//************************************

int test_stack(MyStack* stak)
{
    assert(stak);

    //
    if ( !test_error_data(stak))        return 0;
    //
    if ( !test_error_capacity(stak))    return 0;
    //
    if ( !test_error_size(stak))        return 0;
    //
    if ( !test_error_memory(stak))      return 0;
    //
    if ( !test_error_stackbuf(stak))    return 0;
    //
    //
    //Test error popping
    int counter = 0;
    for(double i = 2.01; i < 1225; i = i + 1.111, counter++)
    {
        printf("Testing stack: %.*s %.*s\r", counter * 55 / 2200, "||||||||||||||||||||||||||||||||||||||||||||||||||||||||", (2200 - counter) * 55 / 2200, "--------------------------------------------------------");
        stack_push((stak), 5.023 + i);
    }

    for(double i = 1224.11; i >= 2.01; i = i - 1.111, counter++)
    {
        printf("Testing stack: %.*s %.*s\r", counter * 55 / 2200, "||||||||||||||||||||||||||||||||||||||||||||||||||||||||", (2200 - counter) * 55 / 2200, "--------------------------------------------------------");
        UTEST(stack_pop(stak),(5.023 + i))
    }
    printf("\n");
    return 1;
}
