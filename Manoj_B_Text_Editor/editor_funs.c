#include "text_editor.h"

Status editor_find(text_editor *ted, DLL **head, DLL **tail, char *text)
{
    /*Start at *head.
 Search the current line's str for the requested text.
 If found:
 determine the character position where it starts
 determine which line it is
 update ted->curs.line_no
 update ted->curs.pos
 return success.
 Otherwise move to the next DLL node.
 If you reach NULL, the text wasn't found.*/
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
            int char_pos = pos - (temp->str); /*mistake we had it backwards */
            ted->curs.pos = char_pos;
            ted->curs.line_no = count;
            return FOUND;
        }
        temp = temp->next; /*measn pos was null means not found in that dll*/
    }
    return NOTFOUND;
}

Status editor_replace(text_editor *ted, DLL **head, DLL **tail, char *find_text, char *replacer)
{
    save_undo_state(ted);
clear_redo_stack(ted);
    /*need to find the text in each line andd replace it with igven str*/
    /*throughou tthe document the text sshall be replaced*/
    /*as of now find is finding the first occur  of the word*/
    /*we find it , repalce it and call find again then that will be first and will get replaced and so on*/
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
        // char before_curs_text[wordsize];
        while ((search_pos = strstr(search_pos, find_text)) != NULL)
        {
            int prefix_len = search_pos - temp->str; /* no of chars before the match */
                                                     // need to cheeck character right before and right match cuz we need to replace to repalce entire words not subparts
            char char_before = (prefix_len > 0) ? temp->str[prefix_len - 1] : ' ';
            char char_after = temp->str[prefix_len + find_len];

            if (!isalnum((unsigned char)char_before) && !isalnum((unsigned char)char_after))
            {
                int old_len = strlen(temp->str);
                int new_len = old_len - find_len + rep_len;
                char *new_str = malloc(new_len + 1); // +1 for '\0'
                if (new_str == NULL)
                {
                    printf("Error: Memory allocation failed during replacement.\n");
                    return FAILURE;
                }
                strncpy(new_str, temp->str, prefix_len); // Copy everything before the match
                new_str[prefix_len] = '\0';              // Null-terminate explicitly
                strcat(new_str, replacer);               // Append the new text
                strcat(new_str, search_pos + find_len);  // Append everything after the match
                free(temp->str);
                temp->str = new_str;
                search_pos = temp->str + prefix_len + rep_len;

                ted->curs.line_no = current_line;
                ted->curs.pos = prefix_len + rep_len;
                count++;
            }
            else
            {
                // part of a larger word  so skip it
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
