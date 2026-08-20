#include "rbt.h"

void get_gp_uncle(tree_t *node, tree_t **uncle, tree_t **grandparent)
{
    
    
    if(node->parent==NULL || node->parent->parent == NULL)
    {
        
        *grandparent=NULL;
        *uncle=NULL;
        return;
    }
    else
    {
        
        *grandparent=node->parent->parent;
        if((*grandparent)->right == node->parent)
        {
            *uncle=(*grandparent)->left;
        }
        else
        {
            *uncle=(*grandparent)->right;
        }
    }
    return;
}

tree_t * get_sibling(tree_t *parent, tree_t *replacement)
{
    if(replacement==parent->left)
    {
        return parent->right;
    }
    return parent->left;
}

tree_t *findmin_node(tree_t *root)
{
    if (root == NULL)
        return NULL;

    while (root->left != NULL)
        root = root->left;

    return root;
}
