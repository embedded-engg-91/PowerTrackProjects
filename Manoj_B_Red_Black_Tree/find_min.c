#include "rbt.h"

int findmin(tree_t *root)
{
    if(root==NULL) return FAILURE;
    
    if(root->left == NULL)
    {
        return root->data;
    }
    return findmin(root->left); 
}
