#include "text_editor.h"

Status editor_find(text_editor *ted, DLL **head, DLL **tail, char *text)
{
    
    DLL *temp = (*head);
    if (temp == NULL)
    {
        printf("Please insert some charcters in the editor first then try the operation!!!\n");
        return FAILURE;
    }
    int count = 0;
    while (temp != NULL)
    {
        count++;
        char *pos = strstr(temp->str, text);
        if (pos != NULL)
        {
            int char_pos = pos - (temp->str); 
            ted->curs.pos = char_pos;
            ted->curs.line_no = count;
            return FOUND;
        }
        temp = temp->next; 
    }
    return NOTFOUND;
}

Status editor_replace(text_editor *ted, DLL **head, DLL **tail, char *find_text, char *replacer)
{
    save_undo_state(ted);
clear_redo_stack(ted);
    
    
    
    
    if (head == NULL || *head == NULL || find_text == NULL || replacer == NULL)
    {
        return FAILURE;
    }
    int count = 0;
    int find_len = strlen(find_text);
    int rep_len = strlen(replacer);
    DLL *temp = *head;
    int current_line = 1;
    while (temp != NULL)
    {
        char *search_pos = temp->str;
        
        while ((search_pos = strstr(search_pos, find_text)) != NULL)
        {
            int prefix_len = search_pos - temp->str; 
                                                     
            char char_before = (prefix_len > 0) ? temp->str[prefix_len - 1] : ' ';
            char char_after = temp->str[prefix_len + find_len];

            if (!isalnum((unsigned char)char_before) && !isalnum((unsigned char)char_after))
            {
                int old_len = strlen(temp->str);
                int new_len = old_len - find_len + rep_len;
                char *new_str = malloc(new_len + 1); 
                if (new_str == NULL)
                {
                    printf("Error: Memory allocation failed during replacement.\n");
                    return FAILURE;
                }
                strncpy(new_str, temp->str, prefix_len); 
                new_str[prefix_len] = '\0';              
                strcat(new_str, replacer);               
                strcat(new_str, search_pos + find_len);  
                free(temp->str);
                temp->str = new_str;
                search_pos = temp->str + prefix_len + rep_len;

                ted->curs.line_no = current_line;
                ted->curs.pos = prefix_len + rep_len;
                count++;
            }
            else
            {
                
                search_pos += find_len;
            }
        }
        temp = temp->next;
        current_line++;
    }
    if (count > 0)
    {
        printf("Modified text at %d places!!!\n", count);
        modified = 1;
        return REPLACED;
    }
    return NOTREPLACED;
}
