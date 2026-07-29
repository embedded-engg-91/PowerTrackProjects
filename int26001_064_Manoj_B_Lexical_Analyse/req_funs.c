#include "lexical.h"

Status token_type(char *token)
{
    //nedd to iterate theought the array of stgs and comapre and 
    //see whther it matches or noot 
    int i=0;
    while(reserved[i]!=NULL)
    {
        if(strcmp(reserved[i], token) == 0)
        {
            return RES;
        }
        i++;
    }
    /* come out of this loop means not a reserved keyword*/
    i=0; //reset for non reserved na 
    while(non_reserved[i]!=NULL)
    {
        if(strcmp(non_reserved[i], token) == 0)
        {
            return NONRES;
        }
        i++;
    }
    /*if come till here means it is not a non reseved keyowrd also */
    
    /*if all the above fails means the token is deifnatrly ands identifier*/
    return IDEN;
}
Status is_operator(char token)
{
    int i=0; 
    while(operators[i]!=NULL)
    {
        if(operators[i][0] == token)
        {
            return TRUE;
        }
        i++;
    }
    return FALSE;
}
Status is_brace(char token)
{
    int i=0; //reset for braces na 
    while(braces[i]!=NULL)
    {
        if(braces[i][0] == token)
        {
            return TRUE;
        }
        i++;
    }
    return FALSE;
}

Status is_delim(char token)
{
    int i=0; //reset for delimiter na 
    while(delimiter[i]!=NULL)
    {
        if(delimiter[i][0] == token)
        {
            return TRUE;
        }
        i++;
    }
    return FALSE;
}


Status is_directives(char *token)
{
    int i=0; //reset for delimiter na 
    while(preprocessor_directives[i]!=NULL)
    {
        if(strcmp(preprocessor_directives[i], token) == 0)
        {
            return TRUE;
        }
        i++;
    }
    return FALSE;
}