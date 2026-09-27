#include "text_editor.h"

Status move_up(text_editor *ted, DLL **head, DLL **tail)
{
    DLL *temp = goto_cursor(head, tail, &ted->curs); 
    if (temp == NULL)
    {
        printf("No line exists!!!\n");
        return FAILURE;
    }
    if (temp->prev == NULL) 
    {
        printf("Cursor is already at the top line of the document.!!!\n");
        return MOVED;
    }
    else
    {
        int curr_line_len = strlen(temp->str);

        DLL *prev = temp->prev;
        int prev_line_len = strlen(prev->str);

        if (prev_line_len >= curr_line_len)
        {
            
            
            (ted->curs.line_no)--;
            return MOVED;
        }
        else
        {
            
            ted->curs.pos = strlen(prev->str);
            (ted->curs.line_no)--;
            return MOVED;
        }
    }
    return FAILURE;
}
Status move_down(text_editor *ted, DLL **head, DLL **tail)
{
    DLL *temp = goto_cursor(head, tail, &ted->curs); 
    if (temp == NULL)
    {
        printf("No line exists!!!\n");
        return FAILURE;
    }
    if (temp->next == NULL) 
    {
        printf("Cursor is already at the last line of the document.!!!\n");
        return MOVED;
    }
    else
    {
        int curr_line_len = strlen(temp->str);

        DLL *next = temp->next;
        int next_line_len = strlen(next->str);

        if (next_line_len >= curr_line_len)
        {
            
            
            (ted->curs.line_no)++;
            return MOVED;
        }
        else
        {
            
            ted->curs.pos = strlen(next->str);
            (ted->curs.line_no)++;
            return MOVED;
        }
    }
    return FAILURE;
}

Status move_left(text_editor *ted, DLL **head, DLL **tail)
{
    int current_curs_pos = ted->curs.pos;
    if (current_curs_pos == 0)
    {
        printf("The Cursor is already at the start of the line.\nCannot move any more left\n");
        return MOVED;
    }
    (ted->curs.pos)--; 
    printf("Cursor has been moved one char towards left!!!\n");
    return MOVED;
}

Status move_right(text_editor *ted, DLL **head, DLL **tail)
{
    DLL *temp = goto_cursor(head, tail, &ted->curs); 
    if (temp == NULL)
    {
        printf("No line exists!!!\nPlease insert some text first!!!\n");
        return FAILURE;
    }
    int current_curs_pos = ted->curs.pos;
    if (current_curs_pos == strlen(temp->str))
    {
        printf("The Cursor is already at the end of the line.\nCannot move any more right\n");
        return MOVED;
    }
    (ted->curs.pos)++; 
    printf("Cursor has been moved one char towards right!!!\n");
    return MOVED;
}
