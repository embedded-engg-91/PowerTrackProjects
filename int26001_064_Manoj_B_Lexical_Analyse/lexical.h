#ifndef LEXICAL_H
#define LEXICAL_H

#include<stdio.h>
#include<string.h>
#include<ctype.h>
typedef enum
{
    FAILURE,SUCCESS, FILE_FOUND, FILE_NOT_FOUND, RES, NONRES, OPR, IDEN, 
    BRAC, DELIM, TRUE, FALSE
}Status;

extern const char *reserved[];


extern const char *non_reserved[];

extern const char *operators[];
extern const char *braces[];
extern const char *delimiter[]; //storing delimiter also as a string 
extern const char *preprocessor_directives[];

Status validate(int argc, char *argv[]);
Status file_handler(FILE **fptr, char *file_name);
Status token_type(char *token);
Status is_operator(char token);
Status is_brace(char token);
Status is_delim(char token);
Status tokeniser(char *buffer);
void test();
Status is_directives(char *token);



#endif