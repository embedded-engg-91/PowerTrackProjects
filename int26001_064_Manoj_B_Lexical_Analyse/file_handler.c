#include "lexical.h"

Status file_handler(FILE **fptr, char *file_name)
{
    (*fptr)=fopen(file_name, "r");
    if(*fptr == NULL)
    return FILE_NOT_FOUND;

    return FILE_FOUND;
}