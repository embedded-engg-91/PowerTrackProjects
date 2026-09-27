#include "text_editor.h"

DLL *goto_cursor(DLL **head, DLL **tail, cursor *curs)
{
    if ((*head) == NULL)
        return NULL;
    int count = 1;
    DLL *temp = (*head);
    int l_no = curs->line_no;
    int pos = curs->pos;
    // NEED TO FIX WHAT IF THE USERT REIS TO GO TOWARDS A LINE THAT DOES NOT EXIST ONLY
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
    // if(count==l_no)
    // {
    //     /*then i need to move inside this lines content like position*/
    //     char buffer[wordsize];
    //     strcpy(buffer,temp->str);/*temp is already poiting at that line no */
    //     curs->pos=buffer[curs->pos];
    // }
    return temp;
}

Status insert(text_editor *ted, char *str, DLL **head, DLL **tail)
{
    /* insert the txt into document at the current cursor position*/
    /*if there is any terxt, shfit it towards right*/
    /*after insertion the cursor comes to the end of that inserted text*/

    /*if the firstline is nulll then create a new dll and make it pint towards the firstlin*/
    /*if the firstline is not null then i need to traverse to that line no dll and inside that to that poas then insert*/

    save_undo_state(ted);
clear_redo_stack(ted);
    if (ted->doc.firstline == NULL) // means the document is empty
    {
        dl_insert_last(head, tail, str); // only createa new nodd and cpoy the str
        ted->doc.firstline = (*head);    /*see head and taill got modified for first line*/
        ted->doc.lastline = (*tail);
        (ted->doc.linecount)++;
        ted->curs.line_no = ted->doc.linecount;
        ted->curs.pos = strlen(str); /* gonna makke hime piint twards the last inserted text char*/
    }
    else
    {
        /*now if there is already content inside the document then */
        /*we DONT need to go ANYWHERE MAN WE NEED TO INSERT AT THAT PSOTION ITSELf then shift then insert then shift again*/
        /*do not forget to upadte the line no and cursoe position*/
        DLL *temp;
        temp = goto_cursor(head, tail, &ted->curs); // tmep is the addrs of the DLL  that we need to copy the str from
        if (temp == NULL)
            return FAILURE;
        int old_len = strlen(temp->str);
        int insert_len = strlen(str);
        int new_len = old_len + insert_len;

        char *new_str = malloc(new_len + 1); // +1 for the null terminator
        if (new_str == NULL)
        {
            printf("Error: Memory allocation failed during insert.\n");
            return FAILURE;
        }
        /*dont care whether the next char is null or not */

        // char before_curs_text[wordsize];

        // char after_curs_text[wordsize];
        // int ind1=0;
        // for (int i = ted->curs.pos; temp->str[i]!='\0'; i++)
        // {
        //     after_curs_text[ind1++]=temp->str[i];
        // }
        // after_curs_text[ind1]='\0';

        // int ind2=0;
        // for (int i = 0; i<ted->curs.pos; i++)
        // {
        //     before_curs_text[ind2++]=temp->str[i];
        // }
        // before_curs_text[ind2]='\0';

        // strcpy(buffer, temp->str[ted->curs.pos]);
        // strcat(before_curs_text, " " ); /*append space*/
        strncpy(new_str, temp->str, ted->curs.pos); // Copy text BEFORE the cursor
        new_str[ted->curs.pos] = '\0';              // Null-terminate explicitly
        strcat(new_str, str);                       /*append text*/
        strcat(new_str, temp->str + ted->curs.pos); // Append text AFTER the cursor
        // strcpy(temp->str, before_curs_text);/* do not miss to writ back to DLLs str*/

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

    temp = goto_cursor(head, tail, &ted->curs); // tmep is the addrs of the DLL  that we need to delete the no of char in str from
    if (temp == NULL)
    {
        printf("Line does not exist\n");
        return LIST_EMPTY;
    }

    int line_len = strlen(temp->str);            /*temp is a dll*/
    if (ted->curs.pos == 0 && count >= line_len) /*if the cursor is at first positiona dn the count is greate then length only then delete the entire line*/
    {
        /*here we need to delete the enotre line and link the prev node with the next node */
        Status check = dl_delete_node(ted, head, tail, temp); /* to make sure the function is called only once*/
        int old_line_pos = ted->curs.line_no;
        if (check == LIST_EMPTY) /*node int he dll represents the entire line*/
        {
            printf("Cannot delete the specified number of chars as the document is empty!!!\n");
            return LIST_EMPTY;
        }
        else if (check == SUCCESS) /*node int he dll represents the entire line*/
        {
            if (old_line_pos > 1)
            {
                ted->curs.line_no = old_line_pos - 1; // Move up one line if we aren't at the top
            }
            else
            {
                ted->curs.line_no = 1; // Stay on line 1 if we deleted the top line
            }
            ted->curs.pos = 0; // Reset horizontal cursor to the start of the line

            (ted->doc.linecount)--; /*dont forget to reduce the linecount cuz we dleted entire line */

            printf("No. of chars from the current cursor position Deleted Suceessfully!!!\n");

            return DELETED;
        }
    }
    int remaining_chars = line_len - ted->curs.pos; /*if i am 5th char there are 10 chars and user enter 12 then we need to remove from 5 */
    if (count > remaining_chars)                    /*important case */
    {
        count = remaining_chars; // Only delete up to the end of the line
    }
    /*every other time */
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

    // strcpy(buffer, temp->str[ted->curs.pos]);
    strcat(before_curs_text, after_curs_text); /*append previous text*/

    strcpy(temp->str, before_curs_text); /* do not miss to writ back to DLLs str*/
    /*no need to toucht he cursor pistion*/

    modified = 1;
    return DELETED;
}

Status create_new_line(text_editor *ted, DLL **head, DLL **tail)
{
save_undo_state(ted);
clear_redo_stack(ted);
    /*to create a newline onto the editor we just need to append one more node tp the list*/
    /*after creating no the cursor must point ti that line, 1st char*/
    /*the newline must be after the current cusor posiiton menaing after the line where cursor was pointing already*/
    // Copy from cursor.pos until the end
    // Put '\0' at cursor.pos in the current line
    // Create a new DLL node
    // Put the copied portion into the new node
    // Link the new node after the current node
    // Move cursor to the new line
    DLL *temp = goto_cursor(head, tail, &ted->curs);
    if (temp == NULL)
    {
        printf("Line does not exist");
        return FAILURE;
    }
    char buffer[wordsize];
    int ind = 0;
    for (int i = ted->curs.pos; temp->str[i] != '\0'; i++) /*cursor is already pointnig ath the psotion after inserted*/
    {
        if (temp->str[i] == '\0')
            continue;
        buffer[ind++] = temp->str[i];
    }
    buffer[ind] = '\0';
    temp->str[ted->curs.pos] = '\0'; /*currently where he is poitnig append null */

    int old_line_no = ted->curs.line_no;
    DLL *new_node = malloc(sizeof(DLL));
    new_node->str = malloc(wordsize); /*dont forget to allocatr memory for the string */
    new_node->next = NULL;
    new_node->prev = NULL;
    strcpy(new_node->str, buffer);

    if (dl_insert_after(head, tail, new_node, temp) == SUCCESS) /*gdata, ndata*/
    {
        // TAKE ACRE OF SETTING THE CURSOR POSITON and also need ti increae the line nos
        (ted->doc.linecount)++;
        ted->curs.line_no = old_line_no + 1;
        ted->curs.pos = strlen(buffer); /*+1 cuz we wnat him to point after inserted character*/
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

    /* Only one line exists */
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

    /* Next line exists means
       Cursor should move to next line */
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

    /* No next line
       Therefore cursor moves to previous line */

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