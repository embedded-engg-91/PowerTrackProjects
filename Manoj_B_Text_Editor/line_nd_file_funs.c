#include "text_editor.h"

Status move_home(text_editor *ted, DLL **head, DLL **tail)
{
    
    DLL *temp = goto_cursor(head, tail, &ted->curs);
    if (temp == NULL)
    {
        printf("Please insert a line first.\nThe document is currently empty!!!\n");
        return MOVED;
    }
    ted->curs.pos = 0;
    return MOVED;
}

Status move_end(text_editor *ted, DLL **head, DLL **tail)
{
    
    DLL *temp = goto_cursor(head, tail, &ted->curs);
    if (temp == NULL)
    {
        printf("Please insert a line first.\nThe document is currently empty!!!\n");
        return MOVED;
    }
    ted->curs.pos = strlen(temp->str);
    return MOVED;
}

Status move_start(text_editor *ted, DLL **head, DLL **tail)
{
    DLL *temp = goto_cursor(head, tail, &ted->curs);
    if (temp == NULL)
    {
        printf("Please insert a line first.\nThe document is currently empty!!!\n");
        return MOVED;
    }
    
    ted->curs.line_no = 1;
    move_home(ted, head, tail);
    return MOVED;
}

Status move_finish(text_editor *ted, DLL **head, DLL **tail)
{
    DLL *temp = goto_cursor(head, tail, &ted->curs);
    if (temp == NULL)
    {
        printf("Please insert a line first.\nThe document is currently empty!!!\n");
        return MOVED;
    }
    
    int current_line_no = ted->curs.line_no;
    int count = 0;
    while (temp->next != NULL)
    {

        temp = temp->next;
        ted->curs.line_no++;
    }
    ted->curs.pos = strlen(temp->prev->str);
    return MOVED;
}
