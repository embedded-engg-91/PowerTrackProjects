#include "text_editor.h"

char *commands[] = {"insert", "delete_line", "delete", "newline", "up", "down", "left", "right", "home",
                    "end", "start", "finish", "copy", "cut", "paste", "open", "print",
                    "save", "close", "find", "replace", "undo", "redo", "exit", "help",NULL};

Status validate(char *s) /*pass by reference*/
{
    char buffer[wordsize];
    strcpy(buffer, s);
    char *tok1 = strtok(buffer, " \n");
    // if (tok1 == NULL)
    // {
    //     return INVALID;
    // }
    char *tok2 = strtok(NULL, "\n"); /*second arg must always be delimiter */
    int flag = 0;
    for (int i = 0; commands[i] != NULL; i++)
    {
        if (strcmp(tok1, commands[i]) == 0)
        {
            flag = 1;
            break;
        }
    }
    if (flag)
        return VALID;
    return INVALID;
}

Status jump_to_fun(char *s, text_editor *ted, DLL **head, DLL **tail)
{
    char buffer[wordsize];
    strcpy(buffer, s);

    char *tok1 = strtok(buffer, " \n");

    char *tok2 = NULL;
    char *tok3 = NULL;

    if (strcmp(tok1, "replace") == 0)
    {
        tok2 = strtok(NULL, " \n"); /*old text part */
        tok3 = strtok(NULL, "\n"); /* new text part */
    }
    else
    {
        tok2 = strtok(NULL, "\n"); /*normal two part cmd */
    }
    if (strncmp(s, "insert", 6) == 0)
    {

        if(tok2 == NULL) 
        {
            printf("Please retype the command with the text you want to insert!!!\n");
            return FAILURE;
        }
        if (insert(ted, tok2, head, tail) == SUCCESS) /*pass the data direclty */
            return INSERTED;
    }
    else if (strncmp(s, "delete_line", 11) == 0)
    {
        if (delete_line(ted, head, tail) == DELETED)
        {
            printf("Delete line operation successfull!!!\n");
        }
    }
    else if (strncmp(s, "delete", 6) == 0)
    {
        if(tok2 == NULL) 
        {
            printf("Please retype the command with the no of characters you want to delete!!!\n");
            return FAILURE;
        }
        return delete_chars(ted, atoi(tok2), head, tail);
    }
    else if (strncmp(s, "newline", 7) == 0)
    {
        create_new_line(ted, head, tail);
    }
    else if (strncmp(s, "up", 2) == 0)
    {
        move_up(ted, head, tail);
    }
    else if (strncmp(s, "down", 4) == 0)
    {
        move_down(ted, head, tail);
    }
    else if (strncmp(s, "left", 4) == 0)
    {
        move_left(ted, head, tail);
    }
    else if (strncmp(s, "right", 4) == 0)
    {
        move_right(ted, head, tail);
    }
    else if (strncmp(s, "home", 4) == 0)
    {
        move_home(ted, head, tail);
    }
    else if (strncmp(s, "end", 3) == 0)
    {
        move_end(ted, head, tail);
    }
    else if (strncmp(s, "start", 5) == 0)
    {
        move_start(ted, head, tail);
    }
    else if (strncmp(s, "finish", 6) == 0)
    {
        move_finish(ted, head, tail);
    }
    else if (strncmp(s, "copy", 4) == 0)
    {
        if(tok2 == NULL) 
        {
            printf("Please retype the command with the no of characters you want to copy!!!\n");
            return FAILURE;
        }
        if (copy_text(ted, head, tail, atoi(tok2)) == COPIED)
        {
            printf("Copy Characters Successfull!!!\n");
        }
    }
    else if (strncmp(s, "cut", 3) == 0)
    {
        if(tok2 == NULL) 
        {
            printf("Please retype the command with the no of characters you want to cut!!!\n");
            return FAILURE;
        }
        if (cut_text(ted, head, tail, atoi(tok2)) == CUT)
        {
            printf("Cut Characters Successfull!!!\n");
        }
    }
    else if (strncmp(s, "paste", 5) == 0)
    {
        if (paste_text(ted, head, tail) == PASTE)
        {
            printf("Pasted content of clipboard into the document Successfull!!!\n");
        }
    }

    else if (strncmp(s, "open", 4) == 0)
    {

        if(tok2 == NULL) 
        {
            printf("Please retype the command with the file name with .txt extension!!!\n");
            return FAILURE;
        }
        if (valid_filename(tok2) == VALID)
        {
            if (file_open(ted, head, tail, tok2) == LOADED)
            {
                printf("Your file has been successfully loaded into the docoument\n");
            }
        }
        // else this taken care in fucntion itself
        // {
        //     printf("Please provide a valid  filename with .txt extension");
        // }
    }
    else if (strncmp(s, "save", 4) == 0)
    {
        if(tok2 == NULL) 
        {
            printf("Please retype the command with the filename you want to save in!!!\n");
            return FAILURE;
        }
        if (valid_filename(tok2) == VALID)
        {
            if (file_save(ted, head, tail, tok2) == SAVED)
            {
                printf("Your file has been Successfully Saved!!!\n");
            }
        }
        // else //this taken care in function itself
        // {
        //     printf("Please provide a valid  filename with .txt extension");
        // }
    }
    else if (strncmp(s, "close", 5) == 0)
    {

        if (file_close(ted, head, tail) == CLOSED)
        {
            printf("The editor has been erased completely!!!\n");
        }
    }
    else if (strncmp(s, "find", 4) == 0)
    {
        if(tok2 == NULL) 
        {
            printf("Please retype the command with the text you want to find!!!\n");
            return FAILURE;
        }
        Status status = editor_find(ted, head, tail, tok2);
        if (status == FOUND)
        {
            printf("The required text has been found and the cursor is set to that position!!!\n");
        }
        else if (status == NOTFOUND)
        {
            printf("The required text is not present in the editor!!!\n");
        }
    }
    else if (strncmp(s, "replace", 7) == 0)
    {

        if(tok2 == NULL) 
        {
            printf("Please retype the command with the text you want to replace with!!!\n");
            return FAILURE;
        }
        Status status = editor_replace(ted, head, tail, tok2, tok3); // FIX REPLACER NEED HIS OWN PARSING LOGIC
        if (status == REPLACED)
        {
            printf("Replace operation has been performed successfully!!!\n");
        }
        // else if( status == NOTFOUND) /*taken care in function itself
        // {
        //         printf("The required text is not present in the editor!!!\n");

        //     }
    }
    else if (strncmp(s, "undo", 4) == 0)
    {
        Status stat = editor_undo(ted);

        if (stat == SUCCESS)
        {
            *head = ted->doc.firstline; /*mistake were causing seg faults*/
            *tail = ted->doc.lastline;

            printf("Undo operation performed Successfully\n");
        }
    }
    else if (strncmp(s, "redo", 4) == 0)
    {
        Status stat = editor_redo(ted);

        if (stat == SUCCESS)
        {
            *head = ted->doc.firstline;
            *tail = ted->doc.lastline;

            printf("Redo operation performed Successfully\n");
        }
    }
    else if (strncmp(s, "exit", 4) == 0)
    {
        if (modified)
        {
            printf("Changes have been found in the editor!!!\n");
            printf("Save changes before exiting? Y/N\n");
            char ch;
            if (scanf("%c", &ch) != 1)
            {

                printf("Enter a valid option!!!\n");
            }
            int temp; /* to clear the input buffer */
            while ((temp = getchar()) != '\n' && temp != EOF)
                ;
            if (ch == 'Y' || ch == 'y')
            {
                char buffer[wordsize];
                printf("Please retype the command with the filename you want the document to be saved as: \n");
                printf("NOTE: Saving Supported only in .txt file formats!!!\n");
                if (fgets(buffer, sizeof(buffer), stdin) != NULL)
                {

                    buffer[strcspn(buffer, "\n")] = '\0';
                    strcat(buffer, ".txt");
                    file_save(ted, head, tail, buffer);
                }
            }
            return EXIT;
        }
        return EXIT;
    }
    else if (strncmp(s, "print", 5) == 0)
    {
        print_list(*head);
    }
    else if (strncmp(s, "help", 4) == 0)
    {
        printMenu();
    }
    return SUCCESS;
}
