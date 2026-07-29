#include "lexical.h"
 Status tokeniser(char *buffer)
 {
    //the striing we have contains all the words in a file line
    //if blank space neglect it ad elimainate it 
    //if arithmatic -> find its type ->write operaor
    //if keyword -> write keyword
    //if identifier -> write identifier 
    //ex: while ( i != 0)

    char *temp=buffer;
    int i=0;
    char token[100];
    int ind=0;
    while(temp[i] != '\0')
    {
        if(temp[i]==' ')
        {
            i++;
            continue;
        }
        else if(temp[i]=='#')
        {
            //handle preprocessor
            ind=0;
            while (!isspace((unsigned char)temp[i]) && temp[i] != '\0')
            {
                token[ind++]=temp[i++];
            }
            token[ind]='\0';
            if(is_directives(token)==TRUE)
            {
                printf("%s is a Preprocessor Directive\n", token);
            }
            //then skip some spcaes
            while(temp[i]==' ')
            {
                i++;
            }
            ind=0;
            if(temp[i]=='<')
            {
                //readtill next >
                i++; //skip opening <
                while(temp[i]!='>'  && temp[i] != '\0')
                {
                    token[ind++]=temp[i];
                    i++;
                }
                token[ind]='\0';

                if (temp[i] == '>')
                    i++;    // Skip that extra > else becomes stdio.h>
                printf("%s is a Header\n",token);
            }
            else if(temp[i]=='"')
            {
                //read till next doyble quotes 
                //readtill next ""
                i++; //skip opening "   
                while(temp[i]!='"' && temp[i] != '\0')
                {
                    token[ind++]=temp[i];
                    i++;
                }
                token[ind]='\0';

                if (temp[i] == '"')
                i++;    // Skip closing "

                printf("%s is a Header\n",token);
            }
            
        }
        else if ((temp[i]>='a' && temp[i]<='z') || (temp[i]>='A' && temp[i]<='Z') || temp[i]== '_') 
        {
            //read and build the token
            ind=0;
            while(isalnum(temp[i]) || temp[i]=='_')
            {
                token[ind++]=temp[i++];
            }
            token[ind]='\0'; 

            //NNED TO VERIFY WHETHER IT IS A  RESER / NON RESERV/ IDENTIFIER 
            if(token_type(token)==RES)
            {
                printf("%s is an Reserved Keyword\n",token);
            }
            else if(token_type(token)==NONRES)
            {
                printf("%s is a Non Reserved Keyword\n",token);
            }
            else
            {
                //come till her means its an identifier 
                //so print the token 
                printf("%s is an Identifier\n",token);
            }
        }
        else if ((temp[i]>='0' && temp[i]<='9') ) 
        {
            //read and build the number  
            int num=0;
            while(isdigit(temp[i]))
            {
                num=num*10+ temp[i]-'0';
                i++;
            }

            //PRINT IT ALSO 
            printf("%d is an Constant\n",num);
            return SUCCESS;
        }
        else if(is_operator(temp[i])==TRUE)
        {
            //directly print the operator 
            printf("%c is an Operator\n",temp[i]);
            i++;

        }
        else if(is_brace(temp[i])==TRUE)
        {
            //directly print the brace
            printf("%c is a Brace\n",temp[i]);
            i++;
            //i++ ;
        }
        else if(is_delim(temp[i])==TRUE)
        {
            //print the delimite 
            printf("%c is an Delimiter\n",temp[i]);
            i++;
        }
        else
        {
            //its an unknnown char 
            
            i++; //skip unknown
            
        }
    }    
    return SUCCESS;
 }