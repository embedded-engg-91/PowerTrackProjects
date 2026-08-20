#include "rbt.h"

int findmax(tree_t * root)
{

    if(root==NULL) return FAILURE;

    
    
    if(root->right == NULL)
    {
        return root->data;
    }
    return findmax(root->right);
}

