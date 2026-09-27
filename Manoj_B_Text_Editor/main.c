#include "text_editor.h"

int wordsize = 1024;

int main()
{
    text_editor ted;
    initialise(&ted);
    char *input = malloc(wordsize);
    printMenu();
    printf("Enter the command of operation you want to perform:\n");
    DLL *head, *tail;
    head = NULL;
    tail = NULL;
    while (1)
    {
        fgets(input, wordsize, stdin);
        input[strcspn(input, "\n")] = '\0'; // Strip newline character
        if (validate(input) == VALID)
        {
            Status check = jump_to_fun(input, &ted, &head, &tail);
            if (check == INSERTED)
            {
                printf("Your text has been inserted into the editor successfully!!!\n");
            }
            if (check == DELETED)
            {
                printf("Delete Operation Performed Successfully!!!\n");
            }
            if (check == NEW_LINE)
            {
                printf("Newline has been inserted into the document Successfully!!!\n");
            }
            if( check == EXIT)
            {
                return 0;
            }
        }
        else
        {
            printf("Pleas enter a valid command and try again!!!\n");
        }
    }
}