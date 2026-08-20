#include "rbt.h"

Status fix_delete(tree_t **root, tree_t *replacement, tree_t *parent, int was_left) 
{
    if (replacement != NULL && replacement->color == 'R')
    {
        replacement->color = 'B'; 
        return SUCCESS;
    }
    while (parent != NULL)
    {

        
        tree_t *sibling;

        if (was_left)
        {
            sibling = parent->right;
        }
        else
        {
            sibling = parent->left;
        }
        if (sibling == NULL)
        {
            replacement = parent;
            parent = replacement->parent;

            if (parent == NULL)
                return SUCCESS;

            if (parent->left == replacement)
                was_left = 1;
            else
                was_left = 0;

            continue;
        }

        if (sibling->color == 'R')
        {
            
            sibling->color = 'B';
            parent->color = 'R';
            
            
            if (was_left)
            {
                rotate_left(root, &parent); 
                
                sibling = parent->right;
            }
            else
            {
                rotate_right(root, &parent);
                sibling = parent->left;
            }
        }

        char left_clr;
        char right_clr;
        if (sibling->left == NULL || sibling->left == NULL) 
            left_clr = 'B';        
        else
            left_clr = sibling->left->color;

        if (sibling->right == NULL || sibling->right == NULL)
            right_clr = 'B';
        else
            right_clr = sibling->right->color; 

        

        
        
        

        if (left_clr == 'B' && right_clr == 'B') 
        {
            sibling->color = 'R';

            if (parent->color == 'R')
            {
                parent->color = 'B';
                return SUCCESS;
            }
            else
            {
                
                
                replacement = parent;
                parent = replacement->parent;

                
                if (parent == NULL)
                {
                    replacement->color = 'B';
                    return SUCCESS;
                }

                if (parent->left == replacement)
                    was_left = 1;
                else
                    was_left = 0;

                continue;
            }
        }
        

        
        if (was_left)
        {
            if (right_clr == 'R') 
            {
                sibling->color = parent->color;
                parent->color = 'B';

                sibling->right->color = 'B';

                rotate_left(root, &parent);

                return SUCCESS;
            }
            else
            {
                
                sibling->left->color = 'B';

                rotate_right(root, &sibling);

                
                sibling = parent->right;

                

                sibling->color = parent->color;
                parent->color = 'B';

                sibling->right->color = 'B';

                rotate_left(root, &parent);

                return SUCCESS;
            }
        }
        else
        {
            if (left_clr == 'R') 
            {
                sibling->color = parent->color;
                parent->color = 'B';

                sibling->left->color = 'B';

                rotate_right(root, &parent);

                return SUCCESS;
            }
            else
            {
                
                sibling->right->color = 'B';

                rotate_left(root, &sibling);

                
                sibling = parent->left;

                

                sibling->color = parent->color;
                parent->color = 'B';

                sibling->left->color = 'B';

                rotate_right(root, &parent);

                return SUCCESS;
            }
        }
    }
    return SUCCESS;
}
