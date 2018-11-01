//text.h
#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <clocale>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#define READ "r"
#define READBYTES "rb"
#define ADD "a"
#define ADDBYTES "ab"
#define DELANDWRITE "w"
#define DELANDWRITEBYTES "wb"

#ifndef $
#define $(x) std::cout<<"~"<<#x " = "<<x<<"\n";
#endif

typedef struct MyText MyText;
const unsigned int StartLength = 10;

struct MyText
{
    char** text_table = NULL;

    int N_table = 0;
};

void text_Ctor(MyText*, int length);
void text_Dtor(MyText*);
int txt_file_length (FILE* file);
char* read_from_file (const char * file_name, const char* mode);
MyText* read_in_Text (const char * file_name, const char* mode);
int write_Text_to_file (const char* file_name, MyText* text, const char* mode);
int write_to_file(const char* file_name, char* txt, unsigned int len, const char* mode);
char** get_text_in_table (char* text);
void free_table (char** table, int n_table);
int string_counter (char* text);

#endif // TEXT_H_INCLUDED
