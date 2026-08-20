#include "rbt.h"

Status fix_insert(tree_t **root, tree_t **node)
{

    
    if (*node == (*root))
        return SUCCESS;
    tree_t *parent = (*node)->parent;
    tree_t *uncle;
    tree_t *grandparent;
    if (parent->color == 'B')
        return SUCCESS;

    get_gp_uncle(*node, &uncle, &grandparent);
    if (uncle != NULL && uncle->color == 'R')
    {
        
        parent->color = 'B';
        uncle->color = 'B';
        if (grandparent != (*root))
        {
            grandparent->color = 'R';
        }
        
        fix_insert(root, &grandparent); 
    }
    else
    {
        if (grandparent->right == parent && parent->right == (*node)) 
        {
            rotate_left(root, &grandparent); 
            swap_colors(&grandparent, &parent);
        }

        else if (grandparent->left == parent && parent->left == (*node)) 
        {
            rotate_right(root, &grandparent); 
            swap_colors(&grandparent, &parent);
        }

        else if (grandparent->left == parent && parent->right == (*node)) 
        {
            
            rotate_left(root, &parent); 
            
            rotate_right(root, &grandparent); 
            swap_colors(&grandparent, node);
        }
        else 
        {
            rotate_right(root, &parent);
            rotate_left(root, &grandparent);
            swap_colors(&grandparent, node);
        }
    }
    return SUCCESS;
}
