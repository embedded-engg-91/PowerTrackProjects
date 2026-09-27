#include "text_editor.h"

DLL *goto_cursor(DLL **head, DLL **tail, cursor *curs)
{
    if ((*head) == NULL)
        return NULL;
    int count = 1;
    DLL *temp = (*head);
    int l_no = curs->line_no;
    int pos = curs->pos;
    
    while (temp != NULL && count != l_no)
    {
        count++;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("The line number does not exist.\n");
        return NULL;
    }
    
    
    
    
    
    
    
    return temp;
}

Status insert(text_editor *ted, char *str, DLL **head, DLL **tail)
{
    
    
    

    
    

    save_undo_state(ted);
clear_redo_stack(ted);
    if (ted->doc.firstline == NULL) 
    {
        dl_insert_last(head, tail, str); 
        ted->doc.firstline = (*head);    
        ted->doc.lastline = (*tail);
        (ted->doc.linecount)++;
        ted->curs.line_no = ted->doc.linecount;
        ted->curs.pos = strlen(str); 
    }
    else
    {
        
        
        
        DLL *temp;
        temp = goto_cursor(head, tail, &ted->curs); 
        if (temp == NULL)
            return FAILURE;
        int old_len = strlen(temp->str);
        int insert_len = strlen(str);
        int new_len = old_len + insert_len;

        char *new_str = malloc(new_len + 1); 
        if (new_str == NULL)
        {
            printf("Error: Memory allocation failed during insert.\n");
            return FAILURE;
        }
        

        

        
        
        
        
        
        
        

        
        
        
        
        
        

        
        
        strncpy(new_str, temp->str, ted->curs.pos); 
        new_str[ted->curs.pos] = '\0';              
        strcat(new_str, str);                       
        strcat(new_str, temp->str + ted->curs.pos); 
        

        free(temp->str);
        temp->str = new_str;
        ted->curs.pos = ted->curs.pos + insert_len;
    }
    modified = 1;
    return SUCCESS;
}

Status delete_chars(text_editor *ted, int count, DLL **head, DLL **tail)
{
   save_undo_state(ted);
clear_redo_stack(ted);
    DLL *temp;

    temp = goto_cursor(head, tail, &ted->curs); 
    if (temp == NULL)
    {
        printf("Line does not exist\n");
        return LIST_EMPTY;
    }

    int line_len = strlen(temp->str);            
    if (ted->curs.pos == 0 && count >= line_len) 
    {
        
        Status check = dl_delete_node(ted, head, tail, temp); 
        int old_line_pos = ted->curs.line_no;
        if (check == LIST_EMPTY) 
        {
            printf("Cannot delete the specified number of chars as the document is empty!!!\n");
            return LIST_EMPTY;
        }
        else if (check == SUCCESS) 
        {
            if (old_line_pos > 1)
            {
                ted->curs.line_no = old_line_pos - 1; 
            }
            else
            {
                ted->curs.line_no = 1; 
            }
            ted->curs.pos = 0; 

            (ted->doc.linecount)--; 

            printf("No. of chars from the current cursor position Deleted Suceessfully!!!\n");

            return DELETED;
        }
    }
    int remaining_chars = line_len - ted->curs.pos; 
    if (count > remaining_chars)                    
    {
        count = remaining_chars; 
    }
    
    char before_curs_text[wordsize];
    char after_curs_text[wordsize];
    int ind1 = 0;
    for (int i = ted->curs.pos + count; temp->str[i] != '\0'; i++)
    {
        after_curs_text[ind1++] = temp->str[i];
    }
    after_curs_text[ind1] = '\0';

    int ind2 = 0;
    for (int i = 0; i < ted->curs.pos; i++)
    {
        before_curs_text[ind2++] = temp->str[i];
    }
    before_curs_text[ind2] = '\0';

    
    strcat(before_curs_text, after_curs_text); 

    strcpy(temp->str, before_curs_text); 
    

    modified = 1;
    return DELETED;
}

Status create_new_line(text_editor *ted, DLL **head, DLL **tail)
{
save_undo_state(ted);
clear_redo_stack(ted);
    
    
    
    
    
    
    
    
    
    DLL *temp = goto_cursor(head, tail, &ted->curs);
    if (temp == NULL)
    {
        printf("Line does not exist");
        return FAILURE;
    }
    char buffer[wordsize];
    int ind = 0;
    for (int i = ted->curs.pos; temp->str[i] != '\0'; i++) 
    {
        if (temp->str[i] == '\0')
            continue;
        buffer[ind++] = temp->str[i];
    }
    buffer[ind] = '\0';
    temp->str[ted->curs.pos] = '\0'; 

    int old_line_no = ted->curs.line_no;
    DLL *new_node = malloc(sizeof(DLL));
    new_node->str = malloc(wordsize); 
    new_node->next = NULL;
    new_node->prev = NULL;
    strcpy(new_node->str, buffer);

    if (dl_insert_after(head, tail, new_node, temp) == SUCCESS) 
    {
        
        (ted->doc.linecount)++;
        ted->curs.line_no = old_line_no + 1;
        ted->curs.pos = strlen(buffer); 
    }
    modified = 1;
    return NEW_LINE;
}

Status delete_line(text_editor *ted, DLL **head, DLL **tail)
{
    save_undo_state(ted);
clear_redo_stack(ted);
    DLL *temp = goto_cursor(head, tail, &ted->curs);

    if (temp == NULL)
    {
        printf("No line exists!!!\n");
        return FAILURE;
    }

    
    if (temp->prev == NULL && temp->next == NULL)
    {
        dl_delete_node(ted, head, tail, temp);
        ted->doc.firstline = NULL;
        ted->doc.lastline = NULL;
        ted->curs.line_no = 1;
        ted->curs.pos = 0;
        ted->doc.linecount = 0;
        return DELETED;
    }

    
    if (temp->next != NULL)
    {

        DLL *next = temp->next;

        dl_delete_node(ted, head, tail, temp);
        ted->doc.firstline = *head;
        ted->doc.lastline = *tail;
        ted->curs.pos = (next->str != NULL) ? strlen(next->str) : 0;
        ted->doc.linecount--;
        return DELETED;
    }

    

    DLL *prev = temp->prev;

    dl_delete_node(ted, head, tail, temp);
    ted->doc.firstline = *head;
    ted->doc.lastline = *tail;
    ted->curs.line_no--;
    ted->curs.pos = (prev->str != NULL) ? strlen(prev->str) : 0;
    ted->doc.linecount--;
    modified = 1;
    return DELETED;
}
