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
    new_node->str = malloc(wordsize); /* dont forget this */
    /*need to cou the data into the dll*/
    strcpy(new_node->str, s); // copied the new_data
    if ((*head) == NULL)      // Here equal to null means list is empty
    {
        (*head) = new_node;
        (*tail) = new_node;
        return SUCCESS;
    }
    // else if((*head)==(*tail))//means there is a single element in the list
    // {

    // }
    // THE ABOVE CONDITIONS PART DOESNT MAKE ANY DIIFERNCE CUZ BE IT ONE OR N TAIL IS
    // ALWAYS POINTING TO THE LAST AND WE NEED TO EDIT THAT, HERE NO NO NODES WONT MATTER, MODIFYING TAIL DOES
    (*tail)->next = new_node;
    new_node->prev = (*tail);
    (*tail) = new_node;
    return SUCCESS;
}

Status dl_delete_node(text_editor *ted, DLL **head, DLL **tail, DLL *node_to_delete) /*node int he dll represents the entire line*/
{
    if ((*head) == NULL) // dont use head==tail cuz addrs wont be same not an array its a dll
    {
        return LIST_EMPTY;
    }
    int count = 1;
    if ((*head)->next == NULL && (*head) == node_to_delete) // list has one element and mtches at first
    {
        free(*head);
        (*head) = (*tail) = NULL;
        return SUCCESS;
    }
    DLL *temp = (*head);
    if ((*head)->next != NULL && (*head) == node_to_delete) // list has multiple element and matches at first
    {
        temp = (*head);
        (*head) = (*head)->next;
        free(temp);
        (*head)->prev = NULL;

        return SUCCESS;
    }
    if ((*head)->next != NULL && (*tail) == node_to_delete) // list has multiple element and matches at last
    {
        temp = (*tail);
        (*tail) = (*tail)->prev;
        free(temp);
        (*tail)->next = NULL;

        return SUCCESS;
    }
    // matching in between
    while (temp != NULL)
    {
        if (temp == node_to_delete)
        {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            free(temp); // free the DLL node

            return SUCCESS;
        }
        temp = temp->next;
    }

    return SUCCESS; /*it can not find a node fail finding a node which must be present right */
}

Status dl_insert_after(DLL **head, DLL **tail, DLL *new_node, DLL *temp) /*gdata, ndata*/
{
    if ((*head) == NULL) // list is empty
    {
        return LIST_EMPTY;
    }
    DLL *temp2 = (*head);
    // if(new_node==NULL)
    // {
    //     return FAILURE;
    // }
    // new_node->prev=NULL;
    // new_node->data=ndata;
    // new_node->next=NULL;
    if (((*tail)) == temp) // case: data matching at the end so simply use tail
    {
        new_node->prev = (*tail);
        (*tail)->next = new_node;
        (*tail) = new_node;
        return SUCCESS;
    }
    while (temp2 != NULL) // if data not matching at end then tarverse and see where it matches
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
