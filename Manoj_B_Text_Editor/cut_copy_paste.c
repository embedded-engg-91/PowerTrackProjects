#include "text_editor.h"

int modified = 0;

Status copy_text(text_editor *ted, DLL **head, DLL **tail, int count)
{
    DLL *temp = goto_cursor(head, tail, &ted->curs);
    if (temp == NULL)
    {
        printf("No line exists!!!\nPlease insert a line.!!!\n");
        return FAILURE;
    }
    int available = strlen(temp->str) - ted->curs.pos; 

    if (available == 0)
    {
        printf("Nothing to copy. Cursor is at the end of the line.\n");
        return FAILURE;
    }

    if (count > available) 
        count = available; 

    free(ted->clipboard.data); 

    ted->clipboard.data = malloc(count + 1); 

    memcpy(ted->clipboard.data, temp->str + ted->curs.pos, count); 

    ted->clipboard.data[count] = '\0';
#ifdef DEBUG
    printf("The data inside clipboard is : %s\n", ted->clipboard.data);
#endif
    ted->clipboard.clipboard_size = count; 

    modified = 1;
    return COPIED;
}

Status cut_text(text_editor *ted, DLL **head, DLL **tail, int count)
{
    save_undo_state(ted);
clear_redo_stack(ted);
    Status status = copy_text(ted, head, tail, count); 

    if (status == FAILURE)
        return FAILURE;
    DLL *temp = goto_cursor(head, tail, &ted->curs);
    if (temp == NULL)
    {
        printf("The document is empty!!!\nPlease insert some content first!!!\n");
        return CUT;
    }
    char buffer[wordsize];
    strcpy(buffer, (temp->str) + count);
    temp->str = realloc(temp->str, strlen(temp->str) - count + 1); 
    strcpy(temp->str, buffer);

    modified = 1;
    return CUT;
}

Status paste_text(text_editor *ted, DLL **head, DLL **tail)
{
   save_undo_state(ted);
clear_redo_stack(ted);
    DLL *temp = goto_cursor(head, tail, &ted->curs);
    if (temp == NULL)
    {
        printf("The document is empty!!!\nPlease insert some content first!!!\n");
        return FAILURE;
    }
    if (ted->clipboard.data == NULL)
    {
        printf("The clipboard is empty!!!\n");
        printf("Please copy some text first!!!\n");
        return FAILURE;
    }
    int count = ted->clipboard.clipboard_size; 

    char *new_str = realloc(temp->str, strlen(temp->str) + count + 1);

    if (new_str == NULL)
    {
        printf("Memory allocation failed!!!\n");
        return FAILURE;
    }

    temp->str = new_str;
    memmove(temp->str + ted->curs.pos + count, 
            temp->str + ted->curs.pos,
            strlen(temp->str) - ted->curs.pos + 1);
    memcpy(temp->str + ted->curs.pos, 
           ted->clipboard.data,
           count);

    ted->curs.pos += count; 

    modified = 1;
    return PASTE;
}
