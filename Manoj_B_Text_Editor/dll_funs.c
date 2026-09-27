#include "text_editor.h"

int dl_insert_last(DLL **head, DLL **tail, char *s)
{
    DLL *new_node = malloc(sizeof(DLL));
    if (new_node == NULL)
    {
        return FAILURE;
    }
    new_node->prev = NULL;
    new_node->next = NULL;
    new_node->str = malloc(wordsize); 
    
    strcpy(new_node->str, s); 
    if ((*head) == NULL)      
    {
        (*head) = new_node;
        (*tail) = new_node;
        return SUCCESS;
    }
    
    

    
    
    
    (*tail)->next = new_node;
    new_node->prev = (*tail);
    (*tail) = new_node;
    return SUCCESS;
}

Status dl_delete_node(text_editor *ted, DLL **head, DLL **tail, DLL *node_to_delete) 
{
    if ((*head) == NULL) 
    {
        return LIST_EMPTY;
    }
    int count = 1;
    if ((*head)->next == NULL && (*head) == node_to_delete) 
    {
        free(*head);
        (*head) = (*tail) = NULL;
        return SUCCESS;
    }
    DLL *temp = (*head);
    if ((*head)->next != NULL && (*head) == node_to_delete) 
    {
        temp = (*head);
        (*head) = (*head)->next;
        free(temp);
        (*head)->prev = NULL;

        return SUCCESS;
    }
    if ((*head)->next != NULL && (*tail) == node_to_delete) 
    {
        temp = (*tail);
        (*tail) = (*tail)->prev;
        free(temp);
        (*tail)->next = NULL;

        return SUCCESS;
    }
    
    while (temp != NULL)
    {
        if (temp == node_to_delete)
        {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            free(temp); 

            return SUCCESS;
        }
        temp = temp->next;
    }

    return SUCCESS; 
}

Status dl_insert_after(DLL **head, DLL **tail, DLL *new_node, DLL *temp) 
{
    if ((*head) == NULL) 
    {
        return LIST_EMPTY;
    }
    DLL *temp2 = (*head);
    
    
    
    
    
    
    
    if (((*tail)) == temp) 
    {
        new_node->prev = (*tail);
        (*tail)->next = new_node;
        (*tail) = new_node;
        return SUCCESS;
    }
    while (temp2 != NULL) 
    {
        if (temp2 == temp)
        {
            new_node->next = temp2->next;
            new_node->prev = temp2;
            temp2->next->prev = new_node;
            temp2->next = new_node;
            return SUCCESS;
        }
        temp2 = temp2->next;
    }
    return FAILURE;
}
