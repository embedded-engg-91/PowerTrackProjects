#include "text_editor.h"




Status save_undo_state(text_editor *ted)
{
    History *new_history = malloc(sizeof(History));

    if (new_history == NULL)
        return FAILURE;

    if (ted->doc.firstline == NULL)
    {
        new_history->head = NULL;
        new_history->tail = NULL;
        new_history->linecount = 0;
    }
    else
    {
        new_history->head =
            copy_document(ted->doc.firstline,
                          &new_history->tail);

        new_history->linecount = ted->doc.linecount;
    }

    new_history->curs = ted->curs;

    new_history->next = ted->undo_stack;
    ted->undo_stack = new_history;

    return SUCCESS;
}




DLL *copy_document(DLL *head, DLL **tail)
{
    if (head == NULL)
    {
        *tail = NULL;
        return NULL;
    }

    DLL *copy_head = malloc(sizeof(DLL));
    if (copy_head == NULL)
        return NULL;

    copy_head->str = malloc(strlen(head->str) + 1);

    if (copy_head->str == NULL)
    {
        free(copy_head);
        return NULL;
    }

    strcpy(copy_head->str, head->str);
    copy_head->prev = NULL;
    copy_head->next = NULL;

    DLL *copy_temp = copy_head;

    head = head->next;
    while (head != NULL)
    {
        DLL *new_node = malloc(sizeof(DLL));
        new_node->str = malloc(strlen(head->str) + 1);
        strcpy(new_node->str, head->str);

        new_node->prev = copy_temp;
        new_node->next = NULL;

        copy_temp->next = new_node;
        copy_temp = new_node;

        head = head->next;
    }

    *tail = copy_temp;

    return copy_head;
}
Status save_redo_state(text_editor *ted)
{
    History *new_history = malloc(sizeof(History));
    if (new_history == NULL)
        return FAILURE;

    if (ted->doc.firstline == NULL)
    {
        new_history->head = NULL;
        new_history->tail = NULL;
        new_history->linecount = 0;
    }
    else
    {
        new_history->head =
    copy_document(ted->doc.firstline,&new_history->tail);

    new_history->linecount = ted->doc.linecount;
    }
    new_history->curs = ted->curs;

    new_history->next = ted->redo_stack; 
    ted->redo_stack = new_history;

    return SUCCESS;
}

void clear_redo_stack(text_editor *ted)
{
    History *temp = ted->redo_stack; 
    while (temp != NULL)
    {
        History *next_history = temp->next;

        
        DLL *doc_temp = temp->head;
        while (doc_temp != NULL)
        {
            DLL *next_doc = doc_temp->next;
            free(doc_temp->str);
            free(doc_temp);
            doc_temp = next_doc;
        }
        
        free(temp);
        temp = next_history; 
    }

    
    ted->redo_stack = NULL;
}

Status editor_undo(text_editor *ted)
{
    if (ted->undo_stack == NULL)
    {
        printf("Nothing to undo!!!\n");
        return LIST_EMPTY;
    }
    
    save_redo_state(ted);
    
    free_document(&ted->doc.firstline,&ted->doc.lastline);

    History *temp = ted->undo_stack;    

    
    ted->doc.firstline = temp->head;
    ted->doc.lastline = temp->tail;

    ted->doc.linecount = temp->linecount;
    ted->curs = temp->curs;    
    ted->undo_stack = temp->next;    
    free(temp);
    return SUCCESS;
}

Status editor_redo(text_editor *ted)
{
    if (ted->redo_stack == NULL)
    {
        printf("Nothing to redo!!!\n");
        return LIST_EMPTY;
    }
    
    save_undo_state(ted);   
    
    free_document(&ted->doc.firstline,
                  &ted->doc.lastline);

    History *temp = ted->redo_stack;

    
    ted->doc.firstline = temp->head;
    ted->doc.lastline = temp->tail;
    ted->doc.linecount = temp->linecount;

    ted->curs = temp->curs;

    
    ted->redo_stack = temp->next;

    
    free(temp);

    return SUCCESS;
}
void free_document(DLL **head, DLL **tail)
{
    DLL *temp = *head;

    while (temp != NULL)
    {
        DLL *next = temp->next;

        free(temp->str);
        free(temp);

        temp = next;
    }

    *head = NULL;
    *tail = NULL;
}
