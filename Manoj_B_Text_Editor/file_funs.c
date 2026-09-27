#include "text_editor.h"

Status valid_filename(char *filename)
{
    char *pos = strrchr(filename, '.');
    if (pos == NULL)
    {
        printf("Please enter a valid file name with proper file format.!!!\n");
        return INVALID;
    }
    if (strcmp(pos, ".txt") == 0) 
    {
        return VALID;
    }
    printf("Please enter a valid file name with proper file format.!!!\n");

    return INVALID;
}

Status file_open(text_editor *ted, DLL **head, DLL **tail, char *filename)
{
    
    FILE *fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        printf("Failure to open file!!!\n");
        printf("Provide name of a file that exists\n");
        return FAILURE;
    }

    char buffer[wordsize];

    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        
        buffer[strcspn(buffer, "\n")] = '\0';

        if (dl_insert_last(head, tail, buffer) != SUCCESS)
        {
            fclose(fptr);
            return FAILURE;
        }

        ted->doc.linecount++;
    }

    fclose(fptr);

    ted->doc.firstline = *head;
    ted->doc.lastline = *tail;

    
    ted->curs.line_no = 1;
    ted->curs.pos = 0;
    modified = 1;
    return LOADED;
}

Status file_save(text_editor *ted, DLL **head, DLL **tail, char *filename)
{
    FILE *fptr = fopen(filename, "w");
    
    if ((*head) == NULL)
    {
        printf("The file is empty!!!\nPlease first enter some content into the editor\n");
        return FAILURE;
    }
    DLL *temp = (*head);
    while (temp != NULL)
    {
        fprintf(fptr, "%s\n", temp->str);
        temp = temp->next;
    }
    fclose(fptr);
    printf("All the contents of the editor have been saved in your file!!!\n");
    return SAVED;
}

Status file_close(text_editor *ted, DLL **head, DLL **tail)
{
    
    if (*head == NULL)
    {
        printf("The editor is already empty!!!\nPlease insert some content first!!!\n");
        return FAILURE;
    }
    DLL *temp = (*head);
    while (temp != NULL)
    {
        DLL *next = temp->next;
        free(temp->str); 
        free(temp);
        temp = next;
    }
    (*head) = NULL;
    (*tail) = NULL;
    ted->doc.firstline = NULL;
    ted->doc.lastline = NULL;
    ted->doc.linecount = 0;
    ted->curs.line_no = 1; 
    ted->curs.pos = 0;
    return DELETED;
}
