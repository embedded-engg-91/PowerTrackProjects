#include "rbt.h"

int findmin(tree_t *root)
{
    if(root==NULL) return FAILURE;
    //go on traversing till last node on left side there our min elem is there
    if(root->left == NULL)
    {
        return root->data;
    }
    return findmin(root->left); 
}
