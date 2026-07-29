#include "lexical.h"

const char *reserved[]={"const", "volatile", "extern", "auto", "register",
"static", "signed", "unsigned", "short", "long", "double", "char",
"int", "float", "struct", "union", "enum", "void", "typedef", NULL};


const char *non_reserved[]={"goto", "return", "continue", "break", "if",
"else", "for", "while", "do", "switch", "case", "default", NULL};

const char *operators[]={"~", "+", "-", "*", "/", "%", "=", "<", ">", "-", "&",",", "!", "^", "|", NULL};
const char *braces[]={ "{", "}", "(", ")", "[", "]", NULL};
const char *delimiter[]={";", NULL}; //storing delimiter also as a string 

const char *preprocessor_directives[] =
{
    "#include",
    "#define",
    "#undef",
    "#if",
    "#ifdef",
    "#ifndef",
    "#elif",
    "#else",
    "#endif",
    "#error",
    "#pragma",
    "#line",

    NULL
};
Status validate(int argc, char *argv[])
{
    if (argc != 2)
    {
        return FAILURE;
    }

    // at the end we need .c extension
    char *pos = strrchr(argv[1], '.');
    if(pos==NULL) return FAILURE; //if '.' not fiund then directly return failure 
    if (strcmp(pos, ".c") == 0) //no need of extra pointer 
    {
        return SUCCESS;
    }
    return FAILURE;
}
int main(int argc, char *argv[])
{
    if(validate(argc, argv)==SUCCESS)
    {
        FILE *fptr;
        if(file_handler(&fptr, argv[1])==FILE_FOUND) //pass by refernce conceptna 
        {
            //now i nned the tokeniser fucntion and i need ot keep printing thewors ans 
            //after need to close the file as well 
            char buffer[1000];
            while(fgets(buffer, sizeof(buffer), fptr)!=NULL)
            {
                //need to call the tokeniser function for the buffer
                //now buffer has that entire line 
                tokeniser(buffer);
            }
            fclose(fptr); //we also need to close the file 
        }
        else
        {
            printf("The file does not exist.\nPlease enter a valid file name.\n");
        }
    }
    else
    {
        printf("Invalid Input Argments\nPlease Try Again\n");
        
    }
    // after validation call the file handler fun to know wheter file erxosts and open it
    // then get each line from the fiel and pass it to tokeniser untile eof
    return 0;
}